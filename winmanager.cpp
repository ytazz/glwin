#include <glwin/winmanager.h>
#include <glwin/window.h>
#include <glwin/button.h>
#include <glwin/listbox.h>
#include <glwin/folder.h>
#include <glwin/hint.h>
#include <glwin/label.h>
#include <glwin/switcher.h>
#include <glwin/textbox.h>
#include <glwin/tree.h>
#include <glwin/jog.h>
#include <glwin/viewer.h>

#include <sbmessage.h>
using namespace Scenebuilder;

#include <functional>
using namespace std::placeholders;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

WindowManager::WindowManager(){
	windowPosX   = -1;
	windowPosY   = -1;
	windowWidth  = 1024;
	windowHeight = 768;
	windowTitle  = "no title";
	
	root       = 0;
	pushedWin  = 0;
	focusedWin = 0;
	hoverWin   = 0;

	fontManager    = new FontManager   ();
	outlineManager = new OutlineManager();

	lastFocusTime = 0;
}

WindowManager::~WindowManager(){
	delete fontManager;
	delete outlineManager;
}

void WindowManager::Read(XMLNode* node){
	outlineManager->Read(node);

	XMLNode* rootWinNode = 0;
	if(node){
		node->Get(windowPosX  , ".window_pos_x" );
		node->Get(windowPosY  , ".window_pos_y" );
		node->Get(windowWidth , ".window_width" );
		node->Get(windowHeight, ".window_height");
		node->Get(windowTitle , ".window_title" );

		rootWinNode = node->GetNode(0, false);
	}
	
	root = new Window(0);
	root->manager = this;
	if(rootWinNode)
		root->Read(rootWinNode);
}

bool WindowManager::Init(){
	// �t�H���g������
	if(fontManager->Init())
		 Message::Out("font initialized");
	else Message::Error("font initialization failed");

	// �A�E�g���C��������
	if(outlineManager->Init())
		 Message::Out("outline initialized");
	else Message::Out("outline initialization failed");
	
	// ���C�A�E�g
	root->SetSize((float)windowWidth, (float)windowHeight);

	return true;
}

void WindowManager::Close(){
	fontManager->Close();
}

Window*	WindowManager::GetPointedWindow(Vec2f pos){
	// �}�E�X���W���܂ރE�B���h�E�̒���Z���ŏ��i��Ԏ�O�j�̂��̂�T��
	float inf = numeric_limits<float>::max();
	int   dmax;
	Window* wmin = 0;
	for(uint i = 0; i < popups.size(); i++){
		dmax = 0;
		popups[i]->Recurs(std::bind(&Window::HitTest, _1, pos, &wmin, &dmax), Window::Group::Shown);
		if(wmin)
			return wmin;
	}
	dmax = 0;
	root->Recurs(std::bind(&Window::HitTest, _1, pos, &wmin, &dmax), Window::Group::Shown | Window::Group::NonPopup);
	return wmin;
}

