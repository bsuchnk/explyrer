#pragma once

#define GLEW_STATIC
#include "dep/GLEW/glew.h"
#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "model.h"

class MyGLCanvas : public wxGLCanvas
{

private:
	std::vector<GLuint> shadersF;
	std::vector<GLuint> shadersM;

public:
	wxGLContext* m_context;
	Model model;
	glm::vec3 bcgColor;

	bool isShowingGrid;
	bool isShowingMesh;

	int fillShader = 0;
	int meshShader = 0;

public:
    MyGLCanvas(wxWindow* parent, int* attribList = NULL);
	~MyGLCanvas();

	void setFillShader(int id);
	void setMeshShader(int id);

public:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);

	void prepare3dViewport();
	void initiateShaders();

	void loadModel(wxString path);
	void destroyModel();

    wxDECLARE_EVENT_TABLE();
};