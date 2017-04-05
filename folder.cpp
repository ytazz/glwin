#include <glwin/winmanager.h>
#include <glwin/folder.h>
#include <glwin/button.h>
#include <glwin/label.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Folder::Folder(Window* p):Window(p){
	barHeight = 30.0f;

	label  = new Label (this);
	label->SetAlign        (Align::Absolute);
	label->SetContentsAlign(Align::Left | Align::VCenter);

	button = new Button(this);
	button->SetAlign(Align::Absolute);
	button->SetStyle(Button::Style::Opener);
	button->AddCallback(this);

	SetOpen(true);

	AddCallback(manager);
}

Window* Folder::GetChild(){
	return (children.size() >= 3 ? children[2] : 0);
}

void Folder::SetOpen(bool on){
	open = on;
	Window* child = GetChild();
	if(child)
		child->Show(on);
	Notify(on ? Folder::Open : Folder::Close);
	button->SetOn(on);
		
	// 親ウィンドウに再レイアウト
	parent->Layout();
}

void Folder::SetLabelColor(const string& s){
	label ->SetBackColor(s);
}

void Folder::SetButtonColor(const string& s){
	button->SetBackColor(s);
}

void Folder::Read(XMLNode* node){
	Window::Read(node);

	bool   b;
	string s;
	if(node->Get(s, ".label_color" )) SetLabelColor (s);
	if(node->Get(s, ".button_color")) SetButtonColor(s);
	if(node->Get(b, ".open"        )) SetOpen       (b);
}

void Folder::SetText(const wstring& t){
	// ラベルにリダイレクト
	label->SetText(t);
}

bool Folder::OnEvent(Window* w, int code){
	// ラベルとボタン以外の子ウィンドウ
	Window* child = GetChild();
	
	// 子ウィンドウ表示切替
	if(w == button && code == Button::Clicked){
		SetOpen(!open);
		return true;
	}

	return false;
}

void Folder::CalcWindowSize(){
	// 高さのみ指定
	szWindow.y = barHeight;

	Window* child = GetChild();
	if(child && open){
		child->CalcWindowSize();
		szWindow.y += child->szWindow.y;
	}
}

void Folder::Layout(){
	float w = szWindow.x;
	float h = szWindow.y;
	Rect rc;

	label ->SetPosition(0.0f         , 0.0f     );
	button->SetPosition(w - barHeight, 0.0f     );
	label ->SetSize    (w - barHeight, barHeight);
	button->SetSize    (barHeight    , barHeight);
	button->SetMarkerSize(0.4f * barHeight);

	Window* child = GetChild();
	if(child){
		child->SetPosition(0.0f, barHeight    );
		child->SetSize    (w   , h - barHeight);
	}
}

}
