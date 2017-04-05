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

	int         mode;				///< 投影変換モード
	float       radius;             ///< 注視対象の半径
	float       aspect;             ///< スクリーンアスペクト比
	float       fov;				///< field of view
	float		distMin;			///< カメラ距離最小値
	float		distMax;			///< カメラ距離最大値
	float		latMin;				///< 緯度範囲
	float		latMax;
	float		lonMin;				///< 経度範囲
	float		lonMax;
	float		rotGain;			///< 角度操作ゲイン
	float		zoomGain;			///< ズームゲイン
	float		trnGain;			///< 注視点移動ゲイン
	
	int			rotMask;
	int			zoomMask;
	int			trnMask;

	bool		lbutton;
	bool		mbutton;
	bool		rbutton;
	int			modifier;
	int			lastX;
	int			lastY;

	Vec3f		target;		///< 注視点
	Vec3f		offset;		///< 注視点オフセット
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
