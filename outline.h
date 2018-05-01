#pragma once

#include <glwin/typedefs.h>
#include <glwin/rect.h>

namespace GLWin{;

class OutlineManager;

/* Outline
	- SVGパスを点列へ変換してOpenGLなどで描画する際に使用
 */

class Outline : public UTRefCount{
public:
	struct Element{
		bool			enabled;
		int				listId;

		Element(){
			enabled = true;
			listId  = -1;
		}
	};
	std::vector<Element>		elements;

	bool			ready;
	
	string			name;
	Vec2f			center;
	vector<Vec2f>	points;

public:
	void Read        (SVG::Path* path);
	void CalcRange   (Rect& r);
	void Scale       (float sx, float sy, Vec2f offset);
	void EnableStroke(bool on = true);
	void EnableFill  (bool on = true);

	virtual void Create (GRRenderIf* render);
	virtual void Cleanup(GRRenderIf* render);
	virtual void Draw   (GRRenderIf* render);

	Outline();
};

class OutlineSet : public UTRefCount{
public:
	string		name;
	string		filename;
	Rect		rect;

	OutlineManager*	manager;

	typedef vector< UTRef<Outline> >	Outlines;
	Outlines	outlines;

public:
	bool		Init();
	Outline*	Find(string n);

	OutlineSet(OutlineManager* _manager, string _name, string _filename);
};

class OutlineManager : public UTRefCount{
public:
	typedef vector< UTRef<OutlineSet> >	OutlineSets;
	OutlineSets		outlineSets;
	string			dirname;

public:
	void			Read(XMLNode* node);
	bool			Init();
	OutlineSet*		Find(string name);

	OutlineManager();
};

}
