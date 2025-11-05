#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <iostream>
#include <string>
using namespace std;

#include <wx/image.h>
#include <wx/wx.h>

#include "complex.h"
#include "parameters.h"

#define DEFAULT_ITERATIONS "100"
#define DEFAULT_ZOOM_FACTOR "2"
#define DEFAULT_MOVE_LENGTH "20"
#define DEFAULT_C_REAL_VALUE "-0.123"	//the "rabbit" fractal is the default
#define DEFAULT_C_IMAG_VALUE "0.745"  
#define DEFAULT_Z0_REAL_VALUE "0"	//z0 = 0+0i ensures a normal, healthy
#define DEFAULT_Z0_IMAG_VALUE "0"	//mandelbrot set :)
#define DEFAULT_DX "0.01"
#define DEFAULT_DY "0.01"
#define DEFAULT_CENTER_Y "0"
#define DEFAULT_MANDELBROT_CENTER_X "-0.5"
#define DEFAULT_JULIA_CENTER_X "-0.1"

class Frame : public wxFrame
{
public:
	Frame(const wxString &title, const wxPoint &pos, const wxSize &size);

	wxPen pens[255];

	wxMenuItem *color_menu_danni;
	wxMenuItem *color_menu_rainbow;
	wxMenuItem *color_menu_grayscale;
	wxMenuItem *color_menu_bifurcating;
	wxMenuItem *color_menu_testing;

	//variables:
	int width, height;
	int old_height, old_width;
	int xpos, ypos;
	int mouse_xpos, mouse_ypos;
	double x_values[2000];
	double y_values[2000];

	parameters params;
	vector<parameters> last_params;
	int nparam;
	int maxparam;

	//general functions:
	void menu_setup();
	void draw(wxDC *dc, bool restart = false);
	void init_animation(double, double, double, double, fractal_type);
	void calc_values();
	void calc_colors();
	void update_color_table();
	void update_buffer();
	void repaint(int, int, int, int, wxClientDC* = NULL);
	void pop_parameters();
	void push_parameters(parameters*);

    void Draw();
    void ZoomOut();
    void Stop() { stop = true; }
    void ClosePrefDialog() { pref_dialog_open = false; }

	//signal functions:
	void OnPaint(wxPaintEvent& event);
	void OnPaintCommand(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnClose(wxCommandEvent& event);

	void OnAbout(wxCommandEvent& event);
	void OnDraw(wxCommandEvent& event);
	
	void OnMouseLeftDown(wxMouseEvent &e);
	void OnMouseLeftUp(wxMouseEvent &e);
	void OnMouseMiddleDown(wxMouseEvent &e);
	void OnMouseRight(wxMouseEvent &e);
	void OnMouseMove(wxMouseEvent &e);
	
	void OnSaveImageDialog(wxCommandEvent& event);
	void OnSaveParametersDialog(wxCommandEvent& event);
	void OnLoadParameters(wxCommandEvent& event);

	void OnUndo(wxCommandEvent& event);
	void OnRedo(wxCommandEvent& event);

	void OnViewKeys(wxCommandEvent& event);
	void OnSelectMandelbrot(wxCommandEvent& event);
	void OnSelectJulia(wxCommandEvent& event);
	
	void OnColorDanni(wxCommandEvent& event);
	void OnColorRainbow(wxCommandEvent& event);
	void OnColorGrayscale(wxCommandEvent& event);
	void OnColorBifurcating(wxCommandEvent& event);
	void OnColorTesting(wxCommandEvent& event);
	
	void OnPrefDialog(wxCommandEvent& event);

	void OnMoveLeft(wxCommandEvent& event);
	void OnMoveRight(wxCommandEvent& event);
	void OnMoveUp(wxCommandEvent& event);
	void OnMoveDown(wxCommandEvent& event);
	void OnZoomIn(wxCommandEvent& event);
	void OnZoomOut(wxCommandEvent& event);

private:
    bool pref_dialog_open = false;
    bool running = false, stop = false;
    bool size_changed = false;
    bool left_mouse_down = false;

    string image_filename = "";
    string image_last_dir = "";
    string param_filename = "";
    string param_last_dir = "";

    //wx type variables:
    wxMemoryDC* buffer;
    wxBitmap* bitmap;
};

//enumeration table for signal functions:
enum {
	ID_None = 0,
	ID_Close = 1,
	ID_Exit,
	ID_About,
	
	ID_Draw,

	ID_SaveImage,
	ID_SaveParameters,
	ID_LoadParameters,

	ID_Undo,
	ID_Redo,

	ID_ViewKeys,
	ID_SelectMandelbrot,
	ID_SelectJulia,
	
	ID_ColorDanni,
	ID_ColorGrayscale,
	ID_ColorBifurcating,
	ID_ColorRainbow,
	ID_ColorTesting,
	
	ID_PrefDialog,
	
	ID_MoveLeft,
	ID_MoveRight,
	ID_MoveUp,
	ID_MoveDown,
	ID_ZoomIn,
	ID_ZoomOut,

	ID_Paint
};

#endif //FRAME_H

