#include <glwin/font.h>
#include <glwin/winmanager.h>
#include <glwin/console.h>
using namespace Scenebuilder;

namespace GLWin{;

Console::Console(Window* p):Textbox(p){
	SetHint(L"ここにコマンドを入力します．↑↓キーで履歴が出ます");

	histPos = (int)history.size();
}

bool Console::OnEvent(SDL_Event* ev){
	bool ret = Textbox::OnEvent(ev);

	if(ev->type != SDL_KEYDOWN)
		return ret;
	
	int sym  = ev->key.keysym.sym;
	
	// up
	if(sym == SDLK_UP){
		if(!history.empty() && histPos > 0){
			histPos--;
			text = history[histPos];
			cursorPos = (int)text.size();
			SetReady(Item::Text, false);
			
			ret = true;
		}
	}
	// down
	if(sym == SDLK_DOWN){
		if(!history.empty() && histPos < (int)history.size()-1){
			histPos++;
			text = history[histPos];
			cursorPos = (int)text.size();
			SetReady(Item::Text, false);

			ret = true;
		}
	}
	// enter
	if(sym == SDLK_RETURN || sym == SDLK_KP_ENTER){
		if(!text.empty()){
			history.push_back(text);
			histPos = (int)history.size();

			// リクエスト発行
			string req;
			Converter::ConvertString(req, text);
			Query(req);
		}

		// イベント通知
		Notify(TypeEnter);

		text.clear();
		cursorPos = 0;
		SetReady(Item::Text, false);

		ret = true;
	}

	return ret;
}

}
