#include <glwin/winmanager.h>
#include <glwin/jog.h>
using namespace Scenebuilder;

#include <boost/math/special_functions/round.hpp>

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Jog::Jog(Window* p):Window(p){
	pos       =  0;
	stepSmall =  1;
	stepLarge = 10;

	SetContentsAlign(Align::HCenter | Align::VCenter);

	SetSplit(1, 5);
	SetColRatio(0, 40.0f);
	SetColRatio(1, 40.0f);
	SetColRatio(2, 1.0f);
	SetColRatio(3, 40.0f);
	SetColRatio(4, 40.0f);

	btnMinusSmall = new Button(this);
	btnMinusLarge = new Button(this);
	btnPlusSmall  = new Button(this);
	btnPlusLarge  = new Button(this);

	btnMinusSmall->AssignPanel(0, 1);
	btnMinusLarge->AssignPanel(0, 0);
	btnPlusSmall ->AssignPanel(0, 3);
	btnPlusLarge ->AssignPanel(0, 4);

	SetValue(pos);
}

void Jog::Read(XMLNode* node){
	Window::Read(node);
}

bool Jog::Init(){
	btnMinusSmall->SetFontSize(fontSize);
	btnMinusLarge->SetFontSize(fontSize);
	btnPlusSmall ->SetFontSize(fontSize);
	btnPlusLarge ->SetFontSize(fontSize);

	btnMinusSmall->AddCallback(this);
	btnMinusLarge->AddCallback(this);
	btnPlusSmall ->AddCallback(this);
	btnPlusLarge ->AddCallback(this);

	char str[256];
	sprintf(str, "%+d", -stepSmall); btnMinusSmall->SetText(str);
	sprintf(str, "%+d", -stepLarge); btnMinusLarge->SetText(str);
	sprintf(str, "%+d", +stepSmall); btnPlusSmall ->SetText(str);
	sprintf(str, "%+d", +stepLarge); btnPlusLarge ->SetText(str);

	pos = 0;

	return true;
}

void Jog::SetValue(int val){
	pos = val;
	stringstream ss;
	ss << pos;
	SetText(ss.str());
	Notify(Changed);
}

int Jog::GetValue(){
	return pos;
}

bool Jog::OnEvent(Window* win, int code){
	if(code == Button::Clicked){
		bool matched = false;
		if(win == btnMinusSmall){
			pos -= stepSmall;
			matched = true;
		}
		if(win == btnMinusLarge){
			pos -= stepLarge;
			matched = true;
		}
		if(win == btnPlusSmall){
			pos += stepSmall;
			matched = true;
		}
		if(win == btnPlusLarge){
			pos += stepLarge;
			matched = true;
		}
		if(matched){
			SetValue(pos);
			return true;
		}
	}
	return false;
}

}
