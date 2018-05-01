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
}

void Jog::Read(XMLNode* node){
	Window::Read(node);
}

bool Jog::Init(){
	if(!(btnMinusSmall = (Button*)FindChild("btn_minus_small")) ) return false;
	if(!(btnMinusLarge = (Button*)FindChild("btn_minus_large")) ) return false;
	if(!(btnPlusSmall  = (Button*)FindChild("btn_plus_small" )) ) return false;
	if(!(btnPlusLarge  = (Button*)FindChild("btn_plus_large" )) ) return false;
	
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

/*
bool Jog::OnUpdate(){
	pos = (int)boost::math::round(Module::Get()->controller->GetJogPos(index));
	stringstream ss;
	ss << pos;
	SetText(ss.str());
	return true;
}
*/

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
			Notify(Updated);
			//stringstream ss;
			//ss << "jog " << index << " " << pos;
			//Module::Get()->reqManager->Query(ss.str());
			return true;
		}
	}
	return false;
}

}
