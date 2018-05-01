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
	
	/// �h���N���X���������鉼�z�֐�
	virtual void DrawView    (){};
	
	Viewer(Window* p);
};

}
