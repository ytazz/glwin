#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Button;
class Label;
class TreeView;

/*
 * �c���[�A�C�e��
 */
class TreeItem : public Window{
public:
	TreeView*   tree;
	Button*		button;		///< �W�J�{�^��
	Label*		label;		///< �A�C�e�����x���v
	int			data;		///< �A�C�e���֘A�f�[�^
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
 * �c���[
 */
class TreeView : public Window, public WindowCallback{
public:
	enum{
		Open = TreeViewEventBegin,
		Close,
		ItemHover,
		SelChange,
	};

	float		itemIndent;		///< �K�w�Ԃ̎�����
	float		itemHeight;		///< �c���[�A�C�e���̍���

	TreeItem*	rootItem;		///< ���[�g�A�C�e��
	TreeItem*   selItem;		///< �I���A�C�e��

	void      LayoutRecurs         (TreeItem* item, bool show, float& y, int depth);
	bool      OnEventRecurs        (TreeItem* item, Window* w, int code);
	void      GetSelectedPathRecurs(TreeItem* item, string& path);

public:
	void      Clear             ();
	TreeItem* AddItem           (const  string& name, TreeItem* par = 0);
	TreeItem* AddItem           (const wstring& name, TreeItem* par = 0);
	void      GetSelectedPath   (string& path);		///< ���[�g����I���A�C�e���܂ł̃p�X

	void      SetItemIndent(float w);
	void      SetItemHeight(float h);

	virtual bool OnEvent         (Window* w, int code);
	virtual void Layout          ();
	virtual void CalcContentsSize();
	
	TreeView(Window* p);
};

}
