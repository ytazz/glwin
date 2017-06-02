#include <glwin/winmanager.h>
#include <glwin/viewer.h>

using namespace Scenebuilder;

namespace GLWin{;

Viewer::Viewer(Window* p):Window(p){
	SetFrameColor("none");
	SetBackColor ("none");
	SetHint(L"�r���[���E�B���h�E�ł�");
}

void Viewer::DrawContents(){
	GRRenderIf* render = manager->render;

	Affinef affProj;
	Affinef affView;
	Affinef affModel;
	render->GetProjectionMatrix(affProj );
	render->GetViewMatrix      (affView );
	render->GetModelMatrix     (affModel);

	render->SetLighting (true);
	render->SetDepthTest(true);
	
	/* �r���[�|�[�g��ݒ�
	   - �E�B���h�E���W�n�͍�������_�Ƃ��Ă��邪�r���[�|�[�g���W�ł͍��������_�Ȃ̂Œ���
	 */
	Vec2f vpPos  = render->GetViewportPos ();
	Vec2f vpSize = render->GetViewportSize();
	render->SetViewport(Vec2f(posAbs.x, (float)manager->windowHeight - (posAbs.y + szWindow.y)), szWindow);

	DrawView();

	render->SetLighting (false);
	render->SetDepthTest(false);
	
	render->SetProjectionMatrix(affProj );
	render->SetViewMatrix      (affView );
	render->SetModelMatrix     (affModel);
	render->SetViewport        (vpPos, vpSize);
}

}
