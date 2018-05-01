#pragma once

#include <glwin/window.h>

namespace GLWin{;

class OutlineSet;
class Outline;
class RadioGroup;

/* �{�^��

 */
class Button : public Window{
public:
	struct Item{
		enum{
			Style = Window::Item::End,
		};
	};
	struct Style{
		enum{
			Normal,		//< ��`, �e�L�X�g�\��
			Lamp  ,		//< �ۂ������v
			Opener,		//< ���X�g�{�b�N�X��t�H���_�̊J�{�^��
		};
	};
	enum{
		Clicked = Window::ButtonEventBegin,
		LatchOn ,
		LatchOff,
	};

	int		style;		///< �`��X�^�C��
	bool	pushed;		///< �}�E�X�ɂ���ĉ����������Ă�����
	bool	latch;		///< ���b�`�i�����ƃI���C������x�����ƃI�t�j
	bool	onoff;		///< �I���I�t���
	
	OutlineSet*			outlineSet;
	vector<Outline*>	outline;
	float				markerSize;
	
	RadioGroup*			group;

public:
	/// �`��X�^�C���ݒ�
	void	SetStyle(int s);
	/// �}�[�J�T�C�Y�̐ݒ�
	void	SetMarkerSize(float w);
	/// ���b�`�ݒ�
	void	SetLatch(bool on = true);
	/// �I���I�t��Ԑݒ�i���b�`���j
	void	SetOn(bool on = true);
	/// �v�b�V��
	void	Push();
	
	virtual void  Read            (XMLNode* node);
	virtual bool  OnEvent         (SDL_Event* ev);
	virtual bool  Prepare         (int mask);
	virtual Vec4f CalcBackColor   ();
	virtual Vec4f CalcFrameColor  ();
	virtual float CalcFrameWidth  ();
	virtual void  CalcContentsSize();
	virtual void  DrawContents    ();
	
	Button(Window* p);
};

}
