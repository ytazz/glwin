#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Label;
class ListBox;

class Viewer : public Window{
public:
	
public:
	virtual void DrawContents();
	
	/// �h���N���X���������鉼�z�֐�
	virtual void DrawView    () = 0;
	
	Viewer(Window* p);
};

}
