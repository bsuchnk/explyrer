#include "App.h"

wxIMPLEMENT_APP(App);


bool App::OnInit()
{
	frame = new CanvasFrame();
	frame->Show();

	return true;
}

//TestGLContext& App::GetContext(wxGLCanvas* canvas)
//{
//	m_glContext = new TestGLContext(canvas);
//	TestGLContext* glContext = m_glContext;
//    glContext->SetCurrent(*canvas);
//    return *glContext;
//}