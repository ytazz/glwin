#include <glwin/rect.h>
#include <glwin/outline.h>
using namespace Scenebuilder;

#define USE_FREEGLUT
#include <GL/glew.h>
#include <GL/glut.h>

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Outline::Outline(){
	ready  = false;
}

void Outline::Read(SVG::Path* path){
	name = path->name;

	path->GetPoints(points);
	// ï`âÊópíÜêSì_
	center.clear();
	if(!points.empty()){
		for(uint i = 0; i < points.size(); i++)
			center += points[i];
		center /= (float)points.size();
	}

	elements.resize(2);

	ready = false;
}

void Outline::CalcRange(Rect& r){
	for(uint i = 0; i < points.size(); i++)
		r.Expand(points[i]);
}

void Outline::Scale(float sx, float sy, Vec2f offset){
	for(uint i = 0; i < points.size(); i++){
		Vec2f& p = points[i];
		p.x = sx * (p.x + offset.x);
		p.y = sy * (p.y + offset.y);
	}
	center.x = sx * (center.x + offset.x);
	center.y = sy * (center.y + offset.y);
}

void Outline::EnableStroke(bool on){
	elements[0].enabled = on;
}

void Outline::EnableFill  (bool on){
	elements[1].enabled = on;
}

void Outline::Create(GRRenderIf* render){
	elements[0].listId = render->StartList();
	glBegin(GL_LINE_STRIP);
	for(uint i = 0; i < points.size(); i++)
		glVertex2f(points[i].x, points[i].y);
	glEnd();
	render->EndList();

	elements[1].listId = render->StartList();
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(center.x, center.y);
	for(uint i = 0; i < points.size(); i++)
		glVertex2f(points[i].x, points[i].y);
	glEnd();
	glEnable(GL_CULL_FACE);
	render->EndList();
}

void Outline::Cleanup(GRRenderIf* render){
	for(uint i = 0; i < elements.size(); i++){
		Element& elem = elements[i];
		if(elem.listId != -1)
			render->ReleaseList(elem.listId);
	}
}

void Outline::Draw(GRRenderIf* render){
	if(!ready){
		Cleanup(render);
		Create (render);
		ready = true;
	}
	for(uint i = 0; i < elements.size(); i++){
		Element& elem = elements[i];
		if(!elem.enabled)
			continue;
		render->DrawList(elem.listId);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

OutlineSet::OutlineSet(OutlineManager* _manager, string _name, string _filename){
	manager  = _manager;
	name     = _name;
	filename = _filename;
}

bool OutlineSet::Init(){
	XML xml;
	SVG svg;
	try{
		xml.Load(manager->dirname + filename);
		svg.Load(xml);
	}
	catch(...){
		return false;
	}

	rect = Rect();

	for(uint i = 0; i < svg.paths.size(); i++){
		Outline* o = new Outline();
		o->Read(svg.paths[i]);
		o->CalcRange(rect);
		outlines.push_back(o);
	}
	
	float s = 1.0f / rect.Width();
	
	for(uint i = 0; i < outlines.size(); i++)
		outlines[i]->Scale(s, s, -rect.TopLeft());
	
	return true;
}

Outline* OutlineSet::Find(string n){
	for(uint i = 0; i < outlines.size(); i++){
		Outline* o = outlines[i];
		if(o->name == n)
			return o;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

OutlineManager::OutlineManager(){
	dirname = "drawings/";
}

void OutlineManager::Read(XML& xml){
	XMLNode* drawingNode;
	try{
		drawingNode = xml.GetRootNode()->GetNode("drawing");
	}
	catch(XMLException&){ return; }

	for(int i = 0; ; i++){
		try{
			XMLNode* node = drawingNode->GetNode("outline", i);
			
			string name;
			string filename;
			node->Get(name    , ".name"    );
			node->Get(filename, ".filename");
			
			outlineSets.push_back(new OutlineSet(this, name, filename));
		}
		catch(XMLException&){ break; }
	}
}

bool OutlineManager::Init(){
	for(uint i = 0; i < outlineSets.size(); i++)
		outlineSets[i]->Init();
	return true;
}

OutlineSet* OutlineManager::Find(string name){
	for(uint i = 0; i < outlineSets.size(); i++){
		OutlineSet* os = outlineSets[i];
		if(os->name == name)
			return os;
	}
	return 0;
}

}
