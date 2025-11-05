#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "frame.h"
#include "options.h"
#include "main.h"

OptionsWindow::OptionsWindow(Frame *parent, const wxString &title,
	const wxPoint &pos, const wxSize &size, long style)
  : wxFrame((wxFrame*)NULL, -1, title, pos, size, style)
{
	GetPosition(&xpos, &ypos);

	this->parent = parent;

	char value[100];
	int label_width, label_height;
	int side_len = 30;
	int button_len = 60;

	//set_type = parent->set_type;
	
	sprintf(value, "%i", parent->params.iterations);
	label_width = display_text("Iterations: ", side_len, 20);
	iterations_spin = new wxSpinCtrl(this, -1, value,
		wxPoint(side_len+80, 18), wxSize(100, 27));
	iterations_spin->SetRange(0, 10000);

	sprintf(value, "%i", parent->params.zoom_factor);
	label_width = display_text("Zoom factor: ", side_len, 52);
	zoom_factor_spin = new wxSpinCtrl(this, -1, value,
		wxPoint(side_len+80, 50), wxSize(100, 27));
	zoom_factor_spin->SetRange(1, 20);

	sprintf(value, "%i", parent->params.move_length);
	label_width = display_text("Move length: ", side_len, 84);
	move_length_spin = new wxSpinCtrl(this, -1, value,
		wxPoint(side_len+80, 82), wxSize(100, 27));
	move_length_spin->SetRange(1, 1000);

	display_text("Center:", side_len+200, 20);
	display_text("X: ", side_len+273, 20);
	center_x_text = entry_with_value(wxPoint(side_len+290, 16),
		wxSize(100, 23), parent->params.center_x);
	display_text("Y: ", side_len+400, 20);
	center_y_text = entry_with_value(wxPoint(side_len+275+140, 16),
		wxSize(100, 23), parent->params.center_y);

	display_text("Scale factor:", side_len+200, 52);
	display_text("X: ", side_len+273, 52);
	dx_text = entry_with_value(wxPoint(side_len+290, 48), wxSize(100, 23),
		parent->params.dx);
	display_text("Y: ", side_len+400, 52);
	dy_text = entry_with_value(wxPoint(side_len+275+140, 48), wxSize(100, 23),
		parent->params.dy);

	const wxString choices[] = { "Mandelbrot", "Julia" };
	radio_box = new wxRadioBox(this, -1, "Set type",
		wxPoint(side_len, 130), wxDefaultSize, 2, choices, 0,
		wxRA_SPECIFY_ROWS);
	radio_box->SetStringSelection((parent->params.set_type == MANDELBROT)
		? "Mandelbrot" : "Julia");

	label_width = display_text("Only for the Julia set:", side_len+27, 190);
	label_width = display_text("C = ", side_len, 215);
	c_real_text = entry_with_value(wxPoint(side_len+label_width, 212),
		wxSize(60, 23), parent->params.c_real, "%.6g");
	display_text("+", side_len+label_width+65, 215);
	c_imag_text = entry_with_value(wxPoint(side_len+label_width+75, 212),
		wxSize(60, 23), parent->params.c_imag, "%.6g");
	label_width = display_text(" i", 30+label_width+137, 215);

	label_width = display_text("Only for the Mandelbrot set:",side_len+25,250);
	label_width = display_text("z0 = ", side_len-3, 275);
	z0_real_text = entry_with_value(wxPoint(side_len+label_width-3, 272),
		wxSize(60, 23), parent->params.z0_real, "%.6g");
	display_text("+", side_len+label_width+62, 275);
	z0_imag_text = entry_with_value(wxPoint(side_len+label_width+72, 272),
		wxSize(60, 23), parent->params.z0_imag, "%.6g");
	label_width = display_text(" i", 30+label_width+134, 275);
	
	Fit();
	GetSize(&width, &height);

	wxButton *ok_button = new wxButton(this, ID_Ok, "Ok",
		wxPoint(side_len, height+20), wxSize(button_len, 27));
	wxButton *cancel_button = new wxButton(this, ID_Cancel, "Cancel",
		wxPoint(side_len+button_len+10, height+20),
		wxSize(button_len, 27));
	wxButton *reset_button = new wxButton(this, ID_Reset, "Reset",
		wxPoint(side_len+(button_len+10)*2, height+20),
		wxSize(button_len, 27));
	reset_button->SetDefault();
	cancel_button->SetDefault();
	ok_button->SetDefault();
	
	Fit();
	GetSize(&width, &height);
	SetSize(width+10, height+10);
	
	wxAcceleratorEntry entries[2];
	entries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, ID_Close);
	entries[1].Set(wxACCEL_NORMAL, WXK_RETURN, ID_Ok);
	wxAcceleratorTable accel(2, entries);
	SetAcceleratorTable(accel);

	Bind(wxEVT_CLOSE_WINDOW, &OptionsWindow::OnClose, this);
}

  void OptionsWindow::Close()
  {
	  parent->ClosePrefDialog();
	  radio_box->Destroy();
	  Destroy();
  }

