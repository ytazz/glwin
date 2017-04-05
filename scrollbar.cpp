#include <glwin/outline.h>
#include <glwin/winmanager.h>
#include <glwin/scrollbar.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

ScrollBar::ScrollBar(Window* p):Window(p){
	SetFrameColor("none");
	SetStyle(Style::Horizontal);
	SetContentsAlign(Align::Left | Align::Top);

	barPushed = false;
	barPos    = 0.0f;
	barLength = 0.0f;
	barListId = -1;
}

void ScrollBar::SetStyle(int s){
	style = s;
	SetReady(Item::Bar, false);
}

void ScrollBar::UpdateRange(){
	// 親ウィンドウのウィンドウサイズとコンテンツサイズの比からスクロール範囲を設定
	float ratio;
	Vec2f& szw = parent->szWindow;
	Vec2f& szc = parent->szContents;
	if(style == Style::Horizontal){
		ratio     = (szc.x <= szw.x ? 1.0f : szw.x / szc.x);
		barLength = szWindow.x * ratio;
		barPos    = std::min(barPos, szWindow.x - barLength);
	}
	if(style == Style::Vertical){
		ratio     = (szc.y <= szw.y ? 1.0f : szw.y / szc.y);
		barLength = szWindow.y * ratio;
		barPos    = std::min(barPos, szWindow.y - barLength);
	}
	SetReady(Item::Bar, false);
}

bool ScrollBar::OnEvent(SDL_Event* ev){
	bool ret = false;

	if(ev->type == SDL_MOUSEBUTTONDOWN){
		Vec2f p = vec2_t(ev->button.x, ev->button.y) - posAbs;

		if(style == Style::Horizontal){
			// バーの左
			if(0.0 <= p.x && p.x < barPos)
				barPos = p.x;
			// バーの右
			if(barPos + barLength <= p.x && p.x < szWindow.x)
				barPos = p.x - barLength;
			
			if(barPos <= p.x && p.x < barPos + barLength){
				barPushed  = true;
				pushPos    = p;
				pushBarPos = barPos;
			}
		}
		else{	
			// バーの上
			if(0.0 <= p.y && p.y < barPos)
				barPos = p.y;			
			// バーの下
			if(barPos + barLength <= p.y && p.y < szWindow.y)
				barPos = p.y - barLength;

			if(barPos <= p.y && p.y < barPos + barLength){
				barPushed  = true;
				pushPos    = p;
				pushBarPos = barPos;
			}
		}
		ret = true;
	}
	if(ev->type == SDL_MOUSEBUTTONUP){
		barPushed = false;
		ret = true;
	}
	if(ev->type == SDL_MOUSEMOTION){
		Vec2f p = vec2_t(ev->button.x, ev->button.y) - posAbs;
		if(style == Style::Horizontal){
			barPos = pushBarPos + (p.x - pushPos.x);
			barPos = std::min(std::max(0.0f, barPos), szWindow.x - barLength);
		}
		else{
			barPos = pushBarPos + (p.y - pushPos.y);
			barPos = std::min(std::max(0.0f, barPos), szWindow.y - barLength);
		}
		ret = true;
	}
	return ret;
}

bool ScrollBar::Prepare(int mask){
	Window::Prepare(mask);

	if((mask & Item::Bar) && !IsReady(Item::Bar)){
		GRRenderIf* render = manager->render;

		if(barListId != -1)
			render->ReleaseList(barListId);
		barListId = render->StartList();

		Vec3f v[4];
		float w, h;
		if(style == Style::Horizontal){
			w = barLength;
			h = szWindow.y;
		}
		if(style == Style::Vertical){
			w = szWindow.x;
			h = barLength;
		}
		v[0] = Vec3f(0.5f,     0.5f,     0.0f);
		v[1] = Vec3f(0.5f,     h - 0.5f, 0.0f);
		v[2] = Vec3f(w - 0.5f, h - 0.5f, 0.0f);
		v[3] = Vec3f(w - 0.5f, 0.5f,     0.0f);

		glInterleavedArrays(GL_V3F, sizeof(Vec3f), v);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		render->EndList();
		SetReady(Item::Bar, true);
	}
	return true;
}

void ScrollBar::DrawContents(){
	GRRenderIf* render = manager->render;

	// テキスト色で描画
	SetColor(textColorRGB);
	glPushMatrix();
	if(style == Style::Horizontal)
		glTranslatef(barPos, 0.0f, 0.0f);
	if(style == Style::Vertical)
		glTranslatef(0.0f, barPos, 0.0f);

	render->DrawList(barListId);
	
	glPopMatrix();
}

Vec4f ScrollBar::CalcFrameColor(){
	// 描画しない
	return Vec4f();
}

Vec4f ScrollBar::CalcBackColor(){
	Vec4f c = backColorRGB;

	// 状態に応じて輝度を変化
	float k = 1.0f;
	if(manager->hoverWin == this)
		k = 1.1f;
	if(manager->pushedWin == this)
		k = 1.5f;
	c[0] *= k;
	c[1] *= k;
	c[2] *= k;
	
	return c;
}

}
