#include "CanvasFrame.h"
#include "MyGLContext.h"
#include "MyGLCanvas.h"
#include <wx/event.h>
#include "App.h"

wxBEGIN_EVENT_TABLE(CanvasFrame, wxFrame)
//EVT_BUTTON(10001, OnButtonClicked)
//EVT_BUTTON(10002, OnButtonClicked)
wxEND_EVENT_TABLE()

CanvasFrame::CanvasFrame() : wxFrame(nullptr, wxID_ANY, "exPLYrer", wxDefaultPosition, wxSize(720, 720))
{
	menubar = new wxMenuBar();
	menu_file = new wxMenu();
	menu_file->Append(wxID_OPEN);
	menu_file->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CanvasFrame::onFileMenuClick), NULL, this);

	menubar->Append(menu_file, "&File");
	SetMenuBar(menubar);

	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 24, 0 };
	myglcanvas = new MyGLCanvas(this, args);
	Centre();
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
		Refresh(false);// <- rysuje od nowa
	}

	//context.SetCurrent(*myglcanvas);
	//context.draw();
}
