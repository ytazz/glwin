#include <glwin/font.h>
#include <glwin/winmanager.h>
#include <glwin/textbox.h>
using namespace Scenebuilder;

namespace GLWin{;

Textbox::Textbox(Window* p):Window(p){
	SetFrameColor("none");
	SetContentsAlign(Align::Left | Align::VCenter);

	AddCallback(manager);

	cursorPos = 0;
	maxLength = 1000;
}

void Textbox::Read(XMLNode* node){
	Window::Read(node);
	node->Get(maxLength, ".max_length");
}

void Textbox::SetMaxLength(int len){
	maxLength = len;
}

bool Textbox::OnEvent(SDL_Event* ev){
	bool ret = false;

	if(ev->type == SDL_TEXTINPUT){
		int code = ev->text.text[0];

		// ascii code‚Ì’Êí•¶š
		if(' ' <= code && code <= '~' && (int)text.size() < maxLength){
			text.insert(text.begin() + cursorPos, code);
			cursorPos++;
			SetReady(Item::Text, false);
			Notify(TypeAscii);

			ret = true;
		}
	}
	if(ev->type == SDL_KEYDOWN){
		int sym  = ev->key.keysym.sym;
	
		// backspace
		if(sym == SDLK_BACKSPACE){
			if(cursorPos != 0){
				cursorPos--;
				text.erase(text.begin() + cursorPos);
				SetReady(Item::Text, false);
			}
			ret = true;
		}
		// delete
		if(sym == SDLK_DELETE){
			if(cursorPos != text.size() && !text.empty()){
				text.erase(text.begin() + cursorPos);
				SetReady(Item::Text, false);
			}
			ret = true;
		}
		// left
		if(sym == SDLK_LEFT){
			if(cursorPos != 0)
				cursorPos--;
			ret = true;
		}
		// right
		if(sym == SDLK_RIGHT){
			if(cursorPos != text.size())
				cursorPos++;
			ret = true;
		}
		// home
		if(sym == SDLK_HOME){
			cursorPos = 0;
			ret = true;
		}
		// end
		if(sym == SDLK_END){
			cursorPos = (uint)text.size();
			ret = true;
		}
	}
	return true;
}

Vec4f Textbox::CalcBackColor(){
	return 0.8f * backColorRGB;
}

void Textbox::DrawContents(){
	GRRenderIf* render = manager->render;

	Vec2f p(fontSize/2, fontSize);
	
	// “ü—Í•¶š—ñ
	font->Draw(render, p, text);
	
	// ƒJ[ƒ\ƒ‹
	if(manager->focusedWin == this){
		float pos = p.x;
		for(int i = 0; i < cursorPos; i++)
			pos += font->GetGlyph(text[i])->advance;
		Vec3f p0, p1;
		p0.x = p1.x = pos;
		p0.y = 5.0f;
		p1.y = 5.0f + (float)fontSize;
		render->DrawLine(p0, p1);
		glFlush();
	}
}

}
