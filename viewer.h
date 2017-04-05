#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Label;
class ListBox;

class Viewer : public Window{
public:
	
public:
	virtual void DrawContents();
	
	/// ”h¶ƒNƒ‰ƒX‚ªÀ‘•‚·‚é‰¼‘zŠÖ”
	virtual void DrawView    () = 0;
	
	Viewer(Window* p);
};

}
