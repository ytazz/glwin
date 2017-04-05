#pragma once

#include <glwin/textbox.h>

namespace GLWin{;

class Console : public Textbox{
public:
	enum{
		TypeEnter = ConsoleEventBegin,
	};
	
	vector<wstring>	history;	//< コンソールに入力されたリクエストの履歴
	int	            histPos;

public:
	virtual bool OnEvent(SDL_Event* ev);

	/// 派生クラスが実装する仮想関数
	virtual void Query(const string& req) = 0;
	
	Console(Window* p);
};

}
