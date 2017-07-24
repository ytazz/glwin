#pragma once

#include <glwin/typedefs.h>

#include <glwin/rect.h>
#include <glwin/radiogroup.h>

namespace GLWin{;

/* OpenGLで描画するウィンドウ */

class Window;
class Font;
class ScrollBar;

/*
 * ウィンドウイベントのコールバッククラス
 */
class WindowCallback{
public:
	virtual bool OnEvent(Window* win, int code){ return false; }
};

/*
 * ウィンドウ基本クラス
 */
class Window : public UTRefCount{
	friend class WindowManager;
public:
	enum Events{
		WindowEventBegin,
		 FocusIn,
		 FocusOut,
		 HoverIn,
		 HoverOut,
		 Destroy,
		WindowEventEnd = WindowEventBegin + 100,
		ButtonEventBegin,
		ButtonEventEnd = ButtonEventBegin + 100,
		ListBoxEventBegin,
		ListBoxEventEnd = ListBoxEventBegin + 100,
		ConsoleEventBegin,
		ConsoleEventEnd = ConsoleEventBegin + 100,
		FolderEventBegin,
		FolderEventEnd = FolderEventBegin + 100,
		LoggerEventBegin,
		LoggetEventEnd = LoggerEventBegin + 100,
		ScrollBarEventBegin,
		ScrollBarEventEnd = ScrollBarEventBegin + 100,
		TextboxEventBegin,
		TextboxEventEnd = TextboxEventBegin + 100,
		TreeViewEventBegin,
		TreeViewEventEnd = TreeViewEventBegin + 100,
		SwitcherEventBegin,
		SwitcherEventEnd = SwitcherEventBegin + 100,
	};
	struct Style{
		enum{
			Normal,	
		};
	};
	struct Align{
		enum{
			Absolute = 1 << 1,	//< 子ウィンドウのposition, sizeを優先し整列しない
			HFit     = 1 << 2,	//< 枠の横幅に合わせる
			HCenter  = 1 << 3,	//< 横方向を枠の中心に合わせる
			Left     = 1 << 4,	//< 左端を枠に合わせる
			Right    = 1 << 5,	//< 右端を枠に合わせる
			VFit     = 1 << 6,	//< 枠の縦幅に合わせる
			VCenter  = 1 << 7,	//< 縦方向を枠の中心に合わせる
			Top      = 1 << 8,	//< 上端を枠に合わせる
			Bottom   = 1 << 9,	//< 下端を枠に合わせる
		};
		static int FromString(string s);
	};
	/// 
	struct Group{
		enum{
			All      = 0,
			Shown    = 1 << 0,
			NonPopup = 1 << 1,
		};
	};
	/// 相互に依存関係にあるアイテム
	struct Item{
		enum{
			Color       = 0x1 << 0,
			Font        = 0x1 << 1,
			Rect        = 0x1 << 2,
			Text        = 0x1 << 3,
			Layout      = 0x1 << 4,
			ContentsPos = 0x1 << 5,
			ScrollRange = 0x1 << 6,
			Image       = 0x1 << 7,
			End         = 0x1 << 8,
			All         = 0xffff,
		};
	};
	int itemReady;
	
public:
	typedef vector< UTRef<Window> >	WindowRefs;
	
	WindowManager*	manager;
	Window*			parent;
	WindowRefs		children;
	RadioGroupRefs  groups;
	
	vector<WindowCallback*>		callbacks;

	/// 状態
	Vec2f			pos;			///< 親ウィンドウに対する位置
	Vec2f			posAbs;			///< ルートウィンドウに対する位置
	Vec2f			szWindow;		///< ウィンドウのサイズ
	Vec2f			szContents;		///< コンテンツのサイズ
	int             depth;          ///< 奥行き方向の位置. ルートウィンドウは0, マイナスに大きいほど手前
	bool			enabled;		///< イネーブル
	bool			shown;			///< 表示状態
	
	/// 属性
	string          name;           ///< 名前（参照用）
	int				style;			///< スタイル
	int				align;			///< 整列
	Vec2f			margin;
	string			frameColor;		///< 枠の色
	string			backColor;		///< 背景の色
	string          backImage;      ///< 背景の画像
	string			textColor;		///< 文字の色
	wstring			text;			///< テキスト
	wstring			hint;			///< ヒント文字列
	int				contentsAlign;	///< テキスト描画位置
	string			fontFace;
	int				fontSize;		///< フォント
	int				hotKey;			///< ショートカットキー
	bool			focusable;		///< フォーカスをとれるか
	bool			hscroll;		///< 横スクロールするか
	bool			vscroll;		///< 縦スクロールするか
	float			scrollBarWidth;	///< スクロールバーの幅
	bool			popup;			///< ポップアップウィンドウ

	/// 描画用変数
	Vec4f			backColorRGB;	///< 描画色のRGB
	Vec4f			frameColorRGB;
	Vec4f			textColorRGB;
	Font*			font;			///< フォント
	vector<wstring>	textLines;		///< 複数行に分解したテキスト
	Vec2f			szText;			///< テキスト領域のサイズ
	int				textListId;		///< テキストのディスプレイリスト
	int				backListId;		///< 背景のディスプレイリスト
	int				frameListId;	///< 枠のディスプレイリスト
	int             backTexId;
	Vec2f			contentsPos;

	/// 整列管理
	vector<float>	rowRatio;	///< 行の幅（比率）
	vector<float>	colRatio;	///< 列の幅（比率）
	vector<float>	rowAbs;		///< 行の幅（ピクセル）
	vector<float>	colAbs;		///< 列の幅（ピクセル）
	vector<Rect>	panels;		///< 子ウィンドウを配置する矩形
	int				rowIdx;		///< このウィンドウの親ウィンドウに対する配置
	int				colIdx;
	
