#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Textbox : public Window{
public:
	enum{
		TypeAscii = TextboxEventBegin,
	};
	
	int	cursorPos;
	int	maxLength;

public:
	void SetMaxLength(int len);

	virtual void  Read         (XMLNode* node);
	virtual bool  OnEvent      (SDL_Event* ev);
	virtual Vec4f CalcBackColor();
	virtual void  DrawContents ();
	
	Textbox(Window* p);
};

}
