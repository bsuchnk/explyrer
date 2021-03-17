#pragma once

#include "wx/wx.h"
#include "CanvasFrame.h"

class App : public wxApp
{
public:
    App() {  }

private:
	CanvasFrame* frame = nullptr;

public:
	virtual bool OnInit();

    // the given canvas.
    //TestGLContext& GetContext(wxGLCanvas* canvas);

    // virtual wxApp methods
    //virtual bool OnInit() wxOVERRIDE;
    //virtual int OnExit() wxOVERRIDE;

private:
    //TestGLContext* m_glContext;
};

wxDECLARE_APP(App);
