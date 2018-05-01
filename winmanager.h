#pragma once

#include <glwin/window.h>
#include <glwin/font.h>
#include <glwin/outline.h>

#undef CreateWindow

namespace GLWin{;

/* OpenGL�ŕ`�悷��E�B���h�E */

/* �E�B���h�E�}�l�[�W��

 */
class WindowManager : public UTRefCount, public WindowCallback{
public:
	GRRenderIf*		render;
	GRDeviceGLIf*	device;
	FontManager*	fontManager;
	OutlineManager*	outlineManager;

	Window*			root;				///< ���[�g�E�B���h�E
	Wins			popups;				///< �|�b�v�A�b�v�E�B���h�E
	
	Window*			hoverWin;			///< ��ɃJ�[�\��������E�B���h�E
	Window*			pushedWin;			///< ��Ń}�E�X�{�^���������ꂽ�E�B���h�E
	Window*			focusedWin;			///< ��Ń}�E�X���N���b�N���ꂽ�E�B���h�E

	int             windowPosX;
	int             windowPosY;
	int				windowWidth;		///< �E�B���h�E��
	int				windowHeight;		///< �E�B���h�E����
	string			windowTitle;		///< �E�B���h�E�^�C�g��

	int             lastFocusTime;

public:
	void Read (XMLNode* node);
	void Close();
	
	/// �J�[�\���̉��ɂ���E�B���h�E���擾
	Window*	GetPointedWindow(Vec2f pos);

	/// �C�x���g�n���h��
	bool	OnEvent(SDL_Event* ev);				//< SDL�C�x���g
	void	OnUpdate();							//< World�̍X�V��

	/// �A�j���[�V��������
	void    Step(float dt);

	/// �`��
	void	Draw();

public:
	/// �h���N���X���������鉼�z�֐�
	virtual Window* CreateWindow(string type, Window* par);
	virtual bool    Init        ();
	
	WindowManager();
	virtual ~WindowManager();
};

}
