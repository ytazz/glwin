#include <glwin/font.h>
#include <glwin/winmanager.h>
#include <glwin/logger.h>

#include <sbmessage.h>
using namespace Scenebuilder;

namespace GLWin{;

///////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger(Window* p):Window(p){
	SetHint(L"ログが出ます");

	SetFontSize(16);
	focusable  = false;
	errorColor = "yellow";
	errorFont  = 0;
	curLogIdx  = -1;

	logTexId  = 0;
	logBufId  = 0;

	SetContentsAlign(Align::Left | Align::Top);
	SetHScroll(true);
	SetVScroll(true);

	AddCallback(manager);
}

void Logger::Read(XMLNode* node){
	Window::Read(node);
	
	// Logger固有属性
	string s;
	if(node->Get(s, ".error_color")) SetErrorColor(s);
}

void Logger::SetErrorColor(const string& s){
	errorColor = s;
	SetReady(Item::Log, false);
}

void Logger::CalcContentsSize(){
	Prepare(Item::Log);
	szContents.x = logWidth;	//< 2文字分マージン確保
	szContents.y = logHeight;	//< 1行分マージン確保
}

bool Logger::Prepare(int mask){
	Window::Prepare(mask);
	
	/* OnUpdateはコマンドが処理されたときしか呼ばれないので，
	 * ログの変化は描画タイミングでチェックする
	 */

	// メッセージログが伸びていたら通知
	nlogs = GetMessageLogLength();
	if(curLogIdx < (int)nlogs-1){
		Notify(Logger::Normal);
		SetReady(Item::Log, false);

		// 新しく表示したメッセージにエラーが含まれていたら通知
		bool found = false;
		for(int i = curLogIdx+1; i < nlogs; i++){
			int lv = GetMessageLevel(i);
			found |= (lv == Message::Level::Error);
		}
		if(found)
			Notify(Logger::Error);

		// ログ描画領域のサイズを計算
		nrows  = 0;
		rowMax = 0;
		for(int i = 0; i < nlogs; i++){
			string str = GetMessageString(i);
			uint   lv  = GetMessageLevel (i);
			
			if(str.empty())
				continue;
			if(lv == Message::Level::Extra)
				continue;

			nrows++;
			rowMax = std::max(rowMax, (int)str.size());
		}
		logWidth  = (fontSize/2) * (float)(rowMax+2);
		logHeight = (fontSize  ) * (float)(nrows +1);
		
		curLogIdx = nlogs-1;
	}

	if((mask && Item::Log) && !IsReady(Item::Log)){
		GRRenderIf* render = manager->render;

		// テクスチャ作成
		if(logTexId != 0)
			glDeleteTextures(1, (GLuint*)&logTexId);
		glGenTextures       (1, (GLuint*)&logTexId);
		glBindTexture  (GL_TEXTURE_2D, logTexId);
		glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA, (size_t)logWidth, (size_t)logHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		// フレームバッファ作成
		//if(manager->device->CheckGLVersion(3, 0)){
			if(logBufId != 0)
				glDeleteFramebuffers(1, (GLuint*)&logBufId);
			glGenFramebuffers       (1, (GLuint*)&logBufId);

			// フレームバッファにテクスチャをバインド
			glBindFramebuffer(GL_FRAMEBUFFER, logBufId);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, logTexId, 0);
		//}

		// ビューポートと投影変換
		Vec2f   vpPos  = render->GetViewportPos ();
		Vec2f   vpSize = render->GetViewportSize();
		Affinef affProjTmp;
		render->GetProjectionMatrix(affProjTmp);

		float w = logWidth;
		float h = logHeight;
		Affinef affProj;
		affProj.item(0,0) =  2.0f / w;
		affProj.item(1,1) = -2.0f / h;
		affProj.item(2,2) = -1.0f;
		affProj.item(3,3) =  1.0f;
		affProj.item(0,3) = -1.0f;
		affProj.item(1,3) =  1.0f;
		render->SetViewport(Vec2f(), Vec2f(w, h));
		render->SetProjectionMatrix(affProj);
	
		// テクスチャにログ文字列を描画
		errorFont = manager->fontManager->Get(fontFace, fontSize, errorColor);
		Vec2f p(fontSize/2, fontSize);
		
		for(int i = 0; i < nlogs; i++){
			string str = GetMessageString(i);
			uint   lv  = GetMessageLevel (i);
			// 空文字列は描画しない
			if(str.empty())
				continue;
			// Extraメッセージは描画しない（重くなるため）
			if(lv == Message::Level::Extra)
				continue;

			// 末尾の改行文字は描画しない
			if(str.back() == '\n')
				str.pop_back();

			if(lv == Message::Level::Normal)
				font->Draw(render, p, str);
			if(lv == Message::Level::Error)
				errorFont->Draw(render, p, str);
			p.y += fontSize;
		}

		//if(manager->device->CheckGLVersion(3, 0)){
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//}
		//else{
		//	glCopyTexSubImage2D(logTexId, 0, 0, 0, 0, 0, (size_t)logWidth, (size_t)logHeight);
		//}
		render->SetViewport(vpPos, vpSize);
		render->SetProjectionMatrix(affProjTmp);

		SetReady(Item::Log, true);
		SetReady(Window::Item::ScrollRange, false);
	}
	return true;
}

void Logger::DrawContents(){
	GRRenderIf* render = manager->render;	
	
	render->SetTexture2D(true);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, logTexId);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f    , logHeight);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(logWidth, logHeight);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f    , 0.0f     );
	glTexCoord2f(1.0f, 1.0f); glVertex2f(logWidth, 0.0f     );
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	render->SetTexture2D(false);
}

}
