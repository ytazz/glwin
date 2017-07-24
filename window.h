#pragma once

#include <glwin/typedefs.h>

#include <glwin/rect.h>
#include <glwin/radiogroup.h>

namespace GLWin{;

/* OpenGL�ŕ`�悷��E�B���h�E */

class Window;
class Font;
class ScrollBar;

/*
 * �E�B���h�E�C�x���g�̃R�[���o�b�N�N���X
 */
class WindowCallback{
public:
	virtual bool OnEvent(Window* win, int code){ return false; }
};

/*
 * �E�B���h�E��{�N���X
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
			Absolute = 1 << 1,	//< �q�E�B���h�E��position, size��D�悵���񂵂Ȃ�
			HFit     = 1 << 2,	//< �g�̉����ɍ��킹��
			HCenter  = 1 << 3,	//< ��������g�̒��S�ɍ��킹��
			Left     = 1 << 4,	//< ���[��g�ɍ��킹��
			Right    = 1 << 5,	//< �E�[��g�ɍ��킹��
			VFit     = 1 << 6,	//< �g�̏c���ɍ��킹��
			VCenter  = 1 << 7,	//< �c������g�̒��S�ɍ��킹��
			Top      = 1 << 8,	//< ��[��g�ɍ��킹��
			Bottom   = 1 << 9,	//< ���[��g�ɍ��킹��
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
	/// ���݂Ɉˑ��֌W�ɂ���A�C�e��
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

	/// ���
	Vec2f			pos;			///< �e�E�B���h�E�ɑ΂���ʒu
	Vec2f			posAbs;			///< ���[�g�E�B���h�E�ɑ΂���ʒu
	Vec2f			szWindow;		///< �E�B���h�E�̃T�C�Y
	Vec2f			szContents;		///< �R���e���c�̃T�C�Y
	int             depth;          ///< ���s�������̈ʒu. ���[�g�E�B���h�E��0, �}�C�i�X�ɑ傫���قǎ�O
	bool			enabled;		///< �C�l�[�u��
	bool			shown;			///< �\�����
	
	/// ����
	string          name;           ///< ���O�i�Q�Ɨp�j
	int				style;			///< �X�^�C��
	int				align;			///< ����
	Vec2f			margin;
	string			frameColor;		///< �g�̐F
	string			backColor;		///< �w�i�̐F
	string          backImage;      ///< �w�i�̉摜
	string			textColor;		///< �����̐F
	wstring			text;			///< �e�L�X�g
	wstring			hint;			///< �q���g������
	int				contentsAlign;	///< �e�L�X�g�`��ʒu
	string			fontFace;
	int				fontSize;		///< �t�H���g
	int				hotKey;			///< �V���[�g�J�b�g�L�[
	bool			focusable;		///< �t�H�[�J�X���Ƃ�邩
	bool			hscroll;		///< ���X�N���[�����邩
	bool			vscroll;		///< �c�X�N���[�����邩
	float			scrollBarWidth;	///< �X�N���[���o�[�̕�
	bool			popup;			///< �|�b�v�A�b�v�E�B���h�E

	/// �`��p�ϐ�
	Vec4f			backColorRGB;	///< �`��F��RGB
	Vec4f			frameColorRGB;
	Vec4f			textColorRGB;
	Font*			font;			///< �t�H���g
	vector<wstring>	textLines;		///< �����s�ɕ��������e�L�X�g
	Vec2f			szText;			///< �e�L�X�g�̈�̃T�C�Y
	int				textListId;		///< �e�L�X�g�̃f�B�X�v���C���X�g
	int				backListId;		///< �w�i�̃f�B�X�v���C���X�g
	int				frameListId;	///< �g�̃f�B�X�v���C���X�g
	int             backTexId;
	Vec2f			contentsPos;

	/// ����Ǘ�
	vector<float>	rowRatio;	///< �s�̕��i�䗦�j
	vector<float>	colRatio;	///< ��̕��i�䗦�j
	vector<float>	rowAbs;		///< �s�̕��i�s�N�Z���j
	vector<float>	colAbs;		///< ��̕��i�s�N�Z���j
	vector<Rect>	panels;		///< �q�E�B���h�E��z�u�����`
	int				rowIdx;		///< ���̃E�B���h�E�̐e�E�B���h�E�ɑ΂���z�u
	int				colIdx;
	
	/// �X�N���[���o�[
	ScrollBar*		scrollBar[2];
	
	void    ReadChild (XMLNode* node);
	void    SetReady  (int item, bool on);
	bool    IsReady   (int item);
	void	SetColor  (const Vec4f& c);
	void	AddChild  (Window* w);
	void	DelChild  (Window* w);
	Window* FindChild (string name);
	bool	HitTest   (Vec2f p, Window** w, int* dmax);
	
	/// �ċA�I�Ɋ֐��I�u�W�F�N�gfunc��]������
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

	// �o�^����Ă���R�[���o�b�N�N���X�փC�x���g��ʒm
	void Notify(int ev);
	// �`��
	void Draw();

public:
	// ���O��ݒ�i�����p�j
	void    SetName(const string& n);
	// �e�E�B���h�E�ɑ΂���ʒu��ݒ�E�擾
	void	SetPosition(float x, float y);
	Vec2f	GetPosition();
	// �E�B���h�E�T�C�Y��ݒ�E�擾
	void	SetSize(float w, float h);
	Vec2f	GetSize();
	// �ʒu�ƃT�C�Y�𓯎��ɐݒ�E�擾
	void	SetRect(float x, float y, float w, float h);
	// �R���e���c�̃T�C�Y���擾
	Vec2f	GetContentsSize();
	// �E�B���h�E�̉��s���ʒu��ݒ�
	//void	SetDepth(float d);
	// �`��X�^�C���ݒ�
	void	SetStyle(int s);
	// ����X�^�C���ݒ�
	void	SetAlign(int s);
	// �}�[�W����ݒ�
	void	SetMargin(float hmargin, float vmargin);
	// ����
	void	SetSplit(int nrow, int ncol);
	/** �s�̕���ݒ�
		@param irow	 �s�ԍ�
		@param ratio ���̎w��l
		ratio >  1.0�Ȃ�ΐ�Βl�i�s�N�Z�����j�Ƃ��Ĉ���
		ratio <= 1.0�Ȃ�Δ䗦�Ƃ��Ĉ���
		ratio =  0.0�Ȃ�΁C���̍s�ɑ�����q�E�B���h�E�̕��̍ő�l�Ƃ���
	 */
	void	SetRowRatio(int irow, float ratio);
	/** ��̕���ݒ�
		@param icol	 ��ԍ�
		@param ratio ���̎w��l
		SetRowRatio�Ɠ��l
	 */
	void	SetColRatio(int icol, float ratio);
	/** �e�E�B���h�E�̂ǂ̍s�E��ɔz�u���邩�w��
	 */
	void	AssignPanel(int icol, int irow);
	/// �F��ݒ�
	void	SetFrameColor(const string& c);
	void	SetBackColor (const string& c);
	void	SetTextColor (const string& c);
	// �摜
	void    SetBackImage(const string& filename);
	// �t�H���g
	void    SetFontFace(const string& face);
	void	SetFontSize(int sz);
	/// �\���������ݒ�
	virtual void SetText(const wstring& t);
	virtual void GetText(wstring& t);
	void	SetText(const string& t);
	void	GetText(string& t);
	/// �q���g������
	void    SetHint(const wstring& t);
	void    GetHint(wstring& t);
	void	SetHint(const string& t);
	void	GetHint(string& t);
	// �R���e���c�̐���X�^�C����ݒ�
	void	SetContentsAlign(int align);
	// �X�N���[���ݒ�
	void	SetHScroll       (bool on);
	void	SetVScroll       (bool on);
	void	SetScrollPos     (float h, float v);
	void	SetScrollbarWidth(float w);
	/// �|�b�v�A�b�v�ݒ�
	void	SetPopup(bool on);
	/// �R�[���o�b�N�o�^
	void	AddCallback   (WindowCallback* cb);
	void	RemoveCallback(WindowCallback* cb);
	/// �L���E�������肩��
	void	Enable(bool on = true);
	/// �\���E��\�����肩��
	void	Show(bool on = true);
	/// �㗬�̃E�B���h�E���l�������\������
	bool	IsShown();

