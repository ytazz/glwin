#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Label;
class Button;

/* �܂��݉\�E�B���h�E

 */
class Folder : public Window, public WindowCallback{
public:
	enum{
		Open = FolderEventBegin,
		Close,
	};

	float	barHeight;		//< �o�[�����̍���
	bool	open;			//< �J��

	Label*	label;		//< ���x��
	Button*	button;		//< �܂���/�W�J�{�^��

public:
	Window*	GetChild();		//< �t�H���_�Ɋi�[���ꂽ�q�E�B���h�E���擾

	void    SetOpen       (bool on = true);
	void    SetLabelColor (const string& s);
	void    SetButtonColor(const string& s);
	
	virtual void Read          (XMLNode* node);
	virtual void SetText       (const wstring& t);
	virtual bool OnEvent       (Window* w, int code);
	virtual void CalcWindowSize();
	virtual void Layout        ();
	
	Folder(Window* p);
};

}
