#include "MyGLCanvas.h"
#include "MyGLContext.h"
#include <wx/app.h>
#include "App.h"
#include "global.h"
#include "shaders.h"

wxBEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
	EVT_PAINT(MyGLCanvas::OnPaint)
	EVT_KEY_DOWN(MyGLCanvas::OnKeyDown)
wxEND_EVENT_TABLE()

MyGLCanvas::MyGLCanvas(wxWindow* parent, int* attribList)
    : wxGLCanvas(parent, wxID_ANY, attribList,
        wxDefaultPosition, wxDefaultSize,
        wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);
	wxGLCanvas::SetCurrent(*m_context);

	if (glewInit() != GLEW_OK) {
		wxMessageBox("no glew for you");
		return;
	}

	prepare3dViewport();

	loadModel("ply/ball.ply");
}

MyGLCanvas::~MyGLCanvas()
{
	delete m_context;
}

void MyGLCanvas::prepare3dViewport()
{
	const wxSize cs = GetClientSize();
	int w = cs.x;
	int h = cs.y;
	if (w < h) {
		glViewport(0, (h-w)/2, w, w);
	}
	else if (w > h) {
		glViewport((w-h)/2, 0, h, h);
	}
	else {
		glViewport(0, 0, w, h);
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Black Background
	glClearDepth(1.0f);	// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
}

void MyGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC dc(this);

	prepare3dViewport();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model.drawGrid();
	model.drawAxis();
	model.draw();

	glFlush();

    SwapBuffers();
}

void MyGLCanvas::OnKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode())
    {
        case WXK_RIGHT:break;
        case WXK_LEFT:break;
        case WXK_DOWN: break;
        case WXK_UP:break;

        default:event.Skip();return;
    }
}


void MyGLCanvas::loadModel(wxString path)
{
	model.loadPLY(std::string(path.mb_str()));
	model.makeBuffers();
	model.calculateMVP();
	model.generateGrid();
	model.generateAxis();

	GLuint s1, s2;
	s1 = CreateShader("shaders/vs_rainbow.glsl", "shaders/fs_rainbow.glsl");
	s2 = CreateShader("shaders/vs_raw.glsl", "shaders/fs_black.glsl");
	model.setShaderPrograms(s1, s2);
}

void MyGLCanvas::destroyModel()
{
	model.freeBuffers();
}
