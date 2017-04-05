#pragma once

#include <glwin/window.h>

namespace GLWin{;

/* ƒqƒ“ƒg

 */
class Hint : public Window{
public:
	virtual bool Step(float dt);

	Hint(Window* p);
};

}
