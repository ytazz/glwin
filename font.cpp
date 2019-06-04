#include <glwin/font.h>

#include <sbconverter.h>
#include <sbmessage.h>
using namespace Scenebuilder;

#define USE_FREEGLUT
#include <GL/glew.h>
#include <GL/glut.h>

namespace GLWin{;

Font::Glyph::Glyph(){
	texId   = 0;
	x       = 0;
	y       = 0;
	w       = 0;
	h       = 0;
	advance = 0;
	umax    = 0.0f;
	vmax    = 0.0f;
}

void Font::Glyph::AddMetric(float& advance, float& ascent, float& descent){
	advance += this->advance;
	ascent  = std::max(ascent , (float)(y + h));
	descent = std::min(descent, (float)y);
}

void Font::Glyph::Draw(GRRenderIf* render, Vec2f pos){
	if(!texId)
		return;

	// SDL_TTF 2.0 から仕様が変わった模様
	float xf = 0.0f;//(float)x;
	float yf = 0.0f;//(float)y;
	float wf = (float)w;
	float hf = (float)h;

	render->SetTexture2D(true);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0f, vmax); glVertex2f(pos.x + xf,      pos.y - yf     );
	glTexCoord2f(umax, vmax); glVertex2f(pos.x + xf + wf, pos.y - yf     );
	glTexCoord2f(0.0f, 0.0f); glVertex2f(pos.x + xf,      pos.y - yf - hf);
	glTexCoord2f(umax, 0.0f); glVertex2f(pos.x + xf + wf, pos.y - yf - hf);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	render->SetTexture2D(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Font::Font(FontManager* _manager){
	manager  = _manager;
	height   = 0;
	ascent   = 0;
	descent  = 0;
	lineSkip = 0;
	font     = 0;
}

Font::~Font(){
	
}

int PowerOfTwo(int x){
	int y = 1;
	while(y < x)
		y *= 2;
	return y;
}

bool Font::Init(){
	// フォントをロード
#if defined _WIN32
	if( !(font = TTF_OpenFont( name                           .c_str(), size)) &&
		!(font = TTF_OpenFont( ("fonts\\" + name)             .c_str(), size)) &&
		!(font = TTF_OpenFont( ("c:\\windows\\fonts\\" + name).c_str(), size)) ){
		Message::Error("failed to load font %s:%d", name.c_str(), size);
		return false;
	}
#elif defined __unix__
	if( !(font = TTF_OpenFont( name                           .c_str(), size)) &&
		!(font = TTF_OpenFont( ("fonts/" + name)             .c_str(), size)) &&
		!(font = TTF_OpenFont( ("/usr/share/fonts/" + name).c_str(), size)) ){
		Message::Error("failed to load font %s:%d", name.c_str(), size);
		return false;
	}
#endif

	// 属性取得
	height   = TTF_FontHeight  (font);
	ascent   = TTF_FontAscent  (font);
	descent  = TTF_FontDescent (font);
	lineSkip = TTF_FontLineSkip(font);

	// 色名からRGB
	Vec4f c;
	Converter::ColorFromName(color, c);
	colorRGB.r = (uint)(255.0 * c[0]);
	colorRGB.g = (uint)(255.0 * c[1]);
	colorRGB.b = (uint)(255.0 * c[2]);
	colorRGB.a = (uint)(255.0);

	return true;
}

void Font::Close(){
	if(font)
		TTF_CloseFont(font);
}

Font::Glyph* Font::CreateGlyph(wchar_t c){
	SDL_Color bg;
	bg.r = bg.g = bg.b = 0;

	SDL_Surface* image   = 0;
	SDL_Surface* surface = 0;

	UTRef<Glyph> g = new Glyph();

	int xmax, ymax;
	TTF_GlyphMetrics(font, c, &g->x, &xmax, &g->y, &ymax, &g->advance);
	
	if(manager->mode == FontManager::Solid){
		image = TTF_RenderGlyph_Solid(font, c, colorRGB);
		SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	}
	if(manager->mode == FontManager::Shaded){
		image = TTF_RenderGlyph_Shaded(font, c, colorRGB, bg);
	}
	if(manager->mode == FontManager::Blended){
		image = TTF_RenderGlyph_Blended(font, c, colorRGB);
	}
	if(!image)
		return 0;

	// アルファチャンネルもそのままコピーしたいのでper-surface-alphaをオフ
	//SDL_SetAlpha(image, 0, 0);
	//SDL_SetSurfaceAlphaMod(image, 255);

	// GLのテクスチャに変換
	int wtex = PowerOfTwo(image->w);
	int htex = PowerOfTwo(image->h);
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, wtex, htex, 32, 0xff, 0xff << 8, 0xff << 16, 0xff << 24);
	if(!surface)
		return 0;
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

	g->w = image->w;
	g->h = image->h;
	g->umax = (float)image->w / (float)wtex;
	g->vmax = (float)image->h / (float)htex;

	// Copy the surface into the GL texture image
	SDL_BlitSurface(image, 0, surface, 0);

	uint* pix = (uint*)surface->pixels;
	uint* impix = (uint*)image->pixels;

	// Create an OpenGL texture for the image
	glGenTextures(1, (GLuint*)&g->texId);
	glBindTexture(GL_TEXTURE_2D, g->texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*GL_NEAREST*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_NEAREST*/);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wtex, htex, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(image);
	SDL_FreeSurface(surface);
	
	glyphs[c] = g;
	return g;
}

Font::Glyph* Font::GetGlyph(wchar_t c){
	Glyphs::iterator it = glyphs.find(c);
	if(it == glyphs.end()){
		return CreateGlyph(c);
	}
	return it->second;
}

void Font::GetMetric(const string& text, float& advance, float& ascent, float& descent){
	advance = ascent = descent = 0.0f;
	for(uint i = 0; i < text.size(); i++){
		Glyph* g = GetGlyph((wchar_t)text[i]);
		if(g)
			g->AddMetric(advance, ascent, descent);
	}
}

void Font::GetMetric(const wstring& text, float& advance, float& ascent, float& descent){
	advance = ascent = descent = 0.0f;
	for(uint i = 0; i < text.size(); i++){
		Glyph* g = GetGlyph(text[i]);
		if(g)
			g->AddMetric(advance, ascent, descent);
	}
}

template<typename T>
void DoDraw(Font* font, GRRenderIf* render, Vec2f pos, const basic_string<T>& str){
	//  色を白に設定(テクスチャ色がそのまま出る)
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	Vec2f inipos = pos;

	for(uint i = 0; i < str.size(); i++){
		T c = str[i];
		if(c == (T)'\n'){
			pos.y += (float)font->lineSkip;
			pos.x =  inipos.x;
		}
		else{
			Font::Glyph* g = font->GetGlyph(str[i]);
			if(g){
				g->Draw(render, pos);
				pos.x += (float)g->advance;
			}
		}
	}
}

void Font::Draw(GRRenderIf* render, Vec2f pos, const string& str){
	DoDraw(this, render, pos, str);
}

void Font::Draw(GRRenderIf* render, Vec2f pos, const wstring& str){
	DoDraw(this, render, pos, str);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FontManager::FontManager(){
	//mode = Solid;
	//mode = Shaded;
	mode = Blended;
}

bool FontManager::Init(){
	// SDL_ttf初期化
	TTF_Init();
	return true;
}

void FontManager::Close(){
	for(uint i = 0; i < size(); i++)
		at(i)->Close();

	TTF_Quit();
}

Font* FontManager::Add(const string& name, int size, const string& color){
	UTRef<Font> f = new Font(this);
	f->name  = name;
	f->size  = size;
	f->color = color;
	if(f->Init()){
		push_back(f);
		return f;
	}
	return 0;
}

Font* FontManager::Get(const string& name, int size, const string& color){
	// if no font is registered, add default font
	if(empty()){
#if defined _WIN32
		Add("arial.ttf", 16, "white");
#elif defined __unix__
		Add("truetype/dejavu/DejaVuSansMono.ttf", 16, "white");
#endif
	}

	// find font that matches required attributes
	for(uint i = 0; i < this->size(); i++){
		Font* font = at(i);
		if(font->name == name && font->size == size && font->color == color)
			return font;
	}

	// if there is no match, create a new one
	Font* f = Add(name, size, color);
	if(f)
		return f;
	
	// if failed to add new one, return default font
	return at(0);
}

}
