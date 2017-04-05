#pragma once

#include <glwin/window.h>

namespace GLWin{;

/* スクロールバー

 */
class ScrollBar : public Window{
public:
	enum{
		Scroll = Window::ScrollBarEventBegin,
	};
	struct Style{
		enum{
			Horizontal,
			Vertical,
		};
	};
	struct Item{
		enum{
			Bar = Window::Item::End,
		};
	};
	
	int	        style;			///< スタイル
	float       barPos;			///< バーの位置
	float       barLength;		///< バーの幅
	bool        barPushed;		///< バーが押されているか
	Vec2f		pushPos;		///< 押されたときのカーソル座標
	float		pushBarPos;		///< 押されたときのバーの位置
	int	        barListId;
	
public:
	void	SetStyle   (int s);
	void	UpdateRange();

	virtual bool  OnEvent       (SDL_Event * ev    );
	virtual bool  Prepare       (int mask);
	virtual void  DrawContents  ();
	virtual Vec4f CalcFrameColor();
	virtual Vec4f CalcBackColor ();
	
	ScrollBar(Window* p);
};

}
