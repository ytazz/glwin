#pragma once

#include <glwin/typedefs.h>

#include <SDL_ttf.h>

namespace GLWin{;

class FontManager;

/// フォント
class Font : public UTRefCount{
public:
	struct Glyph : UTRefCount{
		int	    texId;			///< テクスチャID
		int		x;				///< 表示オフセット
		int		y;
		int	    w;				///< 文字のイメージサイズ
		int	    h;
		int		advance;		///< 文字幅
		float	umax;			///< テクスチャ座標範囲
		float	vmax;

		void AddMetric(float& advance, float& ascent, float& descent);
		void Draw     (GRRenderIf* render, Vec2f pos);

		Glyph();
	};

	FontManager*	manager;
	TTF_Font*       font;
	string			name;		///< フォント名
	int			    size;		///< フォントサイズ
	string			color;		///< 色名
	SDL_Color		colorRGB;
	int				height;		///< 文字の最大高さ
	int				ascent;		///< ベースラインから上端までの最大値
	int				descent;	///< ベースラインから下端までの最小値
	int				lineSkip;	///< 行間

	typedef std::map<wchar_t, UTRef<Glyph> >	Glyphs;
	Glyphs			glyphs;		///< 文字別情報

public:
	bool   Init       ();
	void   Close      ();
	Glyph* CreateGlyph(wchar_t c);
	Glyph* GetGlyph   (wchar_t c);
	void   GetMetric  (const string&  text, float& advance, float& ascent, float& descent);
	void   GetMetric  (const wstring& text, float& advance, float& ascent, float& descent);
	void   Draw       (GRRenderIf* render, Vec2f pos, const string&  str);
	void   Draw       (GRRenderIf* render, Vec2f pos, const wstring& str);
	
	 Font(FontManager* _manager);
	~Font();
};

class FontManager : public UTRefCount, public vector< UTRef<Font> >{
public:
	// フォントのレンダリング方法
	enum{
		Solid,
		Shaded,
		Blended,
	};

	int			mode;
	
	Font* Add(const string& name, int size, const string& color);
	
public:
	bool Init ();
	void Close();

	Font* Get(const string& name, int size, const string& color);
	
	FontManager();
};

}
