#pragma once

#include <glwin/textbox.h>

namespace GLWin{;

class Console : public Textbox{
public:
	enum{
		TypeEnter = ConsoleEventBegin,
	};
	
	vector<wstring>	history;	//< �R���\�[���ɓ��͂��ꂽ���N�G�X�g�̗���
	int	            histPos;

public:
	virtual bool OnEvent(SDL_Event* ev);

	/// �h���N���X���������鉼�z�֐�
	virtual void Query(const string& req) = 0;
	
	Console(Window* p);
};

}
