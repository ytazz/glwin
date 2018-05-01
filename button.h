#pragma once

#include <glwin/window.h>

namespace GLWin{;

class OutlineSet;
class Outline;
class RadioGroup;

/* ボタン

 */
class Button : public Window{
public:
	struct Item{
		enum{
			Style = Window::Item::End,
		};
	};
	struct Style{
		enum{
			Normal,		//< 矩形, テキスト表示
			Lamp  ,		//< 丸いランプ
			Opener,		//< リストボックスやフォルダの開閉ボタン
		};
	};
	enum{
		Clicked = Window::ButtonEventBegin,
		LatchOn ,
		LatchOff,
	};

	int		style;		///< 描画スタイル
	bool	pushed;		///< マウスによって押し下げられている状態
	bool	latch;		///< ラッチ（押すとオン，もう一度押すとオフ）
	bool	onoff;		///< オンオフ状態
	
	OutlineSet*			outlineSet;
	vector<Outline*>	outline;
	float				markerSize;
	
	RadioGroup*			group;

public:
	/// 描画スタイル設定
	void	SetStyle(int s);
	/// マーカサイズの設定
	void	SetMarkerSize(float w);
	/// ラッチ設定
	void	SetLatch(bool on = true);
	/// オンオフ状態設定（ラッチ時）
	void	SetOn(bool on = true);
	/// プッシュ
	void	Push();
	
	virtual void  Read            (XMLNode* node);
	virtual bool  OnEvent         (SDL_Event* ev);
	virtual bool  Prepare         (int mask);
	virtual Vec4f CalcBackColor   ();
	virtual Vec4f CalcFrameColor  ();
	virtual float CalcFrameWidth  ();
	virtual void  CalcContentsSize();
	virtual void  DrawContents    ();
	
	Button(Window* p);
};

}
