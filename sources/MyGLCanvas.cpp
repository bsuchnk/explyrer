#include "MyGLCanvas.h"
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

	//
	isShowingGrid = true;
	isShowingMesh = true;
	bcgColor = { 1.0, 1.0, 1.0 };

	initiateShaders();

	model = new Model();
	loadModel("ply/ball.ply");
}

MyGLCanvas::~MyGLCanvas()
{
	delete m_context;
	delete model;
}

void MyGLCanvas::setFillShader(int id)
{
	fillShader = id;
	model->setShaderPrograms(shadersF[fillShader], shadersM[meshShader]);
}

void MyGLCanvas::setMeshShader(int id)
{
	meshShader = id;
	model->setShaderPrograms(fillShader, meshShader);
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

	glClearColor(bcgColor.x, bcgColor.y, bcgColor.z, 1.0f); // Black Background
	glClearDepth(1.0f);	// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glLineWidth(0.5);
	//glEnable(GL_MULTISAMPLE);
}

void MyGLCanvas::initiateShaders()
{
	shadersF.push_back(CreateShader("shaders/vs_rainbow.glsl", "shaders/fs_colored.glsl"));
	shadersF.push_back(CreateShader("shaders/vs_gradient.glsl", "shaders/fs_colored.glsl"));
	shadersF.push_back(CreateShader("shaders/vs_white.glsl", "shaders/fs_colored.glsl"));
	shadersF.push_back(CreateShader("shaders/vs_black.glsl", "shaders/fs_colored.glsl"));

	shadersM.push_back(CreateShader("shaders/vs_black.glsl", "shaders/fs_colored.glsl"));
}

void MyGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC dc(this);

	prepare3dViewport();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (isShowingGrid) {
		model->drawGrid();
		model->drawAxis();
	}
	model->draw();
	if (isShowingMesh) {
		model->drawMesh();
	}

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
	model->loadPLY(std::string(path.mb_str()));
	model->makeBuffers();
	model->calculateMVP();
	model->generateGrid();
	model->generateAxis();

	model->setShaderPrograms(shadersF[fillShader], shadersM[meshShader]);
}

void MyGLCanvas::destroyModel()
{
	delete model;
}
