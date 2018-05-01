#include <glwin/outline.h>
#include <glwin/winmanager.h>
#include <glwin/button.h>

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Button::Button(Window* p):Window(p){
	group = 0;
	
	SetOn        (false);
	SetMargin    (10.0f, 5.0f);
	SetStyle     (Style::Normal);
	SetLatch     (false);
	SetMarkerSize(10.0f);
}

void Button::Read(XMLNode* node){
	Window::Read(node);

	bool b;
	if(node->Get(b, ".latch")) SetLatch(b);
}

void Button::SetStyle(int s){
	style = s;
	SetReady(Item::Style, false);
}

void Button::SetMarkerSize(float w){
	markerSize = w;
	SetReady(Item::Style, false);
}

void Button::SetLatch(bool on){
	latch = on;
}

void Button::SetOn(bool on){
	if(group && on)
		group->Switch(this);
	onoff = on;
}
	
void Button::Push(){
	if(group){
		group->Switch(this);
	}
	else if(latch){
		onoff = !onoff;
		if( onoff) Notify(LatchOn );
		if(!onoff) Notify(LatchOff);
	}
	Notify(Clicked);
}

bool Button::OnEvent(SDL_Event* ev){
	if(ev->type == SDL_MOUSEBUTTONUP){
		if(manager->pushedWin == this){
			Push();
			return true;
		}
	}
	return false;
}

void Button::CalcContentsSize(){
	if(style == Style::Opener || style == Style::Lamp){
		szContents.x = markerSize;
		szContents.y = markerSize;
	}
	else Window::CalcContentsSize();
}

bool Button::Prepare(int mask){
	Window::Prepare(mask);

	if((mask && Item::Style) && !IsReady(Item::Style)){
		if(style == Style::Opener){
			outlineSet = manager->outlineManager->Find("button_opener");
			if(outlineSet){
				outline.resize(2);
				outline[0] = outlineSet->Find("open" );
				outline[1] = outlineSet->Find("close");
			}
			else outline.clear();
		}
		if(style == Style::Lamp){
			outlineSet = manager->outlineManager->Find("button_lamp");
			if(outlineSet){
				outline.resize(1);
				outline[0] = outlineSet->Find("lamp");
			}
			else outline.clear();
		}
		SetReady(Item::Style, true);
	}
	return true;
}

Vec4f Button::CalcBackColor(){
	Vec4f c = backColorRGB;
	if(style == Style::Normal || style == Style::Opener){
		// ó‘Ô‚É‰ž‚¶‚Ä‹P“x‚ð•Ï‰»
		float k = 1.0f;
		if(manager->hoverWin == this)
			 k = 1.1f;
		if(manager->pushedWin == this || onoff)
			 k = 1.5f;
		c[0] *= k;
		c[1] *= k;
		c[2] *= k;
	}
	if(style == Style::Lamp){
		// ”wŒi•`‰æ‚µ‚È‚¢
		c[3] = 0.0f;
	}
	return c;
}

Vec4f Button::CalcFrameColor(){
	Vec4f c = frameColorRGB;
	if(style == Style::Normal || style == Style::Opener){

	}
	if(style == Style::Lamp){
		// ”wŒi•`‰æ‚µ‚È‚¢
		c.w = 0.0f;
	}
	return c;
}

float Button::CalcFrameWidth(){
	if(style == Style::Normal || style == Style::Opener){
		return (manager->pushedWin == this ? 2.0f : 1.0f);
	}
	if(style == Style::Lamp){
		return 0.0f;
	}
	return 1.0f;
}

void Button::DrawContents(){
	GRRenderIf* render = manager->render;

	if(style == Style::Normal){
		Window::DrawContents();
	}
	if(style == Style::Opener || style == Style::Lamp){
		Vec4f c = CalcFrameColor();
		SetColor(c);
		render->SetLineWidth(CalcFrameWidth());
		
		glPushMatrix();
		glScalef(markerSize, markerSize, 1.0f);

		if(style == Style::Opener){
			if(outline.size() == 2)
				outline[onoff ? 1 : 0]->Draw(render);
		}
		if(style == Style::Lamp){
			if(outline.size() == 1)
				outline[0]->Draw(render);
		}
		
		glPopMatrix();
	}
}

}
