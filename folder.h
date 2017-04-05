#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Label;
class Button;

/* 折り畳み可能ウィンドウ

 */
class Folder : public Window, public WindowCallback{
public:
	enum{
		Open = FolderEventBegin,
		Close,
	};

	float	barHeight;		//< バー部分の高さ
	bool	open;			//< 開閉

	Label*	label;		//< ラベル
	Button*	button;		//< 折り畳み/展開ボタン

public:
	Window*	GetChild();		//< フォルダに格納された子ウィンドウを取得

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
