#pragma once

#include <glwin/outline.h>
#include <glwin/window.h>

namespace GLWin{;

class Button;

/*
 * アイテムのドロップダウンリスト
 */
class ItemList : public Window{
public:
	float	btnHeight;

public:
	void Add(const  string& name);
	void Add(const wstring& name);
	void Del(int idx);
	void Get(int idx,  string& name);
	void Get(int idx, wstring& name);

	virtual void CalcWindowSize();

	ItemList(Window* p);
};

/*
 * リストボックス
 */
class ListBox : public Window, public WindowCallback{
public:
	enum{
		Open = ListBoxEventBegin,
		Close,
		ItemHover,
		SelChange,
	};

	ItemList*		itemList;	//< ドロップダウンリスト
	Button*			button;		//< リスト表示ボタン
	Window*			selItem;	//< 選択アイテム表示ウィンドウ
	int 			selIndex;

public:
	void Clear             ();
	int  NumItems          ();
	void AddItem           (const  string& name);
	void AddItem           (const wstring& name);
	void DelItem           (int idx);
	void Select            (int idx);
	int  GetSelectedIndex  ();
	void GetSelectedItem   ( string& name);
	void GetSelectedItem   (wstring& name);

	virtual bool OnEvent   (Window* w, int code);
	virtual void Layout    ();

	ListBox(Window* p);
};

}
