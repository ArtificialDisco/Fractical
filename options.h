#include <wx/wx.h>
#include <wx/spinctrl.h>

#include <stdio.h>

#include "frame.h"

class OptionsWindow : public wxFrame
{
public:
	OptionsWindow(Frame *parent, const wxString &title, const wxPoint &pos,
		const wxSize &size, long style = wxDEFAULT_FRAME_STYLE);

	//variables
	int width, height;
	int xpos, ypos;

	fractal_type set_type;

	//objects
	wxSpinCtrl *iterations_spin;
	wxSpinCtrl *zoom_factor_spin;
	wxSpinCtrl *move_length_spin;
	wxRadioBox *radio_box;
	wxTextCtrl *c_real_text, *c_imag_text;
	wxTextCtrl *z0_real_text, *z0_imag_text;
	wxTextCtrl *center_x_text, *center_y_text;
	wxTextCtrl *dx_text, *dy_text;
	Frame *parent;

	//general functions
	int display_text(string, int, int);
	wxTextCtrl *entry_with_value(const wxPoint&, const wxSize&, double,
		string s = "%.30g");

	void Close();

	//signals
	void OnClose(wxCloseEvent& event);
	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnReset(wxCommandEvent& event);

	enum {
		ID_Ok,
		ID_Cancel,
		ID_Reset
	};

private:
	DECLARE_EVENT_TABLE()	
};
