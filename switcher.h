#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Label;
class Button;

/* 子ウィンドウを切り替え表示

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
