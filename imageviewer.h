#pragma once

#include <glwin/viewer.h>

namespace GLWin{;

class ImageViewer : public Viewer{
public:
	
public:
	/// 派生クラスが実装する仮想関数
	virtual void DrawView();
	
	ImageViewer(Window* p);
};

}
