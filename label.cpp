#include <glwin/winmanager.h>
#include <glwin/label.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Label::Label(Window* p):Window(p){
	// フレームなし
	SetFrameColor("none");

	SetMargin(5.0f, 5.0f);
}

}
