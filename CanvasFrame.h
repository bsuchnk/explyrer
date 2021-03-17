#pragma once

#include "wx/wx.h"
#include <map>
#include <fstream>
#include "MyGLCanvas.h"
#include <wx/bmpcbox.h>
#include <wx/clrpicker.h>

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
	wxBitmapComboBox* shaderComboBox;
	wxColourPickerCtrl* bcgColorPicker;

	void RefreshControls();

	void onFileMenuClick(wxCommandEvent& evt);
	void onShowGrid(wxCommandEvent& evt);
	void onShowMesh(wxCommandEvent& evt);
	void onRotateSlider(wxCommandEvent& evt);
	void onChooseShader(wxCommandEvent& evt);
	void onChooseBcgColor(wxColourPickerEvent& evt);

	wxDECLARE_EVENT_TABLE();
};