bool WindowManager::OnEvent(SDL_Event* ev){
	bool ret = false;

	if(ev->type == SDL_WINDOWEVENT){
		if(ev->window.event == SDL_WINDOWEVENT_FOCUS_LOST){
			//Message::Out("glwin window manager: focus lost: timestamp %d", ev->window.timestamp);
		}
		if(ev->window.event == SDL_WINDOWEVENT_FOCUS_GAINED){
			lastFocusTime = ev->window.timestamp;
			//Message::Out("glwin window manager: focus gained: timestamp %d", ev->window.timestamp);
		}
	}
	if(ev->type == SDL_TEXTINPUT){
		if(focusedWin)
			ret = focusedWin->OnEvent(ev);
	}
	if(ev->type == SDL_KEYDOWN){
		if(focusedWin)
			ret = focusedWin->OnEvent(ev);
	}
	if(ev->type == SDL_MOUSEBUTTONDOWN){
		//Message::Out("glwin window manager: mouse down: timestamp %d", ev->button.timestamp);
		// SDL�̎d�l�i�o�O�H�j�Ńt�H�[�J�X�Ɠ����Ƀ}�E�X�_�E���C�x���g�����邪�C����͖���
		if(ev->button.timestamp == lastFocusTime)
			return false;

		vec2_t pos(ev->button.x, ev->button.y);
		Window* win = GetPointedWindow(pos);
		if(win){
			ret = win->OnEvent(ev);
			pushedWin = win;

			// �t�H�[�J�X���ڂ�
			if(focusedWin != win){
				if(focusedWin)
					focusedWin->Notify(Window::FocusOut);
				if(win && win->focusable){
					win->Notify(Window::FocusIn);
					focusedWin = win;	
				}
				else focusedWin = 0;
			}
		}
	}
	if(ev->type == SDL_MOUSEBUTTONUP){
		vec2_t pos(ev->button.x, ev->button.y);
		Window* win = GetPointedWindow(pos);
		// �v�b�V�����̃E�B���h�E�̃n���h�����Ă�
		if(pushedWin)
			ret = pushedWin->OnEvent(ev);

		pushedWin = 0;
	}
	if(ev->type == SDL_MOUSEMOTION){
		vec2_t pos(ev->motion.x, ev->motion.y);
		// �v�b�V����ԂȂ�v�b�V�����ꂽ�E�B���h�E�̃n���h�����Ă�
		if(pushedWin){
			ret = pushedWin->OnEvent(ev);
		}
		// ����ȊO�̓z�o�[��Ԃ��X�V���ăz�o�[�E�B���h�E�̃n���h�����Ă�
		else{
			Window* win = GetPointedWindow(pos);
			if(win != hoverWin){
				if(hoverWin)
					hoverWin->Notify(Window::HoverOut);
				if(win)
					win->Notify(Window::HoverIn);
			}
			hoverWin = win;
			if(hoverWin)
				hoverWin->OnEvent(ev);
		}
	}
	if(ev->type == SDL_MOUSEWHEEL){
		// �z�o�[�E�B���h�E�̃n���h�����Ă�
		if(hoverWin){
			hoverWin->OnEvent(ev);
		}
	}

	return ret;
}

void WindowManager::OnUpdate(){
	root->Recurs(std::bind(&Window::OnUpdate, _1), Window::Group::All);
}

void WindowManager::Step(float dt){
	root->Recurs(std::bind(&Window::Step, _1, dt), Window::Group::All);
}

void WindowManager::Draw(){
	//  - �r���[���E�B���h�E���d�Ȃ�P�[�X���l����ƃr���[�����ƂɃf�v�X�o�b�t�@���N���A����K�v�����邪�C���܂̂Ƃ���ۗ�
	////  - �o�b�t�@�N���A���̓V�U�[�e�X�g�𖳌���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);

	render->SetLighting   (false);
	render->SetDepthTest  (false);
	render->SetModelMatrix(Affinef());
	render->SetViewMatrix (Affinef());
	
	// ��\���E�B���h�E���T�C�Y�v�Z���͕K�v�Ȃ���Prepare���Ă�
	root->Recurs(std::bind(&Window::Prepare, _1, Window::Item::All), Window::Group::All);
	
	// �o�b�t�@�N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	render->BeginScene();

	//
	float w = (float)windowWidth;
	float h = (float)windowHeight;
	Affinef affProj;
	affProj.item(0,0) =  2.0f / w;
	affProj.item(1,1) = -2.0f / h;
	affProj.item(2,2) = -1.0f;
	affProj.item(3,3) =  1.0f;
	affProj.item(0,3) = -1.0f;
	affProj.item(1,3) =  1.0f;
	render->SetViewport(Vec2f(), Vec2f(w, h));
	render->SetProjectionMatrix(affProj);
		
	// ��|�b�v�A�b�v�E�B���h�E�`��
	root->Draw();
	
	// �|�b�v�A�b�v�E�B���h�E�`��
	for(uint i = 0; i < popups.size(); i++){
		Window* pop = popups[i];
		// ���g���e����\���Ȃ�`�悵�Ȃ�
		if(!pop->IsShown())
			continue;
		glPushMatrix();
		glTranslatef(pop->posAbs.x, pop->posAbs.y, 0.0f);
		pop->Draw();
		glPopMatrix();
	}
	
	render->EndScene();
}

Window* WindowManager::CreateWindow(string type, Window* par){
	// type���w��̓R���e�i�E�B���h�E
	if(type == "window"  ) return new Window  (par);
	if(type == "button"  ) return new Button  (par);
	if(type == "folder"  ) return new Folder  (par);
	if(type == "hint"    ) return new Hint    (par);
	if(type == "label"   ) return new Label   (par);
	if(type == "listbox" ) return new ListBox (par);
	if(type == "switcher") return new Switcher(par);
	if(type == "textbox" ) return new Textbox (par);
	if(type == "tree"    ) return new TreeView(par);
	if(type == "jog"     ) return new Jog     (par);
	if(type == "viewer"  ) return new Viewer  (par);

	return 0;
}

}