BEGIN_EVENT_TABLE(OptionsWindow, wxFrame)
	/*EVT_CLOSE(OptionsWindow::OnClose)
	
	EVT_BUTTON(ID_Ok, OptionsWindow::OnOk)
	EVT_BUTTON(ID_Cancel, OptionsWindow::OnCancel)
	EVT_BUTTON(ID_Reset, OptionsWindow::OnReset)
	EVT_MENU(ID_Cancel, OptionsWindow::OnCancel)*/
END_EVENT_TABLE()

int OptionsWindow::display_text(string text, int x, int y) 
{
	wxStaticText *label = new wxStaticText(this, -1, text,
		wxPoint(x, y), wxDefaultSize, wxALIGN_LEFT);

	int w, h;
	label->GetSize(&w, &h);

	return w;
}

wxTextCtrl *OptionsWindow::entry_with_value(const wxPoint &pos, 
	const wxSize &size, double val, string format)
{
	char strval[100];
	sprintf(strval, format.c_str(), val);

	wxTextCtrl *entry = new wxTextCtrl(this, -1, strval, pos, size);

	return entry;
}

void OptionsWindow::OnClose(wxCloseEvent& event)
{
	Close();
	//delete this;
}

void OptionsWindow::OnOk()
{
	parent->params.iterations = iterations_spin->GetValue();
	parent->params.zoom_factor = zoom_factor_spin->GetValue();
	parent->params.move_length = move_length_spin->GetValue();
	
	set_type = (radio_box->GetStringSelection() == "Mandelbrot")
		? MANDELBROT : JULIA;
	parent->params.set_type = set_type;

	parent->params.center_x = str2num(center_x_text->GetValue().c_str());
	parent->params.center_y = str2num(center_y_text->GetValue().c_str());
	parent->params.dx = str2num(dx_text->GetValue().c_str());
	parent->params.dy = str2num(dy_text->GetValue().c_str());
	parent->params.c_real = str2num(c_real_text->GetValue().c_str());
	parent->params.c_imag = str2num(c_imag_text->GetValue().c_str());
	parent->params.z0_real = str2num(z0_real_text->GetValue().c_str());
	parent->params.z0_imag = str2num(z0_imag_text->GetValue().c_str());
	Close();

	parent->Stop();
}

void OptionsWindow::OnCancel()
{
	Close();
}

void OptionsWindow::OnReset()
{
	iterations_spin->SetValue(DEFAULT_ITERATIONS);
	zoom_factor_spin->SetValue(DEFAULT_ZOOM_FACTOR);
	move_length_spin->SetValue(DEFAULT_MOVE_LENGTH);
	
	if(radio_box->GetStringSelection() == "Mandelbrot")
		center_x_text->SetValue(DEFAULT_MANDELBROT_CENTER_X);
	else
		center_x_text->SetValue(DEFAULT_JULIA_CENTER_X);
	center_y_text->SetValue(DEFAULT_CENTER_Y);

	dx_text->SetValue(DEFAULT_DX);
	dy_text->SetValue(DEFAULT_DY);
	
	c_real_text->SetValue(DEFAULT_C_REAL_VALUE);
	c_imag_text->SetValue(DEFAULT_C_IMAG_VALUE);
	
	z0_real_text->SetValue(DEFAULT_Z0_REAL_VALUE);
	z0_imag_text->SetValue(DEFAULT_Z0_IMAG_VALUE);
}

#endif //OPTIONS_H
