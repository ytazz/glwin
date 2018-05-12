#include <glwin/winmanager.h>
#include <glwin/viewer.h>

using namespace Scenebuilder;

namespace GLWin{;

Viewer::Viewer(Window* p):Window(p){
	SetFrameColor("none");
	SetBackColor ("none");
	SetHint(L"ビューワウィンドウです");

	zUp = true;

	curCamera = 0;
}

void Viewer::Read(XMLNode* node){
	node->Get(cameraName, ".camera");
	node->Get(zUp       , ".z_up"  );

	Window::Read(node);
}

bool Viewer::Init(){
	for(Camera* cam : cameras){
		if(cam->name == cameraName)
			curCamera = cam;
	}
	if(!curCamera && !cameras.empty())
		curCamera = cameras[0];

	return Window::Init();
}

bool Viewer::OnEvent(SDL_Event* ev){
	return curCamera->OnEvent(ev);
}

void Viewer::DrawContents(){
	GRRenderIf* render = manager->render;

	// store current transformation and viewport
	Affinef affProj;
	Affinef affView;
	Affinef affModel;
	render->GetProjectionMatrix(affProj );
	render->GetViewMatrix      (affView );
	render->GetModelMatrix     (affModel);
	Vec2f vpPos  = render->GetViewportPos ();
	Vec2f vpSize = render->GetViewportSize();
	
	// set viewport
	render->SetViewport(Vec2f(posAbs.x, (float)manager->windowHeight - (posAbs.y + szWindow.y)), szWindow);
	
	// set camera transformation
	curCamera->aspect = szWindow.y / szWindow.x;
	curCamera->UpdateView();
	curCamera->UpdateProj();
	render->SetViewMatrix      (curCamera->affView.inv());
	render->SetProjectionMatrix(curCamera->affProj      );

	// set z-axis up
	if(zUp){
		render->PushModelMatrix();
		render->MultModelMatrix(Affinef::Rot((float)Rad(-90.0), 'x'));
	}

	//render->SetLighting (true);
	//render->SetDepthTest(true);
	
	// notify draw event
	Notify(Viewer::Event::Draw);

	// execute virtual function
	DrawView();

	render->SetLighting (false);
	render->SetDepthTest(false);

	if(zUp)
		render->PopModelMatrix();
	
	// restore transformation and viewport setting
	render->SetProjectionMatrix(affProj );
	render->SetViewMatrix      (affView );
	render->SetModelMatrix     (affModel);
	render->SetViewport        (vpPos, vpSize);
}

}
