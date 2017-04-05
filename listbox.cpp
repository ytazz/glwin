#include <glwin/rect.h>
#include <glwin/font.h>
#include <glwin/winmanager.h>
#include <glwin/listbox.h>
#include <glwin/button.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

ItemList::ItemList(Window* p):Window(p){
	btnHeight = 20.0f;
	margin.x  =  0.0f;
}

void ItemList::Add(const string& name){
	wstring ws;
	Converter::ConvertString(ws, name);
	Add(ws);
}

void ItemList::Add(const wstring& name){
	uint n = children.size();
	SetSplit(n+1, 1);
	Button* btn = new Button(this);
	btn->SetMargin(5.0f, 0.0f);
	btn->SetAlign (Align::HFit | Align::VFit);
	btn->SetText  (name);
	// リストボックスをコールバック登録
	btn->AddCallback((ListBox*)parent);

	parent->SetReady(Item::Layout, false);
}

void ItemList::Del(int idx){
	children.erase(children.begin() + idx);
}

void ItemList::Get(int idx, string& name){
	Converter::ConvertString(name, children[idx]->text);
}

void ItemList::Get(int idx, wstring& name){
	name = children[idx]->text;
}

void ItemList::CalcWindowSize(){
	float wmax = 100.0f;
	for(uint i = 0; i < children.size(); i++){
		Window* c = children[i];
		c->CalcContentsSize();
		wmax = std::max(wmax, c->szContents.x + 2.0f * c->margin.x);
	}

	szWindow.x = wmax;
	szWindow.y = btnHeight * children.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ListBox::ListBox(Window* p):Window(p){
	focusable = false;

	selItem  = new Window  (this);
	button   = new Button  (this);
	itemList = new ItemList(this);

	// 手動でレイアウトするのでAbsoluteに設定
	selItem ->SetAlign (Align::Absolute);
	button  ->SetAlign (Align::Absolute);
	itemList->SetAlign (Align::Absolute);
	
	itemList->SetPopup (true );
	itemList->Show     (false);

	button->SetStyle(Button::Style::Opener);
	button->SetOn   (false);
	button->AddCallback(this);
	
	AddCallback(manager);

	Clear();
}

void ListBox::Clear(){
	itemList->Clear();
	Select(-1);
}

void ListBox::AddItem(const string& name){
	itemList->Add(name);
}

void ListBox::AddItem(const wstring& name){
	itemList->Add(name);
}

void ListBox::DelItem(int idx){
	if(0 <= idx && idx < NumItems())
		itemList->Del(idx);
	Select(-1);
}

int ListBox::NumItems(){
	return (int)itemList->children.size();
}

void ListBox::Select(int idx){
	if(0 <= idx && idx < NumItems()){
		selIndex = idx;
		wstring str;
		GetSelectedItem(str);
		selItem->SetText(str);
	}
	else{
		selIndex = -1;
		selItem->SetText("---");
	}
}

int ListBox::GetSelectedIndex(){
	return selIndex;
}

void ListBox::GetSelectedItem(string& name){
	if(0 <= selIndex && selIndex < NumItems())
		itemList->Get(selIndex, name);
}

void ListBox::GetSelectedItem(wstring& name){
	if(0 <= selIndex && selIndex < NumItems())
		itemList->Get(selIndex, name);
}

bool ListBox::OnEvent(Window* w, int code){
	bool ret = false;

	// ドロップダウンリスト表示
	if(w == button && code == Button::Clicked){
		if(NumItems() > 0){
			bool show = !itemList->shown;
			itemList->Show(show);
			button->SetOn(show);
			if(show)
				 Notify(Open );
			else Notify(Close);

			ret = true;
		}
	}
	// ドロップダウンリスト非表示
	if(w == itemList && code == Window::FocusOut){
		itemList->Show(false);
		Notify(Close);

		ret = true;
	}
	// アイテム選択
	for(uint i = 0; i < itemList->children.size(); i++){
		if(w == itemList->children[i]){
			if(code == Button::Clicked){
				Select(i);
				itemList->Show(false);
				Notify(SelChange);
				Notify(Close);

				ret = true;
			}
			if(code == Button::HoverIn){
				Notify(ItemHover);

				ret = true;
			}
		}
	}
	return ret;
}

void ListBox::Layout(){
	float w  = szWindow.x;
	float h  = szWindow.y;
	
	// 選択アイテムは左, ボタンは右
	float wb = h;
	selItem->SetPosition(0.0f  , 0.0f);
	button ->SetPosition(w - wb, 0.0f);
	selItem->SetSize    (w - wb, h   );
	button ->SetSize    (wb    , h   );
	button ->SetMarkerSize(0.4f * wb);

	// リストは選択アイテムの上か下，表示スペースのある方
	Vec2f szRoot = manager->root->GetSize();
	itemList->CalcWindowSize();
	Vec2f sz = itemList->GetSize();
	if(posAbs.y + szWindow.y + sz.y < szRoot.y)
		 itemList->SetPosition(0, h    );
	else itemList->SetPosition(0, -sz.y);

	Window::Layout();
}

}
