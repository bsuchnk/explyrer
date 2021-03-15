#include "CanvasFrame.h"
#include "MyGLCanvas.h"
#include <wx/event.h>
#include "App.h"



#define ID_SHOW_GRID 10001
#define ID_ROTATE_SLIDER 10002
#define ID_SHOW_MESH 10005

wxBEGIN_EVENT_TABLE(CanvasFrame, wxFrame)
EVT_CHECKBOX(ID_SHOW_GRID, onShowGrid)
EVT_CHECKBOX(ID_SHOW_MESH, onShowMesh)
wxEND_EVENT_TABLE()

CanvasFrame::CanvasFrame() : wxFrame(nullptr, wxID_ANY, "exPLYrer", wxDefaultPosition, wxSize(720, 720))
{
	menubar = new wxMenuBar();
	menu_file = new wxMenu();
	menu_file->Append(wxID_OPEN);
	menu_file->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CanvasFrame::onFileMenuClick), NULL, this);

	menubar->Append(menu_file, "&File");
	SetMenuBar(menubar);

	wxPanel* panel = new wxPanel(this, wxID_ANY);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* glhoriz = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* btnhoriz = new wxBoxSizer(wxHORIZONTAL);
	
	showGrid = new wxCheckBox(panel, ID_SHOW_GRID, "Grid");
	showGrid->SetValue(true);
	showMesh = new wxCheckBox(panel, ID_SHOW_MESH, "Mesh");
	showMesh->SetValue(true);
	
	wxArrayString shaderChoices;
	wxArrayPtrVoid shaderBmps;
	shaderChoices.Add("Rainbow");
	shaderChoices.Add("Gradient");
	shaderBmps.Add(NULL);
	shaderBmps.Add(NULL);
	shaderComboBox = new wxBitmapComboBox(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	for (int i = 0; i < shaderChoices.GetCount(); i++)
		shaderComboBox->Append(shaderChoices[i]);

	xRotateSlider = new wxSlider(panel, ID_ROTATE_SLIDER, 180, 0, 359);
	yRotateSlider = new wxSlider(panel, ID_ROTATE_SLIDER+1, 180, 0, 359);
	zRotateSlider = new wxSlider(panel, ID_ROTATE_SLIDER+2, 180, 0, 359);
	xRotateSlider->Bind(wxEVT_SLIDER, &CanvasFrame::onRotateSlider, this);
	yRotateSlider->Bind(wxEVT_SLIDER, &CanvasFrame::onRotateSlider, this);
	zRotateSlider->Bind(wxEVT_SLIDER, &CanvasFrame::onRotateSlider, this);

	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 24, 0 };
	myglcanvas = new MyGLCanvas(panel, args);

	sizer->Add(btnhoriz);
	btnhoriz->Add(showGrid, 0, wxCENTER);
	btnhoriz->Add(showMesh, 0, wxCENTER);
	btnhoriz->Add(shaderComboBox, 0, wxCENTER);
	sizer->Add(xRotateSlider, 0, wxEXPAND);
	sizer->Add(yRotateSlider, 0, wxEXPAND);
	sizer->Add(zRotateSlider, 0, wxEXPAND);
	sizer->Add(glhoriz, 1, wxEXPAND);
	glhoriz->Add(myglcanvas, 1, wxEXPAND);

	panel->SetSizer(sizer);
	Centre();
}


void CanvasFrame::RefreshControls()
{
	xRotateSlider->SetValue(180);
	yRotateSlider->SetValue(180);
	zRotateSlider->SetValue(180);
}


void CanvasFrame::onFileMenuClick(wxCommandEvent& evt)
{
	//MyGLContext& context = wxGetApp().GetContext(myglcanvas);
	//wxMessageBox(wxString("elo"));
	
	//context.x = 69;

	wxFileDialog* file_dialog = new wxFileDialog(
		this, "Choose a file", "", "",
		"ply files (*.ply)|*.ply",
		wxFD_OPEN
	);
	if (file_dialog->ShowModal() == wxID_OK) {
		myglcanvas->destroyModel();
		myglcanvas->model = Model();
		myglcanvas->loadModel(file_dialog->GetPath().c_str().AsChar());
		RefreshControls();
		Refresh(false);// <- rysuje od nowa
	}

	//context.SetCurrent(*myglcanvas);
	//context.draw();
}

void CanvasFrame::onShowGrid(wxCommandEvent & evt)
{
	myglcanvas->isShowingGrid = showGrid->GetValue();
	myglcanvas->Refresh();
}

void CanvasFrame::onShowMesh(wxCommandEvent & evt)
{
	myglcanvas->isShowingMesh = showMesh->GetValue();
	myglcanvas->Refresh();
}

void CanvasFrame::onRotateSlider(wxCommandEvent & evt)
{
	wxSlider* obj = (wxSlider*)evt.GetEventObject();
	int id = evt.GetId() - ID_ROTATE_SLIDER;
	float* rotation = NULL;
	switch (id) {
		case 0: rotation = &myglcanvas->model.xRotation; break;
		case 1: rotation = &myglcanvas->model.yRotation; break;
		case 2: rotation = &myglcanvas->model.zRotation; break;
	}
	if (!rotation) {
		wxMessageBox("Wrong Slider ID");
		return;
	}

	*rotation = (obj->GetValue() + 180) / 180.f * M_PI;
	if (*rotation >= M_PI*2.f) {
		*rotation -= M_PI*2.f;
	}

	myglcanvas->Refresh();
}
