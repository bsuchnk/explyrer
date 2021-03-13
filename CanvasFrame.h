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

	void onFileMenuClick(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};
