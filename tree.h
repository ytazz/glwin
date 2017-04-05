#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Button;
class Label;
class TreeView;

/*
 * ツリーアイテム
 */
class TreeItem : public Window{
public:
	TreeView*   tree;
	Button*		button;		///< 展開ボタン
	Label*		label;		///< アイテムラベル」
	int			data;		///< アイテム関連データ
	bool        expanded;

	TreeItem*	        parItem;
	vector<TreeItem*>   childItems;
	
public:
	void	Expand    (bool on);
	bool	IsExpanded();

	virtual void CalcWindowSize();
	virtual void Layout        ();

	TreeItem(Window* p, TreeItem* pi);
};

/*
 * ツリー
 */
class TreeView : public Window, public WindowCallback{
public:
	enum{
		Open = TreeViewEventBegin,
		Close,
		ItemHover,
		SelChange,
	};

	float		itemIndent;		///< 階層間の字下げ
	float		itemHeight;		///< ツリーアイテムの高さ

	TreeItem*	rootItem;		///< ルートアイテム
	TreeItem*   selItem;		///< 選択アイテム

	void      LayoutRecurs         (TreeItem* item, bool show, float& y, int depth);
	bool      OnEventRecurs        (TreeItem* item, Window* w, int code);
	void      GetSelectedPathRecurs(TreeItem* item, string& path);

public:
	void      Clear             ();
	TreeItem* AddItem           (const  string& name, TreeItem* par = 0);
	TreeItem* AddItem           (const wstring& name, TreeItem* par = 0);
	void      GetSelectedPath   (string& path);		///< ルートから選択アイテムまでのパス

	void      SetItemIndent(float w);
	void      SetItemHeight(float h);

	virtual bool OnEvent         (Window* w, int code);
	virtual void Layout          ();
	virtual void CalcContentsSize();
	
	TreeView(Window* p);
};

}
