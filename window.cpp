#include <glwin/font.h>
#include <glwin/winmanager.h>
#include <glwin/window.h>
#include <glwin/scrollbar.h>

#include <sbmessage.h>
#include <sbtokenizer.h>
#include <sbimagepng.h>
using namespace Scenebuilder;

#include <Foundation/UTPreciseTimer.h>

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

int Window::Align::FromString(string s){
	vector<string> toks;
	Tokenizer::Split(toks, s, "|", true);
	int i = 0;
	for(uint j = 0; j < toks.size(); j++){
		if(toks[j] == "absolute") i |= Absolute;
		if(toks[j] == "hfit"    ) i |= HFit    ;
		if(toks[j] == "hcenter" ) i |= HCenter ;
		if(toks[j] == "left"    ) i |= Left    ;
		if(toks[j] == "right"   ) i |= Right   ;
		if(toks[j] == "vfit"    ) i |= VFit    ;
		if(toks[j] == "vcenter" ) i |= VCenter ;
		if(toks[j] == "top"     ) i |= Top     ;
		if(toks[j] == "bottom"  ) i |= Bottom  ;
	}
	return i;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Window::Window(Window* p){
	manager = 0;
	parent  = p;
	if(parent)
		manager = parent->manager;

	hscroll = vscroll = false;
	scrollBar[0] = scrollBar[1] = 0;
	
	enabled   = true;
	focusable = true;
	shown     = true;

	backListId  = -1;
	frameListId = -1;
	//textListId  = -1;
	textTexId   = -1;
	textBufId   = -1;
	backTexId   = -1;

	itemReady = 0;

	// 属性のデフォルト値
	SetPosition      (0.0f, 0.0f);
	SetSize          (0.0f, 0.0f);
	SetStyle         (Style::Normal);
	SetAlign         (Align::HFit | Align::VFit);
	SetSplit         (1, 1);
	SetRowRatio      (0, 1.0f);
	SetColRatio      (0, 1.0f);
	SetMargin        (0.0f, 0.0f);
	SetContentsAlign (Align::HCenter | Align::VCenter);
	SetHScroll       (false);
	SetVScroll       (false);
	SetScrollbarWidth(10.0f);
	SetPopup         (false);

	// デフォルトのパネル割当て
	if(parent){
		uint nrow = (uint)parent->rowRatio.size();
		uint ncol = (uint)parent->colRatio.size();
		uint n    = (uint)parent->children.size();
		colIdx    = n % ncol;
		rowIdx = (n - colIdx) / ncol;
	}
	else{
		colIdx = rowIdx = 0;
	}
	
	// 以下の属性は親ウィンドウから継承する
	SetBackColor ((manager && manager->root) ? manager->root->backColor  : "black"    );
	SetFrameColor((manager && manager->root) ? manager->root->frameColor : "white"    );
	SetTextColor ((manager && manager->root) ? manager->root->textColor  : "white"    );
	SetFontFace  ((manager && manager->root) ? manager->root->fontFace   : "arial.ttf");
	SetFontSize  ((manager && manager->root) ? manager->root->fontSize   : 16         );
	
	if(parent){
		parent->AddChild(this);
		const float eps = 1.0e-2f;
		depth = parent->depth + 1;
	}
	else{
		depth = 0;
	}
}

Window::~Window(){
	Notify(Window::Destroy);
}

void Window::Clear(){
	for(WindowRefs::iterator it = children.begin(); it != children.end(); ){
		Window* c = *it;
		if(c != scrollBar[0] && c != scrollBar[1])
			 it = children.erase(it);
		else it++;
	}
}

void Window::Read(XMLNode* node){
	vec2_t split;
	vvec_t rratio;
	vvec_t cratio;
	
	// 属性取得
	if(node->Get(split , ".split")){
		SetSplit((int)split.x, (int)split.y);
	}
	if(node->Get(rratio, ".row_ratio")){
		for(uint i = 0; i < rratio.size(); i++)
			SetRowRatio(i, (float)rratio[i]);
	}
	if(node->Get(cratio, ".col_ratio")){
		for(uint i = 0; i < cratio.size(); i++)
			SetColRatio(i, (float)cratio[i]);
	}

	bool    b = false;
	int     i = 0;
	float   f = 0.0f;
	Vec2f   f2;
	string  s;
	
	// 継承値よりもXMLの指定を優先
	if(node->Get(s , ".name"           )) SetName          (s);
	if(node->Get(s , ".text"           )) SetText          (s);
	if(node->Get(s , ".hint"           )) SetHint          (s);
	if(node->Get(s , ".back_color"     )) SetBackColor     (s);
	if(node->Get(s , ".back_image"     )) SetBackImage     (s);
	if(node->Get(s , ".frame_color"    )) SetFrameColor    (s);
	if(node->Get(s , ".text_color"     )) SetTextColor     (s);
	if(node->Get(s , ".font_face"      )) SetFontFace      (s);
	if(node->Get(i , ".font_size"      )) SetFontSize      (i);
	if(node->Get(b , ".hscroll"        )) SetHScroll       (b);
	if(node->Get(b , ".vscroll"        )) SetVScroll       (b);
	if(node->Get(f , ".scrollbar_width")) SetScrollbarWidth(f);
	if(node->Get(f2, ".margin"         )) SetMargin        (f2.x, f2.y);
	if(node->Get(f2, ".pos"            )) SetPosition      (f2.x, f2.y);
	if(node->Get(f2, ".size"           )) SetSize          (f2.x, f2.y);
	if(node->Get(s , ".align"          )) SetAlign         (Align::FromString(s));
	if(node->Get(s , ".contents_align" )) SetContentsAlign (Align::FromString(s));
	if(node->Get(f2, ".panel"          )) AssignPanel      ((int)f2.x, (int)f2.y);
	
	// 子ウィンドウ作成
	for(int i = 0; ; i++) try{
		XMLNode* childNode = node->GetNode(i);
		if(childNode->name == "import"){
			// 別ファイルからインポート
			string filename;
			childNode->Get(filename, ".filename");
			if(filename.empty())
				continue;

			try{
				XML xml;
				xml.Load(filename);

				// importタグのfilename以外の属性を移す
				XMLNode* rootNode = xml.GetRootNode();
				for(XMLNode::Attrs::iterator it = childNode->attrs.begin(); it != childNode->attrs.end(); it++){
					if(it->first != "filename")
						rootNode->SetAttr(it->first, it->second);
				}

				ReadChild(rootNode);
			}
			catch(Exception&){
				Message::Error("glwin: error occurred while parsing %s", filename.c_str());
			}
		}
		else{
			ReadChild(childNode);
		}
	}
	catch(Exception&){ break; }
}

void Window::ReadChild(XMLNode* node){
	// RadioGroup
	if(node->name == "group"){
		RadioGroup* group = new RadioGroup(this);
		group->Read(node);
		return;
	}
	// Camera
	if(node->name == "camera"){
		Camera* camera = new Camera(this);
		camera->Read(node);
		return;
	}

	// typeに応じた子ウィンドウを作成
	Window* child = manager->CreateWindow(node->name, this);
		
	if(child){
		child->Read(node);
		if(!child->Init())
			DelChild(child);
	}
}

bool Window::Init(){
	return true;
}

void Window::Notify(int ev){
	for(uint i = 0; i < callbacks.size(); i++)
		callbacks[i]->OnEvent(this, ev);
}

void Window::SetName(const string& n){
	name = n;
}

void Window::SetPosition(float x, float y){
	pos = Vec2f(x, y);
	if(parent)
		 posAbs = parent->posAbs + pos;
	else posAbs = pos;
}

Vec2f Window::GetPosition(){
	return pos;
}

void Window::SetSize(float w, float h){
	szWindow = Vec2f(w, h);
	SetReady(Item::Layout, false);
	SetReady(Item::Rect  , false);
}

Vec2f Window::GetSize(){
	return szWindow;
}

void Window::SetRect(float x, float y, float w, float h){
	SetPosition(x, y);
	SetSize    (w, h);
}

Vec2f Window::GetContentsSize(){
	return szContents;
}

void Window::SetStyle(int s){
	style = s;
}

void Window::SetAlign(int s){
	align = s;
	if(parent)
		parent->SetReady(Item::Layout, false);
}

void Window::SetMargin(float hmargin, float vmargin){
	margin.x = hmargin;
	margin.y = vmargin;
	SetReady(Item::Layout, false);
}

void Window::SetSplit(int nrow, int ncol){
	if(nrow < 1 || ncol < 1)
		return;
	rowRatio.resize(nrow, 0.0f);
	colRatio.resize(ncol, 0.0f);
	rowAbs  .resize(nrow, 0.0f);
	colAbs  .resize(ncol, 0.0f);

	float r;
	r = 1.0f / (float)rowRatio.size();
	for(int i = 0; i < (int)rowRatio.size(); i++)
		rowRatio[i] = r;

	r = 1.0f / (float)colRatio.size();
	for(int i = 0; i < (int)colRatio.size(); i++)
		colRatio[i] = r;

	SetReady(Item::Layout, false);
}

void Window::SetRowRatio(int irow, float ratio){
	if(0 <= irow && irow < (int)rowRatio.size())
		rowRatio[irow] = ratio;

	SetReady(Item::Layout, false);
}

void Window::SetColRatio(int icol, float ratio){
	if(0 <= icol && icol < (int)colRatio.size())
		colRatio[icol] = ratio;

	SetReady(Item::Layout, false);
}

void Window::AssignPanel(int irow, int icol){
	rowIdx = irow;
	colIdx = icol;

	SetReady(Item::Layout, false);
}
	
void Window::SetFrameColor(const string& c){
	frameColor = c;
	SetReady(Item::Color, false);
}

void Window::SetBackColor(const string& c){
	backColor = c;
	SetReady(Item::Color, false);
}

void Window::SetTextColor(const string& c){
	textColor = c;
	SetReady(Item::Color, false);
	SetReady(Item::Font , false);
}

void Window::SetBackImage(const string& filename){
	backImage = filename;
	SetReady(Item::Image, false);
}

void Window::SetText(const wstring& t){
	text = t;
	SetReady(Item::Text, false);
}

void Window::GetText(wstring& t){
	t = text;
}

void Window::SetText(const string& t){
	wstring ws;
	Converter::ConvertString(ws, t);
	SetText(ws);
}

void Window::GetText(string& t){
	wstring ws;
	GetText(ws);
	Converter::ConvertString(t, ws);
}

void Window::SetHint(const wstring& t){
	hint = t;
}

void Window::GetHint(wstring& t){
	if(hint.empty() && parent)
		 parent->GetHint(t);
	else t = hint;
}

void Window::SetHint(const string& t){
	wstring ws;
	Converter::ConvertString(ws, t);
	SetHint(ws);
}

void Window::GetHint(string& t){
	wstring ws;
	GetHint(ws);
	Converter::ConvertString(t, ws);
}

void Window::SetContentsAlign(int align){
	contentsAlign = align;
	SetReady(Item::Color, false);
}

void Window::SetFontFace(const string& face){
	fontFace = face;
	SetReady(Item::Font, false);
}

void Window::SetFontSize(int sz){
	fontSize = sz;
	SetReady(Item::Font, false);
}

void Window::SetHScroll(bool on){
	hscroll = on;
	if(hscroll && !scrollBar[0]){
		scrollBar[0] = new ScrollBar(this);
		scrollBar[0]->SetStyle(ScrollBar::Style::Horizontal);
	}
	SetReady(Item::Layout     , false);
	SetReady(Item::ScrollRange, false);
}

void Window::SetVScroll(bool on){
	vscroll = on;
	if(vscroll && !scrollBar[1]){
		scrollBar[1] = new ScrollBar(this);
		scrollBar[1]->SetStyle(ScrollBar::Style::Vertical);
	}
	SetReady(Item::Layout     , false);
	SetReady(Item::ScrollRange, false);
}

void Window::SetScrollbarWidth(float w){
	scrollBarWidth = w;
	SetReady(Item::Layout, false);
}

void Window::SetPopup(bool on){
	popup = on;
	if(!parent)
		return;

	Wins::iterator it = find(manager->popups.begin(), manager->popups.end(), this);
	if( popup && it == manager->popups.end())
		manager->popups.push_back(this);
	if(!popup && it != manager->popups.end())
		manager->popups.erase(it);

	parent->SetReady(Item::Layout, false);
}
	
void Window::AddChild(Window* w){
	w->parent = this;
	children.push_back(w);
	SetReady(Item::Layout, false);
}

void Window::DelChild(Window* w){
	children.erase(find(children.begin(), children.end(), w));
	SetReady(Item::Layout, false);
}

Window* Window::FindChild(string _name){
	if(name == _name)
		return this;

	for(uint i = 0; i < children.size(); i++){
		Window* w = children[i]->FindChild(_name);
		if(w)
			return w;
	}
	return 0;
}

void Window::SetReady(int item, bool on){
	if(on)
		 itemReady |=  item;
	else itemReady &= ~item;
}

bool Window::IsReady(int item){
	return !!(itemReady & item);
}

void Window::SetColor(const Vec4f& color){
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	glColor4fv(color);
}

bool Window::Prepare(int mask){
	if((mask & Item::Color) && !IsReady(Item::Color)){
		Converter::ColorFromName( backColor,  backColorRGB);
		Converter::ColorFromName(frameColor, frameColorRGB);
		Converter::ColorFromName( textColor,  textColorRGB);
		SetReady(Item::Color, true);
	}

	if((mask & Item::Font) && !IsReady(Item::Font)){
		font = manager->fontManager->Get(fontFace, fontSize, textColor);
		SetReady(Item::Font, true);

		// フォントサイズが変わっている場合に必要
		SetReady(Item::ContentsPos, false);
	}

	if((mask & Item::Image) && !IsReady(Item::Image)){
		if(backTexId != -1){
			glDeleteTextures(1, (GLuint*)&backTexId);
			backTexId = -1;
		}

		ImagePNG image;
		if(!backImage.empty() && image.Load(backImage)){
			// テクスチャ生成
			glGenTextures(1, (GLuint*)&backTexId);
			glBindTexture  (GL_TEXTURE_2D, backTexId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*GL_NEAREST*/);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_NEAREST*/);
			glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image.pixels[0]);
			glBindTexture  (GL_TEXTURE_2D, 0);
		}
		
		SetReady(Item::Image, true);

		SetReady(Item::Rect, false);
	}
	
	if((mask & Item::Rect) && !IsReady(Item::Rect)){
		GRRenderIf* render = manager->render;

		// ピクセルの中心を頂点とする
		float w = szWindow.x;
		float h = szWindow.y;

		struct V3{
			Vec3f pos;
		};
		struct T2V3{
			Vec2f tex;
			Vec3f pos;
		};
		V3   v3  [4];
		T2V3 t2v3[4];

		// 背景: 頂点座標で始点と終点が[i,j]とすると[i,j)のピクセルがフィルされる．yは反転してるので注意
		t2v3[0].tex = Vec2f(0.0f, 0.0f);
		t2v3[1].tex = Vec2f(0.0f, 1.0f);
		t2v3[2].tex = Vec2f(1.0f, 1.0f);
		t2v3[3].tex = Vec2f(1.0f, 0.0f);
		t2v3[0].pos = Vec3f(    0.5f,   + 0.5f, 0.0f);
		t2v3[1].pos = Vec3f(    0.5f, h + 0.5f, 0.0f);
		t2v3[2].pos = Vec3f(w + 0.5f, h + 0.5f, 0.0f);
		t2v3[3].pos = Vec3f(w + 0.5f,   + 0.5f, 0.0f);
		if(backListId != -1)
			render->ReleaseList(backListId);
		backListId = render->StartList();
		glInterleavedArrays(GL_T2F_V3F, sizeof(T2V3), t2v3);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		render->EndList();

		// 枠: 始点と終点を[i,j]とすると[i,j]がフィルされる
		v3[0].pos = Vec3f(0.5f,     0.5f,     0.0f);
		v3[1].pos = Vec3f(0.5f,     h - 0.5f, 0.0f);
		v3[2].pos = Vec3f(w - 0.5f, h - 0.5f, 0.0f);
		v3[3].pos = Vec3f(w - 0.5f, 0.5f,     0.0f);
		if(frameListId != -1)
			render->ReleaseList(frameListId);
		frameListId = render->StartList();
		glInterleavedArrays(GL_V3F, sizeof(V3), v3);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		render->EndList();
		
		SetReady(Item::Rect       , true );
		SetReady(Item::ContentsPos, false);
	}
	
	if((mask & Item::Text) && !IsReady(Item::Text)){
		Prepare(Item::Font);
		GRRenderIf* render = manager->render;

		// テキストを行分割
		TokenizerW::Split(textLines, text, L"\n", false);
		if(!textLines.empty()){
			// 上と下の空行を除去
			vector<wstring>::iterator it0, it1;
			it0 = textLines.begin();
			it1 = textLines.end();
			while(it0 != it1 && (it1-1)->empty())
				it1--;
			while(it0 != it1 && it0->empty())
				it0++;
			textLines = vector<wstring>(it0, it1);
		}

		// 各行の両端の空白文字を除去
		for(uint i = 0; i < textLines.size(); i++)
			textLines[i] = to_string(eat_white(wstring_iterator_pair(textLines[i])));

		// フォントを考慮してテキスト領域の大きさを計算
		szText = Vec2f();
		if(textLines.empty()){
			// テキストが空でも高さを確保
			szText.y = (float)fontSize;
		}
		else{
			float advance, ascent, descent;
			for(uint i = 0; i < textLines.size(); i++){
				font->GetMetric(textLines[i], advance, ascent, descent);
				szText.x  = std::max(szText.x, advance);
				szText.y += (ascent - descent);
			}

			// テクスチャ作成
			if(textTexId != -1)
				glDeleteTextures(1, (GLuint*)&textTexId);
			glGenTextures       (1, (GLuint*)&textTexId);
			glBindTexture  (GL_TEXTURE_2D, textTexId);
			glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA, (size_t)szText.x, (size_t)szText.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			// フレームバッファ作成
			if(textBufId != -1)
				glDeleteFramebuffers(1, (GLuint*)&textBufId);
			glGenFramebuffers       (1, (GLuint*)&textBufId);

			// フレームバッファにテクスチャをバインド
			glBindFramebuffer(GL_FRAMEBUFFER, textBufId);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textTexId, 0);
			// バッファをクリア（しないと以前のテクスチャが残ることがある）
			glClear(GL_COLOR_BUFFER_BIT);

			// ビューポートと投影変換
			Vec2f   vpPos  = render->GetViewportPos ();
			Vec2f   vpSize = render->GetViewportSize();
			Affinef affProjTmp;
			render->GetProjectionMatrix(affProjTmp);

			float w = szText.x;
			float h = szText.y;
			Affinef affProj;
			affProj.item(0,0) =  2.0f / w;
			affProj.item(1,1) = -2.0f / h;
			affProj.item(2,2) = -1.0f;
			affProj.item(3,3) =  1.0f;
			affProj.item(0,3) = -1.0f;
			affProj.item(1,3) =  1.0f;
			render->SetViewport(Vec2f(), Vec2f(w, h));
			render->SetProjectionMatrix(affProj);

			Vec2f p(0.0f, (ascent - descent));

			for(uint i = 0; i < textLines.size(); i++){
				font->Draw(render, p, textLines[i]);
				p.y += font->lineSkip;
			}
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			render->SetViewport(vpPos, vpSize);
			render->SetProjectionMatrix(affProjTmp);

			/*
			// テキスト描画用ディスプレイリスト
			if(textListId != -1)
				render->ReleaseList(textListId);
			textListId = render->StartList();
			for(uint i = 0; i < textLines.size(); i++){
				font->Draw(render, p, textLines[i]);
				p.y += font->lineSkip;
			}
			render->EndList();
			*/
		}

		SetReady(Item::Text       , true );
		SetReady(Item::ContentsPos, false);
	}

	// 子ウィンドウレイアウト
	if((mask & Item::Layout) && !IsReady(Item::Layout)){
		Layout();
		SetReady(Item::Layout     , true );
		SetReady(Item::ScrollRange, false);
	}

	// スクロール範囲
	if((mask & Item::ScrollRange) && !IsReady(Item::ScrollRange)){
		// コンテンツサイズ計算
		CalcContentsSize();
		// スクロール範囲更新
		if(hscroll)
			scrollBar[0]->UpdateRange();
		if(vscroll)
			scrollBar[1]->UpdateRange();

		SetReady(Item::ScrollRange, true);
	}
	
	// コンテンツ表示位置の計算
	if((mask & Item::ContentsPos) && !IsReady(Item::ContentsPos)){
		CalcContentsSize();
		contentsPos = Vec2f();
		if(contentsAlign & Align::Left)
			 contentsPos.x = margin.x;
		else if(contentsAlign & Align::Right)
			 contentsPos.x = szWindow.x - margin.x - szContents.x;
		else contentsPos.x = 0.5f * (szWindow.x - szContents.x);

		if(contentsAlign & Align::Top)
			 contentsPos.y = margin.y;
		else if(contentsAlign & Align::Bottom)
			 contentsPos.y = szWindow.y - margin.y - szContents.y;
		else contentsPos.y = 0.5f * (szWindow.y - szContents.y);
		
		SetReady(Item::ContentsPos, true);
	}
	return true;
}

