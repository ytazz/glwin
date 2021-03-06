#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Viewer : public Window{
public:
	struct Event{
		enum{
			Draw = ViewerEventBegin,
		};
	};

	string   cameraName;
	bool     zUp;

	Camera*  curCamera;

public:
	virtual void Read        (XMLNode* node);
	virtual bool Init        ();
	virtual void DrawContents();
	virtual bool OnEvent     (SDL_Event* ev);
	
	/// 派生クラスが実装する仮想関数
	virtual void DrawView    (){};
	
	Viewer(Window* p);
};

}
