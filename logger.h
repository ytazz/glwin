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
	
	int 		nlogs;			///< ���O�̑��s��
	int 		nrows;			///< �L���ȃ��O�s��
	int 		rowMax;			///< �s�̍ő啶����
	int			curLogIdx;		///< �\�������Ō�̃��b�Z�[�W�̔ԍ�
	string		errorColor;		///< �G���[���b�Z�[�W�̐F
	Font*		errorFont;		///< �G���[���b�Z�[�W�p�t�H���g
	int	        logListId;		///< ���O�`��p�f�B�X�v���C���X�g
	int 		logTexId;		///< ���O�`��p�e�N�X�`��
	int 		logBufId;		///< ���O�`��p�t���[���o�b�t�@
	float		logWidth;
	float		logHeight;
	
public:
	void SetErrorColor(const string& s);

	virtual void Read            (XMLNode* node);
	virtual bool Prepare         (int mask);
	virtual void CalcContentsSize();
	virtual void DrawContents    ();

	/// �h���N���X���������鉼�z�֐�
	virtual int    GetMessageLogLength()      = 0;		///< ���O�̍s��
	virtual string GetMessageString   (int i) = 0;		///< i�Ԗڂ̃��b�Z�[�W������
	virtual int    GetMessageLevel    (int i) = 0;		///< i�Ԗڂ̃��b�Z�[�W���x��
	
	Logger(Window* p);
};

}
