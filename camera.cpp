#include <glwin/camera.h>
#include <glwin/winmanager.h>
using namespace Scenebuilder;

namespace GLWin{;

Camera::Camera(Window* o){
	owner = o;
	owner->cameras.push_back(this);

	mode    = Projection::Perspective;
	radius  =  1.0f;
	aspect  =  1.0f;
	fov     =  0.3f;
	distMin =  1.0f;
	distMax = 10.0f;
	latMin  = (float)Rad(- 90.0);
	latMax  = (float)Rad(  90.0);
	lonMin  = (float)Rad(-180.0);
	lonMax  = (float)Rad( 180.0);
	rotGain  = 0.01f;
	zoomGain = 0.1f;
	trnGain  = 0.001f;

	lbutton = false;
	mbutton = false;
	rbutton = false;

	rotMask  = Mask::Left;
	trnMask  = Mask::Left | Mask::Alt;
	zoomMask = Mask::Right;

	target    = Vec3f();
	distance  =   1.0f;
	latitude  =   0.0f;
	longitude =   0.0f;

	UpdateView();
}

void Camera::Read(XMLNode* node){
	node->Get(name     , ".name"     );
	node->Get(radius   , ".radius"   );
	node->Get(aspect   , ".aspect"   );
	node->Get(fov      , ".fov"      );
	node->Get(latitude , ".latitude" );
	node->Get(longitude, ".longitude");
	node->Get(distance , ".distance" );
	node->Get(distMin  , ".dist_min" );
	node->Get(distMax  , ".dist_max" );
	node->Get(target   , ".target"   );

	string str;
	node->Get(str, "mode");
	if(str == "perspective") mode = Projection::Perspective;
	if(str == "ortho"      ) mode = Projection::Ortho;

}

bool Camera::OnEvent(SDL_Event* ev){
	bool ret = false;

	if(ev->type == SDL_MOUSEBUTTONDOWN){
		if(ev->button.button == SDL_BUTTON_LEFT  ) lbutton = true;
		if(ev->button.button == SDL_BUTTON_MIDDLE) mbutton = true;
		if(ev->button.button == SDL_BUTTON_RIGHT ) rbutton = true;
		lastX = ev->button.x;
		lastY = ev->button.y;
		ret = true;
	}
	if(ev->type == SDL_MOUSEBUTTONUP){
		if(ev->button.button == SDL_BUTTON_LEFT  ) lbutton = false;
		if(ev->button.button == SDL_BUTTON_MIDDLE) mbutton = false;
		if(ev->button.button == SDL_BUTTON_RIGHT ) rbutton = false;
		ret = true;
	}
	if(ev->type == SDL_MOUSEMOTION){
		int dx, dy;
		dx = ev->button.x - lastX;
		dy = ev->button.y - lastY;
		lastX = ev->button.x;
		lastY = ev->button.y;

		int mod = SDL_GetModState();
		int mask = 0;
		if(lbutton) mask |= Mask::Left;
		if(mbutton) mask |= Mask::Middle;
		if(rbutton) mask |= Mask::Right;
		if(mod & KMOD_CTRL ) mask |= Mask::Ctrl;
		if(mod & KMOD_SHIFT) mask |= Mask::Shift;
		if(mod & KMOD_ALT  ) mask |= Mask::Alt;

		if(mask == rotMask){
			Rotate(dx, dy);
		}
		if(mask == zoomMask){
			Zoom(dy, false);
		}
		if(mask == trnMask){
			Translate(dx, dy);
		}
		ret = true;
	}
	if(ev->type == SDL_MOUSEWHEEL){
		int dx, dy;
		dx = ev->wheel.x;
		dy = ev->wheel.y;
		/*if(ev->wheel.direction == SDL_MOUSEWHEEL_FLIPPED){
			dx = -dx;
			dy = -dy;
		}*/
		Zoom(dy, true);
	}
	
	return ret;
}

void Camera::Limit(float& v, float rmin, float rmax){
	if(rmin <= rmax)
		v = std::min(std::max(rmin, v), rmax);
}

void Camera::UpdateView(){
	// ビュー変換
	pose.Pos() = target + offset + distance * vec3_t(
		cosf(latitude) * sinf(longitude),
		sinf(latitude),
		cosf(latitude) * cosf(longitude));
	pose.Ori() = Quaternionf::Rot(longitude, 'y') * Quaternionf::Rot(-latitude, 'x');
	
	pose.ToAffine(affView);
}

void Camera::UpdateProj(){
	// 投影変換
	back  = distance + radius;
	front = std::max(distance - radius, 0.001f * radius);
	screen;
	if(mode == Projection::Ortho){
		// orthoの場合はdistanceに比例してスクリーンを拡大縮小する
		screen.x = 2.0f * distance * fov;
		screen.y = screen.x * aspect;
		affProj = Affinef::OrthoGL(Vec3f(0.0f, 0.0f, front), screen, front, back);
	}
	if(mode == Projection::Perspective){
		screen.x = 2.0f * front * fov;
		screen.y = screen.x * aspect;
		affProj = Affinef::ProjectionGL(Vec3f(0.0f, 0.0f, front), screen, front, back);
	}

}

void Camera::Rotate(int xrel, int yrel){
	longitude -= (float)xrel * rotGain;
	latitude  += (float)yrel * rotGain;
	Limit(longitude, lonMin, lonMax);
	Limit(latitude,  latMin, latMax);
}

void Camera::Zoom(int yrel, bool cursor){
	float rate = (float)exp((double)yrel * zoomGain);
	distance *= rate;
	Limit(distance, distMin, distMax);

	// カーソル位置を中心にズームするように注視点を移動
	if(cursor && owner){
		// 正規化されたカーソル座標
		Vec2f ps;
		ps.x = (lastX - owner->posAbs.x) / (owner->szWindow.x/2.0f) - 1.0f;
		ps.y = (lastY - owner->posAbs.y) / (owner->szWindow.y/2.0f) - 1.0f;
		ps.y = -ps.y;
		
		// カーソルをdistance面に投影した点
		Vec3f pl, pg;
		pl.x =  distance*fov*       ps.x;
		pl.y =  distance*fov*aspect*ps.y;
		pl.z = -distance;
		pg   = pose * pl;
	
		offset = (1.0f - rate)*(pg - target) + rate*offset;
	}
}

void Camera::Translate(int xrel, int yrel){
	Vec3f rel(-(float)xrel * distance * trnGain, (float)yrel * distance * trnGain, 0.0f);
	offset += pose.Ori() * rel;
}

void Camera::CalcRay(int x, int y, Vec3f& screen, Vec3f& dir){

}

}
