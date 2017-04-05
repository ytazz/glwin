#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Label;
class Button;

/* �q�E�B���h�E��؂�ւ��\��

 */
class Switcher : public Window, public WindowCallback{
public:
	enum{
		SelChange = SwitcherEventBegin,
	};

public:
	virtual void Read          (XMLNode* node);
	virtual bool OnEvent       (Window* w, int code);
	virtual void CalcWindowSize();
	virtual void Layout        ();
	
	Switcher(Window* p);
};

}
