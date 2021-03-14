#pragma once

#define GLEW_STATIC
#include "dep/GLEW/glew.h"
#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "model.h"

class MyGLCanvas : public wxGLCanvas
{
public:
	wxGLContext* m_context;
	Model model;

	bool isShowingGrid;
	bool isShowingMesh;
/*
	float xRotation;
	float yRotation;
	float zRotation;*/

public:
    MyGLCanvas(wxWindow* parent, int* attribList = NULL);
	~MyGLCanvas();

public:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);

	void prepare3dViewport();

	void loadModel(wxString path);
	void destroyModel();

    wxDECLARE_EVENT_TABLE();
};