	/// スクロールバー
	ScrollBar*		scrollBar[2];
	
	void    ReadChild (XMLNode* node);
	void    SetReady  (int item, bool on);
	bool    IsReady   (int item);
	void	SetColor  (const Vec4f& c);
	void	AddChild  (Window* w);
	void	DelChild  (Window* w);
	Window* FindChild (string name);
	bool	HitTest   (Vec2f p, Window** w, int* dmax);
	
	/// 再帰的に関数オブジェクトfuncを評価する
	template<class T>
	void Recurs(T func, int mask){
		if((mask & Group::Shown) && !shown)
			return;
		if((mask & Group::NonPopup) && popup)
			return;

		if(func(this)){
			for(int i = 0; i < (int)children.size(); i++)
				children[i]->Recurs(func, mask);
		}
	}

	// 登録されているコールバッククラスへイベントを通知
	void Notify(int ev);
	// 描画
	void Draw();

public:
	// 名前を設定（検索用）
	void    SetName(const string& n);
	// 親ウィンドウに対する位置を設定・取得
	void	SetPosition(float x, float y);
	Vec2f	GetPosition();
	// ウィンドウサイズを設定・取得
	void	SetSize(float w, float h);
	Vec2f	GetSize();
	// 位置とサイズを同時に設定・取得
	void	SetRect(float x, float y, float w, float h);
	// コンテンツのサイズを取得
	Vec2f	GetContentsSize();
	// ウィンドウの奥行き位置を設定
	//void	SetDepth(float d);
	// 描画スタイル設定
	void	SetStyle(int s);
	// 整列スタイル設定
	void	SetAlign(int s);
	// マージンを設定
	void	SetMargin(float hmargin, float vmargin);
	// 分割
	void	SetSplit(int nrow, int ncol);
	/** 行の幅を設定
		@param irow	 行番号
		@param ratio 幅の指定値
		ratio >  1.0ならば絶対値（ピクセル数）として扱う
		ratio <= 1.0ならば比率として扱う
		ratio =  0.0ならば，その行に属する子ウィンドウの幅の最大値とする
	 */
	void	SetRowRatio(int irow, float ratio);
	/** 列の幅を設定
		@param icol	 列番号
		@param ratio 幅の指定値
		SetRowRatioと同様
	 */
	void	SetColRatio(int icol, float ratio);
	/** 親ウィンドウのどの行・列に配置するか指定
	 */
	void	AssignPanel(int icol, int irow);
	/// 色を設定
	void	SetFrameColor(const string& c);
	void	SetBackColor (const string& c);
	void	SetTextColor (const string& c);
	// 画像
	void    SetBackImage(const string& filename);
	// フォント
	void    SetFontFace(const string& face);
	void	SetFontSize(int sz);
	/// 表示文字列を設定
	virtual void SetText(const wstring& t);
	virtual void GetText(wstring& t);
	void	SetText(const string& t);
	void	GetText(string& t);
	/// ヒント文字列
	void    SetHint(const wstring& t);
	void    GetHint(wstring& t);
	void	SetHint(const string& t);
	void	GetHint(string& t);
	// コンテンツの整列スタイルを設定
	void	SetContentsAlign(int align);
	// スクロール設定
	void	SetHScroll       (bool on);
	void	SetVScroll       (bool on);
	void	SetScrollPos     (float h, float v);
	void	SetScrollbarWidth(float w);
	/// ポップアップ設定
	void	SetPopup(bool on);
	/// コールバック登録
	void	AddCallback   (WindowCallback* cb);
	void	RemoveCallback(WindowCallback* cb);
	/// 有効・無効きりかえ
	void	Enable(bool on = true);
	/// 表示・非表示きりかえ
	void	Show(bool on = true);
	/// 上流のウィンドウを考慮した表示判定
	bool	IsShown();

	/// スクロールバー以外の子ウィンドウを削除
	virtual void  Clear();
	/// XMLを読み込み属性取得，子ウィンドウ作成
	virtual void  Read(XMLNode* node);
	/// 初期化
	virtual bool  Init();
	/// 描画属性をオーバライド
	virtual Vec4f CalcBackColor (){ return backColorRGB; }
	virtual Vec4f CalcFrameColor(){ return frameColorRGB; }
	virtual float CalcFrameWidth(){ return 1.0f; }
	/// 描画準備
	virtual bool  Prepare(int mask);
	/// 描画方法をオーバライド
	virtual void  DrawFrame   ();
	virtual void  DrawBack    ();
	virtual void  DrawContents();
	/** ウィンドウサイズを計算
		- ウィンドウの大きさを計算
	 */
	virtual void  CalcWindowSize();
	/** コンテンツのサイズ計算
		- 表示コンテンツの大きさを計算
		- スクロールバーの表示切替やコンテンツから受動的にウィンドウサイズを計算するのに使用
	 */
	virtual void  CalcContentsSize();
	/** レイアウト
		- 行数，列数，行や列の幅，子ウィンドウの固定サイズ，整列属性などをもとに子ウィンドウの配置を確定する
		- 子ウィンドウのレイアウトをカスタマイズしたい場合にオーバライドする
	 */
	virtual void  Layout();
	/// SDLイベント処理
	virtual bool  OnEvent(SDL_Event* ev){ return false; }
	/// ワールド情報の更新
	virtual bool  OnUpdate(){ return true; }
	/// アニメーション
	virtual bool  Step(float dt){ return true; }
	
	Window(Window* p);
	virtual ~Window();
};

typedef vector< UTRef<Window> >	Wins;

}
