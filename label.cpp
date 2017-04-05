#include <glwin/winmanager.h>
#include <glwin/label.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Label::Label(Window* p):Window(p){
	// ƒtƒŒ[ƒ€‚È‚µ
	SetFrameColor("none");

	SetMargin(5.0f, 5.0f);
}

}
