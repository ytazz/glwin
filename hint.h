#pragma once

#include <glwin/window.h>

namespace GLWin{;

/* �q���g

 */
class Hint : public Window{
public:
	virtual bool Step(float dt);

	Hint(Window* p);
};

}
