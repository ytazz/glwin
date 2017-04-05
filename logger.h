#pragma once

#include <glwin/window.h>

namespace GLWin{;

class Logger : public Window{
public:
	enum{
		Normal = LoggerEventBegin,
		Error,
	};
	struct Item{
		enum{
			Log = Window::Item::End,
		};
	};
	
	int 		nlogs;			///< ログの総行数
	int 		nrows;			///< 有効なログ行数
	int 		rowMax;			///< 行の最大文字数
	int			curLogIdx;		///< 表示した最後のメッセージの番号
	string		errorColor;		///< エラーメッセージの色
	Font*		errorFont;		///< エラーメッセージ用フォント
	int	        logListId;		///< ログ描画用ディスプレイリスト
	int 		logTexId;		///< ログ描画用テクスチャ
	int 		logBufId;		///< ログ描画用フレームバッファ
	float		logWidth;
	float		logHeight;
	
public:
	void SetErrorColor(const string& s);

	virtual void Read            (XMLNode* node);
	virtual bool Prepare         (int mask);
	virtual void CalcContentsSize();
	virtual void DrawContents    ();

	/// 派生クラスが実装する仮想関数
	virtual int    GetMessageLogLength()      = 0;		///< ログの行数
	virtual string GetMessageString   (int i) = 0;		///< i番目のメッセージ文字列
	virtual int    GetMessageLevel    (int i) = 0;		///< i番目のメッセージレベル
	
	Logger(Window* p);
};

}
