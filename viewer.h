#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Viewer : public Window{
public:
	
public:
	virtual void DrawContents();
	
	/// 派生クラスが実装する仮想関数
	virtual void DrawView    () = 0;
	
	Viewer(Window* p);
};

}
