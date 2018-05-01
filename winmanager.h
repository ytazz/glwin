#pragma once

#include <glwin/window.h>
#include <glwin/font.h>
#include <glwin/outline.h>

#undef CreateWindow

namespace GLWin{;

/* OpenGLで描画するウィンドウ */

/* ウィンドウマネージャ

 */
class WindowManager : public UTRefCount, public WindowCallback{
public:
	GRRenderIf*		render;
	GRDeviceGLIf*	device;
	FontManager*	fontManager;
	OutlineManager*	outlineManager;

	Window*			root;				///< ルートウィンドウ
	Wins			popups;				///< ポップアップウィンドウ
	
	Window*			hoverWin;			///< 上にカーソルがあるウィンドウ
	Window*			pushedWin;			///< 上でマウスボタンが押されたウィンドウ
	Window*			focusedWin;			///< 上でマウスがクリックされたウィンドウ

	int             windowPosX;
	int             windowPosY;
	int				windowWidth;		///< ウィンドウ幅
	int				windowHeight;		///< ウィンドウ高さ
	string			windowTitle;		///< ウィンドウタイトル

	int             lastFocusTime;

public:
	void Read (XMLNode* node);
	void Close();
	
	/// カーソルの下にあるウィンドウを取得
	Window*	GetPointedWindow(Vec2f pos);

	/// イベントハンドラ
	bool	OnEvent(SDL_Event* ev);				//< SDLイベント
	void	OnUpdate();							//< Worldの更新時

	/// アニメーション処理
	void    Step(float dt);

	/// 描画
	void	Draw();

public:
	/// 派生クラスが実装する仮想関数
	virtual Window* CreateWindow(string type, Window* par);
	virtual bool    Init        ();
	
	WindowManager();
	virtual ~WindowManager();
};

}