void Window::Draw(){
	GRRenderIf* render = manager->render;

	// このウィンドウ領域用のビットマスクをステンシルに描画
	glColorMask  (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask  (GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0xff, 0xff);
	glStencilOp  (GL_REPLACE, GL_REPLACE, GL_REPLACE);
	if(popup)
		 glStencilMask((0x1 << (depth+1)) - 1);
	else glStencilMask(0x1 << depth);
	render->DrawList(backListId);
	//render->DrawList(frameListId);
	glColorMask  (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask  (GL_TRUE);
	glStencilMask(0);
	
	// ステンシルマスク設定
	// ルートウィンドウからこのウィンドウまでのすべてのビットが立っている領域のみ描画可能
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, (0x1 << (depth+1))-1, 0xff);
	
	DrawBack();

	// スクロールによる平行移動
	glPushMatrix();
	if(scrollBar[0] && scrollBar[0]->szWindow.x > scrollBar[0]->barLength){
		float ratio = (szContents.x - scrollBar[0]->szWindow.x) / (scrollBar[0]->szWindow.x - scrollBar[0]->barLength);
		glTranslatef(-ratio * scrollBar[0]->barPos, 0.0f, 0.0f);
	}
	if(scrollBar[1] && scrollBar[1]->szWindow.y > scrollBar[1]->barLength){
		float ratio = (szContents.y - scrollBar[1]->szWindow.y) / (scrollBar[1]->szWindow.y - scrollBar[1]->barLength);
		glTranslatef(0.0f, -ratio * scrollBar[1]->barPos, 0.0f);
	}
	
	glPushMatrix();
	glTranslatef(contentsPos.x, contentsPos.y, 0.0f);
	DrawContents();
	glPopMatrix();

	// 子ウィンドウ（非表示，ポップアップ，スクロールバー以外）
	for(uint i = 0; i < children.size(); i++){
		Window* child = children[i];
		if(!child->shown || child->popup || child == scrollBar[0] || child == scrollBar[1])
			continue;
		
		glPushMatrix();
		glTranslatef(child->pos.x, child->pos.y, 0.0f);
		child->Draw();
		glPopMatrix();
	}

	glPopMatrix();

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, (0x1 << (depth+1))-1, 0xff);

	// スクロールバー
	for(int i = 0; i < 2; i++){
		Window* sc = scrollBar[i];
		if(sc){
			glPushMatrix();
			glTranslatef(sc->pos.x, sc->pos.y, 0.0f);
			sc->Draw();
			glPopMatrix();
		}
	}

	DrawFrame();

	// ステンシルのビットマスクをクリア
	glColorMask  (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask  (GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0x00, 0xff);
	glStencilOp  (GL_REPLACE, GL_REPLACE, GL_REPLACE);
	if(popup)
		 glStencilMask((0x1 << (depth+1)) - 1);
	else glStencilMask(0x1 << depth);
	render->DrawList(backListId);
	//render->DrawList(frameListId);
	glColorMask  (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask  (GL_TRUE);
	glStencilMask(0);
}

void Window::DrawFrame(){
	GRRenderIf* render = manager->render;

	Vec4f c = CalcFrameColor();
	if(c.w != 0.0f){
		SetColor(c);
		render->SetLineWidth(CalcFrameWidth());
		render->DrawList(frameListId);	
	}
}

void Window::DrawBack(){
	GRRenderIf* render = manager->render;

	Vec4f c = CalcBackColor();
	if(c.w != 0.0f){
		SetColor(c);
		if(backTexId != -1){
			render->SetTexture2D(true);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBindTexture(GL_TEXTURE_2D, backTexId);
		}
		render->DrawList(backListId);
		if(backTexId != -1){
			glBindTexture(GL_TEXTURE_2D, 0);
			render->SetTexture2D(false);
		}
	}
}

void Window::DrawContents(){
	GRRenderIf* render = manager->render;

	render->SetTexture2D(true);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, textTexId);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f    , szText.y);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(szText.x, szText.y);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f    , 0.0f    );
	glTexCoord2f(1.0f, 1.0f); glVertex2f(szText.x, 0.0f    );
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	render->SetTexture2D(false);
	//render->DrawList(textListId);
}

