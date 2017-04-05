#pragma once

#include <glwin/typedefs.h>

#include <set>

namespace GLWin{;

class Window;
class Button;

/*
	ラジオボタングループ
 */
class RadioGroup : public UTRefCount{
public:
	Window*         owner;
	set<Button*>	buttons;
	
public:
	void    Read  (XMLNode* node);
	void	Add   (Button* b);
	void	Remove(Button* b);
	void    Switch(Button* b);

	RadioGroup(Window* o);
};

typedef vector< UTRef<RadioGroup> > RadioGroupRefs;

}
