#include <glwin/winmanager.h>
#include <glwin/hint.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Hint::Hint(Window* p):Window(p){
	SetMargin(5.0f, 5.0f);
	SetContentsAlign(Align::Left | Align::VCenter);
}

bool Hint::Step(float dt){
	if(manager->hoverWin){
		wstring str;
		manager->hoverWin->GetHint(str);
		SetText(str);
	}
	return true;
}

}
