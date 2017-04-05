#pragma once

#include <sbtypes.h>

#include <sdl.h>

namespace GLWin{;

class Window;

class Camera : public UTRefCount{
public:
	struct Projection{
		enum{
			Ortho,
			Perspective,
		};
	};
	struct Mask{
		enum{
			Left   = 0x1 << 0,
			Middle = 0x1 << 1,
			Right  = 0x1 << 2,
			Ctrl   = 0x1 << 3,
			Shift  = 0x1 << 4,
			Alt    = 0x1 << 5,
		};
	};

	Window*     owner;

	int         mode;				///< ���e�ϊ����[�h
	float       radius;             ///< �����Ώۂ̔��a
	float       aspect;             ///< �X�N���[���A�X�y�N�g��
	float       fov;				///< field of view
	float		distMin;			///< �J���������ŏ��l
	float		distMax;			///< �J���������ő�l
	float		latMin;				///< �ܓx�͈�
	float		latMax;
	float		lonMin;				///< �o�x�͈�
	float		lonMax;
	float		rotGain;			///< �p�x����Q�C��
	float		zoomGain;			///< �Y�[���Q�C��
	float		trnGain;			///< �����_�ړ��Q�C��
	
	int			rotMask;
	int			zoomMask;
	int			trnMask;

	bool		lbutton;
	bool		mbutton;
	bool		rbutton;
	int			modifier;
	int			lastX;
	int			lastY;

	Vec3f		target;		///< �����_
	Vec3f		offset;		///< �����_�I�t�Z�b�g
	float		latitude;
	float		longitude;
	float		distance;
	float       back  ;
	float       front ;
	Vec2f       screen;
	
	Posef		pose;
	Affinef		affView;
	Affinef     affProj;

public:
	bool OnEvent(SDL_Event* ev);
	
	void Limit     (float& v, float rmin, float rmax);
	void Rotate    (int xrel, int yrel);
	void Zoom      (int yrel, bool cursor);
	void Translate (int xrel, int yrel);
	void CalcRay   (int x, int y, Vec3f& screen, Vec3f& dir);

	virtual void UpdateView();
	virtual void UpdateProj();

	Camera();
};

}
