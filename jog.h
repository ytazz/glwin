#pragma once

#include <glwin/window.h>
#include <glwin/button.h>

namespace GLWin{;

class Jog : public Window, public WindowCallback{
public:
	enum{
		Updated = Window::JogEventBegin,
	};

	int     pos;
	int     stepSmall;
	int     stepLarge;

	Button* btnMinusSmall;
	Button* btnMinusLarge;
	Button* btnPlusSmall ;
	Button* btnPlusLarge ;
	
public:
	virtual void Read    (XMLNode* node);
	virtual bool Init    ();
	//virtual bool OnUpdate();
	virtual bool OnEvent (Window* win, int code);
	
	Jog(Window* p);
};

}
