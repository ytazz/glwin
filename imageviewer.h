#pragma once

#include <glwin/viewer.h>

namespace GLWin{;

class ImageViewer : public Viewer{
public:
	
public:
	/// �h���N���X���������鉼�z�֐�
	virtual void DrawView();
	
	ImageViewer(Window* p);
};

}
