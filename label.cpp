#include <glwin/winmanager.h>
#include <glwin/label.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Label::Label(Window* p):Window(p){
	// �t���[���Ȃ�
	SetFrameColor("none");

	SetMargin(5.0f, 5.0f);
}

}
