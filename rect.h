#pragma once

#include <sbtypes.h>

struct Rect{
	bool    valid;
	float	left;
	float	top;
	float	right;
	float	bottom;

	float	Width      (){ return right - left;                }
	float	Height     (){ return bottom - top;                }
	float	HCenter    (){ return 0.5f * (left + right);       }
	float	VCenter    (){ return 0.5f * (top + bottom);       }
	Vec2f	Size       (){ return Vec2f(Width(), Height());    }
	Vec2f	TopLeft    (){ return Vec2f(left, top);            }
	Vec2f	BottomRight(){ return Vec2f(right, bottom);        }
	Vec2f	Center     (){ return Vec2f(HCenter(), VCenter()); }
	
	void	Expand  (Vec2f p){
		if(!valid){
			left = right  = p.x;
			top  = bottom = p.y;
			valid = true;
		}
		else{
			left   = std::min(left  , p.x);
			right  = std::max(right , p.x);
			top    = std::min(top   , p.y);
			bottom = std::max(bottom, p.y);
		}
	}
	
	bool Includes(Vec2f p){
		return (left <= p.x && p.x <= right && top <= p.y && p.y <= bottom);
	}

	Rect():valid(false), left(0.0f), top(0.0f), right(0.0f), bottom(0.0f){}
	Rect(float l, float t, float r, float b):valid(false), left(l), top(t), right(r), bottom(b){}
};