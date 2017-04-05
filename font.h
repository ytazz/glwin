#pragma once

#include <glwin/typedefs.h>

#include <SDL_ttf.h>

namespace GLWin{;

class FontManager;

/// �t�H���g
class Font : public UTRefCount{
public:
	struct Glyph : UTRefCount{
		int	    texId;			///< �e�N�X�`��ID
		int		x;				///< �\���I�t�Z�b�g
		int		y;
		int	    w;				///< �����̃C���[�W�T�C�Y
		int	    h;
		int		advance;		///< ������
		float	umax;			///< �e�N�X�`�����W�͈�
		float	vmax;

		void AddMetric(float& advance, float& ascent, float& descent);
		void Draw     (GRRenderIf* render, Vec2f pos);

		Glyph();
	};

	FontManager*	manager;
	TTF_Font*       font;
	string			name;		///< �t�H���g��
	int			    size;		///< �t�H���g�T�C�Y
	string			color;		///< �F��
	SDL_Color		colorRGB;
	int				height;		///< �����̍ő卂��
	int				ascent;		///< �x�[�X���C�������[�܂ł̍ő�l
	int				descent;	///< �x�[�X���C�����牺�[�܂ł̍ŏ��l
	int				lineSkip;	///< �s��

	typedef std::map<wchar_t, UTRef<Glyph> >	Glyphs;
	Glyphs			glyphs;		///< �����ʏ��

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
	// �t�H���g�̃����_�����O���@
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
