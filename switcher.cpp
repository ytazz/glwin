#include <glwin/winmanager.h>
#include <glwin/switcher.h>
#include <glwin/button.h>
#include <glwin/label.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Switcher::Switcher(Window* p):Window(p){
}

void Switcher::Read(XMLNode* node){
	Window::Read(node);
}

bool Switcher::OnEvent(Window* w, int code){
	return false;
}

void Switcher::CalcWindowSize(){
}

void Switcher::Layout(){
}

}
