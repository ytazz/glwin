#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Button;

/* メッセージボックス

 */
class Messagebox : public Window{
public:
	Button*	button;

public:
	Messagebox(Window* p);
};

}
