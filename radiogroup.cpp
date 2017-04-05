#include <glwin/button.h>
#include <glwin/radiogroup.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

RadioGroup::RadioGroup(Window* o){
	owner = o;
	owner->groups.push_back(this);
}

void RadioGroup::Read(XMLNode* node){
	string name;
	for(uint i = 0; ; i++) try{
		XMLNode* elemNode = node->GetNode("element", i);
		elemNode->Get(name, ".name");
		Window* w = owner->FindChild(name);
		if(w)
			Add((Button*)w);
	}
	catch(Exception&){ break; }
}

void RadioGroup::Add(Button* b){
	b->group = this;
	buttons.insert(b);
}

void RadioGroup::Remove(Button* b){
	b->group = 0;
	buttons.erase(buttons.find(b));
}

void RadioGroup::Switch(Button* b){
	for(set<Button*>::iterator it = buttons.begin(); it != buttons.end(); it++)
		(*it)->onoff = ((*it) == b);
}

}
