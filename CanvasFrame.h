#pragma once

#include "wx/wx.h"
#include <map>
#include <fstream>
#include "MyGLCanvas.h"

class CanvasFrame : public wxFrame
{
public:
	CanvasFrame();

public:
	wxMenuBar* menubar;
	wxMenu* menu_file;
	MyGLCanvas* myglcanvas;

	wxCheckBox* showGrid;
	wxCheckBox* showMesh;
	wxSlider* xRotateSlider;
	wxSlider* yRotateSlider;
	wxSlider* zRotateSlider;

	void RefreshControls();

	void onFileMenuClick(wxCommandEvent& evt);
	void onShowGrid(wxCommandEvent& evt);
	void onShowMesh(wxCommandEvent& evt);
	void onRotateSlider(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};