bool Window::HitTest(Vec2f p, Window** w, int* dmax){
	if(!shown)
		return false;
	bool inside =
		(posAbs.x <= p.x && p.x < posAbs.x + szWindow.x) &&
		(posAbs.y <= p.y && p.y < posAbs.y + szWindow.y);
	if(!inside)
		return false;
	if(depth > *dmax){
		*w    = this;
		*dmax = depth;
	}
	return true;
}

void Window::AddCallback(WindowCallback* cb){
	if(find(callbacks.begin(), callbacks.end(), cb) == callbacks.end())
		callbacks.push_back(cb);
}

void Window::RemoveCallback(WindowCallback* cb){
	callbacks.erase(find(callbacks.begin(), callbacks.end(), cb));
}

void Window::Enable(bool on){
	enabled = on;
}

void Window::Show(bool on){
	shown = on;
}

bool Window::IsShown(){
	bool ret = shown;
	if(parent)
		ret &= parent->IsShown();
	return ret;
}

void Window::CalcContentsSize(){
	// デフォルトでテキストサイズ
	Prepare(Item::Text);
	szContents.x = szText.x;
	szContents.y = szText.y;
}

void Window::CalcWindowSize(){
	// デフォルトでコンテンツサイズ＋マージン
	//CalcContentsSize();
	//szWindow.x = 2.0f * margin.x + szContents.x;
	//szWindow.y = 2.0f * margin.y + szContents.y;
}