	/// �X�N���[���o�[�ȊO�̎q�E�B���h�E���폜
	virtual void  Clear();
	/// XML��ǂݍ��ݑ����擾�C�q�E�B���h�E�쐬
	virtual void  Read(XMLNode* node);
	/// ������
	virtual bool  Init();
	/// �`�摮�����I�[�o���C�h
	virtual Vec4f CalcBackColor (){ return backColorRGB; }
	virtual Vec4f CalcFrameColor(){ return frameColorRGB; }
	virtual float CalcFrameWidth(){ return 1.0f; }
	/// �`�揀��
	virtual bool  Prepare(int mask);
	/// �`����@���I�[�o���C�h
	virtual void  DrawFrame   ();
	virtual void  DrawBack    ();
	virtual void  DrawContents();
	/** �E�B���h�E�T�C�Y���v�Z
		- �E�B���h�E�̑傫�����v�Z
	 */
	virtual void  CalcWindowSize();
	/** �R���e���c�̃T�C�Y�v�Z
		- �\���R���e���c�̑傫�����v�Z
		- �X�N���[���o�[�̕\���ؑւ�R���e���c����󓮓I�ɃE�B���h�E�T�C�Y���v�Z����̂Ɏg�p
	 */
	virtual void  CalcContentsSize();
	/** ���C�A�E�g
		- �s���C�񐔁C�s���̕��C�q�E�B���h�E�̌Œ�T�C�Y�C���񑮐��Ȃǂ����ƂɎq�E�B���h�E�̔z�u���m�肷��
		- �q�E�B���h�E�̃��C�A�E�g���J�X�^�}�C�Y�������ꍇ�ɃI�[�o���C�h����
	 */
	virtual void  Layout();
	/// SDL�C�x���g����
	virtual bool  OnEvent(SDL_Event* ev){ return false; }
	/// ���[���h���̍X�V
	virtual bool  OnUpdate(){ return true; }
	/// �A�j���[�V����
	virtual bool  Step(float dt){ return true; }
	
	Window(Window* p);
	virtual ~Window();
};

typedef vector< UTRef<Window> >	Wins;

}
