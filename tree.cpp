#include <glwin/winmanager.h>
#include <glwin/tree.h>
#include <glwin/button.h>
#include <glwin/label.h>
#include <glwin/scrollbar.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

TreeItem::TreeItem(Window* p, TreeItem* pi):Window(p){
	tree     = (TreeView*)parent;
	button   = new Button(this);
	label    = new Label (this);
	data     = 0;
	expanded = false;
	parItem  = pi;

	if(parItem)
		parItem->childItems.push_back(this);

	// 親のTreeViewを展開ボタンとアイテムラベルにコールバック登録
	button->AddCallback(tree);
	label ->AddCallback(tree);
}

void TreeItem::Expand(bool on){
	expanded = on;
}

bool TreeItem::IsExpanded(){
	return expanded;
}

void TreeItem::CalcWindowSize(){
	label->CalcContentsSize();
	szWindow.x = tree->itemHeight + label->szContents.x;
	szWindow.y = tree->itemHeight;
}

void TreeItem::Layout(){
	float w = szWindow.x;
	float h = tree->itemHeight;
	button->SetSize    (h, h);
	button->SetPosition(0.0f, 0.0f);
	label ->SetSize    (w-h, h);
	label ->SetPosition(h, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TreeView::TreeView(Window* p):Window(p){
	itemHeight = 20.0f;
	itemIndent =  5.0f;
	rootItem   = 0;
	selItem    = 0;

	AddCallback(manager);
	Clear();
}

void TreeView::Clear(){
	Window::Clear();
	rootItem = new TreeItem(this, 0);
	// ルートアイテムは表示しない
	rootItem->Show(false);
}

TreeItem* TreeView::AddItem(const string& name, TreeItem* item){
	if(!item)
		item = rootItem;
	TreeItem* childItem = new TreeItem(this, item);
	childItem->label->SetText(name);
	childItem->SetAlign(Align::Absolute);
	return childItem;
}

TreeItem* TreeView::AddItem(const wstring& name, TreeItem* item){
	if(!item)
		item = rootItem;
	TreeItem* childItem = new TreeItem(this, item);
	childItem->label->SetText(name);
	childItem->SetAlign(Align::Absolute);
	return childItem;
}

void TreeView::GetSelectedPathRecurs(TreeItem* item, string& path){
	string name;
	item->label->GetText(name);
	if(path.empty())
		 path = name;
	else path = name + '/' + path;
	if(item->parItem)
		GetSelectedPathRecurs(item->parItem, path);
}

void TreeView::GetSelectedPath(string& path){
	if(!selItem)
		return;
	GetSelectedPathRecurs(selItem, path);
}

void TreeView::SetItemIndent(float w){
	itemIndent = w;
}

void TreeView::SetItemHeight(float h){
	itemHeight = h;
}

bool TreeView::OnEventRecurs(TreeItem* item, Window* w, int code){
	bool ret = false;

	// ツリーアイテムの展開
	if(w == item->button && code == Button::Clicked){
		if(item->IsExpanded()){
			item->Expand(false);
			Notify(TreeView::Close);
		}
		else{
			item->Expand(true);
			Notify(TreeView::Open);
		}
		// 再レイアウト
		SetReady(Item::Layout, false);
		
		ret = true;
	}
	// ツリーアイテムの選択
	if(w == item->label && code == Window::FocusIn){
		bool changed = (selItem != item);
		selItem = item;
		if(changed)
			Notify(TreeView::SelChange);

		ret = true;
	}
	
	if(item->IsExpanded()){
		for(int i = 0; i < (int)item->childItems.size(); i++)
			ret |= OnEventRecurs(item->childItems[i], w, code);
	}

	return ret;
}

bool TreeView::OnEvent(Window* w, int code){
	bool ret = false;
	
	if(code == Window::Destroy)
		return ret;

	for(int i = 0; i < (int)rootItem->childItems.size(); i++)
		ret |= OnEventRecurs(rootItem->childItems[i], w, code);

	return ret;
}

void TreeView::LayoutRecurs(TreeItem* item, bool show, float& y, int depth){
	if(show){
		item->SetPosition(itemIndent * depth, y);
		item->CalcWindowSize();
		Vec2f sz = item->GetSize();
		sz.y = itemHeight;
		item->SetSize(sz.x, sz.y);
		y += itemHeight;
		item->Show(true);
	}
	else{
		item->Show(false);
	}

	for(int i = 0; i < (int)item->childItems.size(); i++)
		LayoutRecurs(item->childItems[i], (show && item->IsExpanded()), y, depth+1);
}

void TreeView::Layout(){
	// ツリーアイテムの配置
	float y = 0.0f;
	for(int i = 0; i < (int)rootItem->childItems.size(); i++)
		LayoutRecurs(rootItem->childItems[i], true, y, 0);
	// スクロールバーの配置
	Window::Layout();
}

void TreeView::CalcContentsSize(){
	// 正しくスクロールするようにツリーアイテムの範囲を計算
	szContents.clear();
	for(uint i = 0; i < children.size(); i++){
		Window* c = children[i];
		if(!c->shown || c == scrollBar[0] || c == scrollBar[1])
			continue;
		szContents.x = std::max(szContents.x, c->pos.x + c->szWindow.x);
		szContents.y = std::max(szContents.y, c->pos.y + c->szWindow.y);
	}
}

}
