#pragma once

#include <glwin/window.h>

namespace GLWin{;

/* �X�N���[���o�[

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
	
	int	        style;			///< �X�^�C��
	float       barPos;			///< �o�[�̈ʒu
	float       barLength;		///< �o�[�̕�
	bool        barPushed;		///< �o�[��������Ă��邩
	Vec2f		pushPos;		///< �����ꂽ�Ƃ��̃J�[�\�����W
	float		pushBarPos;		///< �����ꂽ�Ƃ��̃o�[�̈ʒu
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
