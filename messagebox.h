#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Button;

/* ���b�Z�[�W�{�b�N�X

 */
class Messagebox : public Window{
public:
	Button*	button;

public:
	Messagebox(Window* p);
};

}