void Window::Layout(){
	// 子ウィンドウのサイズ計算
	for(uint i = 0; i < children.size(); i++)
		children[i]->CalcWindowSize();

	uint nrow = (uint)rowRatio.size();
	uint ncol = (uint)colRatio.size();
	panels.resize(nrow * ncol);
	
	// スクロールバーが表示されている場合はコンテンツサイズ，それ以外の場合はウィンドウサイズを基準にレイアウトする
	float wrect = (hscroll ? szContents.x : szWindow.x);
	float hrect = (vscroll ? szContents.y : szWindow.y);
	
	float w = wrect;
	float h = hrect;

	// まずマージンを差し引く
	w -= margin.x * (1 + ncol);
	h -= margin.y * (1 + nrow);
	// スクロールバーの幅を差し引く
	if(hscroll)
		w -= scrollBarWidth;
	if(vscroll)
		h -= scrollBarWidth;
	w = std::max(w, 0.0f);
	h = std::max(h, 0.0f);
	
	float rowRatioSum = 0.0f;
	float colRatioSum = 0.0f;
	for(uint r = 0; r < nrow; r++){
		// ratioが1.0より大きい値ならばピクセル値として扱う
		if(rowRatio[r] > 1.0f){
			rowAbs[r] = rowRatio[r];
			h = std::max(0.0f, h - rowAbs[r]);
		}
		// 0.0なら子ウィンドウの幅で決める
		else if(rowRatio[r] == 0.0f){
			rowAbs[r] = 0.0f;
			for(uint c = 0; c < ncol; c++){
				uint i = ncol * r + c;
				if(i < children.size())
					rowAbs[r] = std::max(rowAbs[r], children[i]->szWindow.y);
			}
			h = std::max(0.0f, h - rowAbs[r]);
		}
		// 1.0より小さい値なら比率として扱う
		else rowRatioSum += rowRatio[r];
	}
	for(uint c = 0; c < ncol; c++){
		if(colRatio[c] > 1.0f){
			colAbs[c] = colRatio[c];
			w = std::max(0.0f, w - colAbs[c]);
		}
		else if(colRatio[c] == 0.0f){
			colAbs[c] = 0.0f;
			for(uint r = 0; r < nrow; r++){
				uint i = ncol * r + c;
				if(i < children.size())
					colAbs[c] = std::max(colAbs[c], children[i]->szWindow.x);
			}
			w = std::max(0.0f, w - colAbs[c]);
		}
		else colRatioSum += colRatio[c];
	}
	// 固定幅の分を差し引いた残りを比率で割る
	for(uint r = 0; r < nrow; r++){
		if(0.0f < rowRatio[r] && rowRatio[r] <= 1.0f)
			rowAbs[r] = h * (rowRatio[r] / rowRatioSum);
	}
	for(uint c = 0; c < ncol; c++){
		if(0.0f < colRatio[c] && colRatio[c] <= 1.0f)
			colAbs[c] = w * (colRatio[c] / colRatioSum);
	}

	for(uint r = 0; r < nrow; r++){
		for(uint c = 0; c < ncol; c++){
			uint i = ncol * r + c;
			panels[i].left   = (c == 0 ? margin.x : panels[i-1   ].right  + margin.x);
			panels[i].top    = (r == 0 ? margin.y : panels[i-ncol].bottom + margin.y);
			panels[i].right  = panels[i].left + colAbs[c];
			panels[i].bottom = panels[i].top  + rowAbs[r];
		}
	}

	for(uint i = 0; i < children.size(); i++){
		Window* child = children[i];

		// スクロールバーの配置
		if(child == scrollBar[0]){
			child->SetPosition(0.0f, szWindow.y - scrollBarWidth);
			child->SetSize(szWindow.x - (scrollBar[1] ? scrollBarWidth : 0), scrollBarWidth);
			continue;
		}
		if(child == scrollBar[1]){
			child->SetPosition(szWindow.x - scrollBarWidth, 0.0f);
			child->SetSize(scrollBarWidth, szWindow.y - (scrollBar[0] ? scrollBarWidth : 0));
			continue;
		}

		Rect rc;
		Vec2f sz  = child->GetSize    ();
		Vec2f pos = child->GetPosition(); 
		
		// その他子ウィンドウの配置
		if(child->align == Align::Absolute){
			rc.left   = (pos.x > 1.0f ? pos.x : w * pos.x);
			rc.top    = (pos.y > 1.0f ? pos.y : h * pos.y);
			rc.right  = rc.left + (sz.x > 1.0f ? sz.x : w * sz.x);
			rc.bottom = rc.top  + (sz.y > 1.0f ? sz.y : h * sz.y);
		}
		else{
			uint idx = child->rowIdx * ncol + child->colIdx;
			// 割当て位置がパネルの範囲外
			if(idx >= panels.size()){
				rc.left   = 0.0f;
				rc.top    = 0.0f;
				rc.right  = 0.0f;
				rc.bottom = 0.0f;
			}
			else{
				Rect& pnl = panels[idx];

				if(child->align & Align::HFit){
					rc.left  = pnl.left;
					rc.right = pnl.right;
				}
				if(child->align & Align::HCenter){
					rc.left  = pnl.HCenter() - 0.5f * sz.x;
					rc.right = pnl.HCenter() + 0.5f * sz.x;
				}
				if(child->align & Align::Left){
					rc.left  = pnl.left;
					rc.right = pnl.left + sz.x;
				}
				if(child->align & Align::Right){
					rc.left  = pnl.right - sz.x;
					rc.right = pnl.right;
				}
				if(child->align & Align::VFit){
					rc.top    = pnl.top;
					rc.bottom = pnl.bottom;
				}
				if(child->align & Align::VCenter){
					rc.top    = pnl.VCenter() - 0.5f * sz.y;
					rc.bottom = pnl.VCenter() + 0.5f * sz.y;
				}
				if(child->align & Align::Top){
					rc.top    = pnl.top;
					rc.bottom = pnl.top + sz.y;
				}
				if(child->align & Align::Bottom){
					rc.top    = pnl.bottom - sz.y;
					rc.bottom = pnl.bottom;
				}
			}
		}
		child->SetRect(rc.left, rc.top, rc.Width(), rc.Height());
	}
}

bool Window::OnUpdate(){
	Notify(Window::Update);
	return true;
}

}
