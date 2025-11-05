#include <complex>

#include <cstring>
#include <math.h>
#include <stdio.h>

#include <wx/graphics.h>

#include "frame.h"
#include "options.h"
#include "main.h"

class App;
extern App a;

Frame::Frame(const wxString &title, const wxPoint &pos, const wxSize &size)
  : wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
    GetClientSize(&width, &height);
    old_width = 0, old_height = 0;
    xpos = 0, ypos = 0;

    strcpy(params.color_scheme, "dannis_favorite");

    params.iterations = atoi(DEFAULT_ITERATIONS);
    params.zoom_factor = atoi(DEFAULT_ZOOM_FACTOR);
    params.move_length  = atoi(DEFAULT_MOVE_LENGTH);

    nparam = maxparam = 0;

    SetBackgroundColour(wxColour("black"));
    //Clear();

    bitmap = new wxBitmap(width, height);
    buffer = new wxMemoryDC();
    buffer->SelectObject(*bitmap);
    buffer->SetDeviceOrigin(0, 0);
    const wxColour* black = wxBLACK;
    buffer->SetBackground(wxBrush(*black));
    buffer->Clear();

    menu_setup();

    wxAcceleratorEntry entries[10];
    entries[0].Set(wxACCEL_NORMAL, WXK_NUMPAD_ADD, ID_ZoomIn);
    entries[1].Set(wxACCEL_NORMAL, WXK_NUMPAD_SUBTRACT, ID_ZoomOut);
    entries[2].Set(wxACCEL_NORMAL, '+', ID_ZoomIn);
    entries[3].Set(wxACCEL_NORMAL, '-', ID_ZoomOut);
    entries[4].Set(wxACCEL_NORMAL, WXK_LEFT, ID_MoveLeft);
    entries[5].Set(wxACCEL_NORMAL, WXK_RIGHT, ID_MoveRight);
    entries[6].Set(wxACCEL_NORMAL, WXK_UP, ID_MoveUp);
    entries[7].Set(wxACCEL_NORMAL, WXK_DOWN, ID_MoveDown);
    entries[8].Set(wxACCEL_NORMAL, 'h', ID_Paint);
    entries[9].Set(wxACCEL_NORMAL, WXK_ESCAPE, ID_Exit);
    wxAcceleratorTable accel(10, entries);

    CreateStatusBar();
    SetAcceleratorTable(accel);
    SetStatusText("Don't panic!");
    GetStatusBar()->SetFocus();

    params.center_x = str2num(DEFAULT_MANDELBROT_CENTER_X);
    params.center_y = str2num(DEFAULT_CENTER_Y);
    params.dx = str2num(DEFAULT_DX);
    params.dy = str2num(DEFAULT_DY);
    
    calc_values();
    update_color_table();

    Bind(wxEVT_PAINT, &Frame::OnPaint, this, ID_Paint);
    Bind(wxEVT_MENU, &Frame::OnPaintCommand, this, ID_Paint);
    Bind(wxEVT_MENU, &Frame::OnDraw, this, ID_Draw);
    Bind(wxEVT_MENU, &Frame::OnClose, this, ID_Exit);
    Bind(wxEVT_SIZE, &Frame::OnSize, this);

    Bind(wxEVT_MENU, &Frame::OnAbout, this, ID_About);

    Bind(wxEVT_LEFT_DOWN, &Frame::OnMouseLeftDown, this);
    Bind(wxEVT_LEFT_UP, &Frame::OnMouseLeftUp, this);
    Bind(wxEVT_MIDDLE_DOWN, &Frame::OnMouseMiddleDown, this);
    Bind(wxEVT_RIGHT_DOWN, &Frame::OnMouseRight, this);
    Bind(wxEVT_MOTION, &Frame::OnMouseMove, this);

    Bind(wxEVT_MENU, &Frame::OnSaveImageDialog, this, ID_SaveImage);
    Bind(wxEVT_MENU, &Frame::OnSaveParametersDialog, this, ID_SaveParameters);
    Bind(wxEVT_MENU, &Frame::OnLoadParameters, this, ID_LoadParameters);

    Bind(wxEVT_MENU, &Frame::OnUndo, this, ID_Undo);
    Bind(wxEVT_MENU, &Frame::OnRedo, this, ID_Redo);

    Bind(wxEVT_MENU, &Frame::OnViewKeys, this, ID_ViewKeys);
    Bind(wxEVT_MENU, &Frame::OnSelectMandelbrot, this, ID_SelectMandelbrot);
    Bind(wxEVT_MENU, &Frame::OnSelectJulia, this, ID_SelectJulia);

    Bind(wxEVT_MENU, &Frame::OnColorDanni, this, ID_ColorDanni);
    Bind(wxEVT_MENU, &Frame::OnColorRainbow, this, ID_ColorRainbow);
    Bind(wxEVT_MENU, &Frame::OnColorGrayscale, this, ID_ColorGrayscale);
    Bind(wxEVT_MENU, &Frame::OnColorBifurcating, this, ID_ColorBifurcating);
    Bind(wxEVT_MENU, &Frame::OnColorTesting, this, ID_ColorTesting);

    Bind(wxEVT_MENU, &Frame::OnPrefDialog, this, ID_PrefDialog);

    Bind(wxEVT_MENU, &Frame::OnZoomIn, this, ID_ZoomIn);
    Bind(wxEVT_MENU, &Frame::OnZoomOut, this, ID_ZoomOut);
    Bind(wxEVT_MENU, &Frame::OnMoveLeft, this, ID_MoveLeft);
    Bind(wxEVT_MENU, &Frame::OnMoveRight, this, ID_MoveRight);
    Bind(wxEVT_MENU, &Frame::OnMoveUp, this, ID_MoveUp);
    Bind(wxEVT_MENU, &Frame::OnMoveDown, this, ID_MoveDown);
}

void Frame::menu_setup()
{
    wxMenuBar *menubar = new wxMenuBar;
    wxMenu *file = new wxMenu;
    file->Append(ID_Draw, "&Draw set\tALT-D", "Weee!", true);
    file->Append(ID_SaveImage, "&Save image\tALT-S",
        "Save image to a file, choose a file", true);
    file->Append(ID_SaveParameters, "Save parame&ters\tALT-T",
        "Save parameters to a file, choose a file", true);
    file->Append(ID_LoadParameters, "&Load parameters\tALT-L",
        "Load parameters from a file", true);
    file->AppendSeparator();
    file->Append(ID_Exit, "&Exit\tALT-X", "Don't touch this one!",
        wxITEM_NORMAL);
    menubar->Append(file, "&File");

    wxMenu *edit = new wxMenu;
    edit->Append(ID_Undo, "&Undo\tALT-Z", "Recall last", wxITEM_NORMAL);
    edit->Append(ID_Redo, "&Redo\tALT-R", "&Undo, the 'undo'", wxITEM_NORMAL);
    menubar->Append(edit, "&Edit");

    wxMenu *options = new wxMenu;
    options->Append(ID_ViewKeys, "&View key bindings\tALT-V",
        "Press, push, hit, strike, punch, tap or click", wxITEM_NORMAL);
    options->Append(ID_SelectMandelbrot, "&Mandelbrot set\tALT-M",
        "Switch to Mandelbrot", wxITEM_NORMAL);
    options->Append(ID_SelectJulia, "&Julia set\tALT-J",
        "Switch to Julia", wxITEM_NORMAL);

    wxMenu *color_menu = new wxMenu;
    color_menu_danni = new wxMenuItem(color_menu, ID_ColorDanni,
        "Danni's favorite", "My Favorite :)", wxITEM_RADIO);
    color_menu_rainbow = new wxMenuItem(color_menu, ID_ColorRainbow,
        "Rainbow", "Some annoyingly happy colors", wxITEM_RADIO);
    color_menu_grayscale = new wxMenuItem(color_menu, ID_ColorGrayscale,
        "Grayscale", "All kinds of different gray colors", wxITEM_RADIO);
    color_menu_bifurcating = new wxMenuItem(color_menu, ID_ColorBifurcating,
        "Bifurcating", "Notice the long bifurcating \"columns\" appear", wxITEM_RADIO);
    color_menu_testing = new wxMenuItem(color_menu, ID_ColorTesting,
        "Testing", "Testing...", wxITEM_RADIO);
    color_menu->Append(color_menu_danni);
    color_menu->Append(color_menu_rainbow);
    color_menu->Append(color_menu_grayscale);
    color_menu->Append(color_menu_bifurcating);
    color_menu->Append(color_menu_testing);

    options->Append(ID_None, "Color scheme", color_menu);
    options->AppendSeparator();
    options->Append(ID_PrefDialog, "&Preferences\tALT-P",
        "Make me please you!");
    menubar->Append(options, "&Options");
    
    wxMenu *help = new wxMenu;
    help->Append(ID_About, "A&bout\tALT-B", "Who da man?");
    menubar->Append(help, "&Help");
    SetMenuBar(menubar);
}

//process command line arguments
void Frame::init_animation(double c_real, double c_imag, double z0_real,
    double z0_imag, fractal_type set_type)
{
    this->params.c_real = c_real, this->params.c_imag = c_imag;
    this->params.z0_real = z0_real, this->params.z0_imag = z0_imag;
    this->params.set_type = set_type;
}

void Frame::calc_values()
{
    for(int i = 0; i < width; i++)
        x_values[i] = params.center_x - params.dx * width / 2.0 + params.dx * i;

    for (int i = 0; i < height; i++)
        y_values[i] = params.center_y - params.dy * height / 2.0 + params.dy * i;
}

void Frame::calc_colors()
{
    int r, g, b;
    r = g = b = 0;
    for(int i = 0; i < 255; i++) {

        if(!strcmp(params.color_scheme, "dannis_favorite")) {
            if(i < 11) {
                pens[i] = wxPen(wxColor(i * 15 + 100, 50, 40), 1, wxPENSTYLE_SOLID);
                continue;
            }
            r = g = b = i * 10;
            pens[i] = wxPen(wxColor(r + 120, g + 60, b - 20), 1, wxPENSTYLE_SOLID);
            
        } else if(!strcmp(params.color_scheme, "rainbow")) {
            r = g = b = i * 15;
            pens[i] = wxPen(wxColor(r + 0, g + 50, b + 150), 1, wxPENSTYLE_SOLID);
        
        } else if(!strcmp(params.color_scheme, "grayscale")) {
            pens[i] = wxPen(wxColor(i*10+50, i*10+50, i*10+50), 1, wxPENSTYLE_SOLID);
        } else if(!strcmp(params.color_scheme, "bifurcating")) {
            int rgb = i*i*i*i;

            r = (rgb >> 16) & 0xff;
            g = (rgb >> 8) & 0xff;
            b = rgb & 0xff;

            pens[i] = wxPen(wxColor(r, g, b), 1, wxPENSTYLE_SOLID);
        } else {
            if(i == params.iterations)
                pens[i] = wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_SOLID);
            else
                pens[i] = wxPen(wxColor(0xff, 0xff, 0xff), 1, wxPENSTYLE_SOLID);
        }
    }
}

void Frame::update_color_table()
{
    calc_colors();

    color_menu_danni->Check(
        !strcmp(params.color_scheme, "dannis_favorite"));
    color_menu_rainbow->Check(!strcmp(params.color_scheme, "rainbow"));
    color_menu_grayscale->Check(!strcmp(params.color_scheme, "grayscale"));
    color_menu_bifurcating->Check(!strcmp(params.color_scheme, "bifurcating"));
    color_menu_testing->Check(!strcmp(params.color_scheme, "testing"));
}

void Frame::update_buffer()
{
    if(size_changed) {
        delete bitmap;
        bitmap = new wxBitmap(width, height);
        buffer->SelectObject(*bitmap);
        buffer->SetDeviceOrigin(0, 0);
        const wxColour* black = wxBLACK;
        buffer->SetBackground(wxBrush(*black));
    }
    
    calc_values();
    calc_colors();
    
    buffer->Clear();
}

void Frame::draw(wxDC *dc, bool restart)
{
    double cr, ci;

    if(size_changed)
        GetClientSize(&width, &height);

    update_buffer();
    dc->Clear();
        
    size_changed = FALSE;

    clock_t c = clock();

    int it;
    double zr = params.z0_real, zr2, zi = params.z0_imag, zi2;
    double tmp;

    if(params.set_type == JULIA)
        cr = params.c_real, ci = params.c_imag;

    running = TRUE;
    for(int ypos = 0; ypos < height && running; ypos++) {
        for(int xpos = 0; xpos < width; xpos++) {
            if(params.set_type == MANDELBROT)
                cr = x_values[xpos], ci = y_values[ypos];
            else
                zr = x_values[xpos], zi = y_values[ypos];

            zr2 = zr*zr, zi2=zi*zi;
            for(it = 0; zr2+zi2 < 4.0 && it < params.iterations; it++) {

                //z[n+1] = z[n]^2+C
                tmp = 2.0 * zr * zi + ci;
                zr = (zr2=zr*zr) - (zi2=zi*zi) + cr;
                zi = tmp;

                //cos_complex(&zr, &zi);
                //multiply_complex(&zr, &zi, zr, zi);
                //add_complex(&zr, &zi, cr, ci);
            }

            if(params.set_type == MANDELBROT)
                zr = params.z0_real, zi = params.z0_imag;

            if(it < params.iterations)
                buffer->SetPen(pens[it % 255]);
            else
                buffer->SetPen(*wxBLACK_PEN);

            buffer->DrawPoint(xpos, ypos);
        }
        while(a.Pending())
            a.Dispatch();

        dc->Blit(0, ypos, width, 1, buffer, 0, ypos);

        if(stop) { running = stop = FALSE; draw(dc); return; }
    }
    running = FALSE;

    char status_text[50];
    sprintf(status_text, "time taken: %g seconds",
        (float)(clock() - c) / 1000000.0f);
    SetStatusText(status_text);
}

//windows doesn't handle OnPaint, make an extra
void Frame::repaint(int x1, int y1, int x2, int y2, wxClientDC *dc)
{	
    bool create_dc = FALSE;
    if(dc == NULL) {
        dc = new wxClientDC(this);
        create_dc = TRUE;
    }
    
    dc->Blit(x1, y1, x2, y2, buffer, x1, y1);

    if(create_dc) delete dc;
}

void Frame::pop_parameters()
{	
    copy_parameters(&params, last_params[(--nparam)-1]);
}

void Frame::push_parameters(parameters *p)
{
    last_params.push_back(*p);
    //copy_parameters(&last_p[nparam++], *p);
    nparam++;
}

void Frame::OnPaint(wxPaintEvent& event)
{	
    wxPaintDC dc(this);
    dc.Blit(0, 0, width, height, buffer, 0, 0);
}

void Frame::OnPaintCommand(wxCommandEvent& event)
{	
    wxPaintDC dc(this);
    dc.Blit(0, 0, width, height, buffer, 0, 0);
}

void Frame::OnSize(wxSizeEvent& event)
{
    size_changed = TRUE;
}

void Frame::OnClose(wxCommandEvent& event)
{
    exit(0);
}

void Frame::OnDraw(wxCommandEvent& event)
{
    Draw();
}

void Frame::Draw()
{
    std::cout << "Draw" << std::endl;
    push_parameters(&params);
    
    if(running) {
        stop = TRUE;
        return;
    }
    
    maxparam = nparam;
    
    calc_values();
    calc_colors();
    
    wxClientDC dc(this);
    const wxColour* black = wxBLACK;
    dc.SetBackground(wxBrush(*black));
    dc.Clear();
    dc.SetDeviceOrigin(0, 0);
    
    draw(&dc);
}

void Frame::OnSaveImageDialog(wxCommandEvent& event)
{
    wxFileDialog d(this, "Save image as...",
        image_last_dir.c_str(), "", "Save by extension|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(d.ShowModal() == wxID_OK) {
        image_filename = d.GetPath();
        image_last_dir = d.GetDirectory();
    }

    wxInitAllImageHandlers();

    wxBitmapType type = wxBITMAP_TYPE_BMP;
    string extension;
    int i;
    
    int len = image_filename.size();

    if(!len) return;
    
    for(i = 0; i < len && image_filename[len-i] != '.'; i++)
        ;
    extension = image_filename.substr(len - i, i);

    if(extension == ".png")
        type = wxBITMAP_TYPE_PNG;
    else if(extension == ".jpg" || extension == ".jpeg")
        type = wxBITMAP_TYPE_JPEG;
    else if(extension == ".bmp")
        type = wxBITMAP_TYPE_BMP;
    else if(extension == ".gif") {
        wxMessageBox("Gif doesn't work (shame on you)!\n"
        "using png instead", "png rules!", wxOK | wxICON_EXCLAMATION);
        type = wxBITMAP_TYPE_PNG;

        image_filename[len-3] = 'p';
        image_filename[len-2] = 'n';
        image_filename[len-1] = 'g';
    }
    bitmap->SaveFile(image_filename, type);
}

void Frame::OnSaveParametersDialog(wxCommandEvent& event)
{
    wxFileDialog d(this, "Save parameters as...",
        image_last_dir.c_str(), "", "None|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(d.ShowModal() == wxID_OK) {
        param_filename = d.GetPath();
        param_last_dir = d.GetDirectory();
        save_parameters_to_file(param_filename, &params);
    }
}

void Frame::OnLoadParameters(wxCommandEvent& event)
{
    wxFileDialog d(this, "Load parameters...",
        image_last_dir.c_str(), "", "None|*", wxFD_OPEN);

    if(d.ShowModal() == wxID_OK) {
        param_filename = d.GetPath();
        param_last_dir = d.GetDirectory();
        load_parameters_from_file(param_filename, &params);
    }
    update_color_table();
}

void Frame::OnUndo(wxCommandEvent& event)
{
    if(nparam -1 > 0) {
        pop_parameters();
        if(running) {
            stop = TRUE;
            return;
        }
        wxClientDC dc(this);
        const wxColour* black = wxBLACK;
        dc.SetBackground(wxBrush(*black));
        dc.Clear();
        dc.SetDeviceOrigin(0, 0);
        draw(&dc);
        update_color_table();
    }
}

void Frame::OnRedo(wxCommandEvent& event)
{
    if(nparam < maxparam) {
        //push_parameters(&last_p[nparam]);
        copy_parameters(&params, last_params[nparam++]);
        
        if(running) {
            stop = TRUE;
            return;
        }
        wxClientDC dc(this);
        const wxColour* black = wxBLACK;
        dc.SetBackground(wxBrush(*black));
        dc.Clear();
        dc.SetDeviceOrigin(0, 0);
        draw(&dc);
        update_color_table();
    }
}

void Frame::OnViewKeys(wxCommandEvent& event)
{
    int xpos, ypos;
    GetPosition(&xpos, &ypos);
    
    wxMessageDialog d(this,
        "Key bindings:\n\n"
        "+: zoom in\n"
        "-: zoom out\n"
        "Left-Mouse: Center view\n"
        "    or rectangle zoom\n"
        "Right-Mouse: Center and Zoom out\n"
        "Middle-mouse: Rotate set around a plane (X*Y)\n"
        "Arrow up: move up\n"
        "Arrow down: move down\n"
        "Arrow left: move left\n"
        "Arrow right: move right\n"
        "Ctrl-M: Select Mandelbrot set\n"
        "Ctrl-J: Select julia set\n"
        "ALT-D: Draw set\n"
        "ALT-S: Save set as an image\n"
        "ALT-E: Save parameters to a file\n"
        "ALT-L: Load parameters\n"
        "ALT-Z: Undo\n"
        "ALT-R: Redo\n"
        "ALT-P: Preferences\n"
        "ALT-B: Who da man?\n"
        "ALT-X: Quit --don't even think about it!\n"
        "ALT-V: View this dialog",
        "Shortcut keys", wxOK);

    d.SetSize(xpos+50, ypos+50, 300, 0);
    d.ShowModal();
}

//set types:
void Frame::OnSelectMandelbrot(wxCommandEvent& event)
{
    if(params.set_type == JULIA) {
        params.set_type = MANDELBROT;
        params.center_x -= 0.4;
    }
    Draw();
}

void Frame::OnSelectJulia(wxCommandEvent& event)
{
    if(params.set_type == MANDELBROT) {
        params.set_type = JULIA;
        params.center_x += 0.4;
    }
    Draw();
}

//color schemes:
void Frame::OnColorDanni(wxCommandEvent& event)
{
    if(strcmp(params.color_scheme, "dannis_favorite"))
        strcpy(params.color_scheme, "dannis_favorite");
    update_color_table();
}

void Frame::OnColorRainbow(wxCommandEvent& event)
{
    if(strcmp(params.color_scheme, "rainbow"))
        strcpy(params.color_scheme, "rainbow");
    update_color_table();
}

void Frame::OnColorGrayscale(wxCommandEvent& event)
{
    if(strcmp(params.color_scheme, "grayscale"))
        strcpy(params.color_scheme, "grayscale");
    update_color_table();
}

void Frame::OnColorBifurcating(wxCommandEvent& event)
{
    if(strcmp(params.color_scheme, "bifurcating"))
        strcpy(params.color_scheme, "bifurcating");
    update_color_table();
}

void Frame::OnColorTesting(wxCommandEvent& event)
{
    if(strcmp(params.color_scheme, "testing"))
        strcpy(params.color_scheme, "testing");
    update_color_table();
}

void Frame::OnPrefDialog(wxCommandEvent& event)
{
    if(pref_dialog_open)
        return;

    pref_dialog_open = TRUE;

    int x, y;
    GetPosition(&x, &y);

    OptionsWindow *options_window = new OptionsWindow(this, "Options",
        wxPoint(x+50, y+60), wxDefaultSize);
    options_window->Show();
}

void Frame::OnAbout(wxCommandEvent& event)
{
    int xpos, ypos;
    GetPosition(&xpos, &ypos);
    
    wxMessageDialog d(this, "A fractal drawing program.\n"
        "by Danni (artificialdisco@gmail.com)", "Fractical", 
        wxOK | wxICON_INFORMATION, wxPoint(xpos+50, ypos+50));

    d.ShowModal();
}

//mouse functions:
void Frame::OnMouseLeftDown(wxMouseEvent &e)
{
    left_mouse_down = TRUE;
    mouse_xpos = e.GetX(), mouse_ypos = e.GetY();
}

void Frame::OnMouseLeftUp(wxMouseEvent &e)
{
    int x = e.GetX(), y = e.GetY();

    if(left_mouse_down && x == mouse_xpos && y == mouse_ypos) {
        params.center_x -= (width/2.0) * params.dx - e.GetX() * params.dx;
        params.center_y -= (height/2.0) * params.dy - e.GetY() * params.dy;
    } else {
          params.center_x -=
              (width / 2.0) * params.dx - ((mouse_xpos + x) / 2.0) * params.dx;
          params.center_y -=
              (height / 2.0) * params.dy - ((mouse_ypos + y) / 2.0) * params.dy;

          int dist, xd, yd;

          xd = abs(mouse_xpos - x);
          if (xd == 0)
            xd = 1;
          xd = width / xd;

          yd = abs(mouse_xpos - y);
          if (yd == 0)
            yd = 1;
          yd = height / yd;

          dist = (xd > yd) ? xd : yd;
          params.dx /= dist, params.dy /= dist;
    }

    left_mouse_down = FALSE;
    buffer->Clear();
    Draw();
}

void Frame::OnMouseMiddleDown(wxMouseEvent &e)
{
    if(params.set_type == MANDELBROT) {
        params.set_type = JULIA;
        params.c_real = x_values[e.GetX()];
        params.c_imag = y_values[e.GetY()];
    } else {
        params.set_type = MANDELBROT;
        params.z0_real = x_values[e.GetX()];
        params.z0_imag = y_values[e.GetY()];
    }
    
    Draw();
}

void Frame::OnMouseRight(wxMouseEvent &e)
{
    params.center_x -= (width/2.0) * params.dx - e.GetX() * params.dx;
    params.center_y -= (height/2.0) * params.dy - e.GetY() * params.dy;
    ZoomOut();
}

void Frame::OnMouseMove(wxMouseEvent &e)
{
    if(left_mouse_down) {
        wxClientDC dc(this);
        dc.SetPen(*wxWHITE_PEN);

        static int x = e.GetX(), y = e.GetY();

        repaint(0, 0, width, height, &dc);
        
        x = e.GetX(), y = e.GetY();
    
        dc.DrawLine(mouse_xpos, mouse_ypos, x, mouse_ypos);
        dc.DrawLine(x, mouse_ypos, x, y);
        dc.DrawLine(x, y, mouse_xpos, y);
        dc.DrawLine(mouse_xpos, y, mouse_xpos, mouse_ypos);
    }
}

//zooming and moving the view:
void Frame::OnZoomIn(wxCommandEvent& event)
{
    params.dx /= params.zoom_factor;
    params.dy /= params.zoom_factor;
    calc_values();
    buffer->Clear();
    Draw();
}

void Frame::ZoomOut()
{
    params.dx *= params.zoom_factor;
    params.dy *= params.zoom_factor;
    calc_values();
    buffer->Clear();
    Draw();
}

void Frame::OnZoomOut(wxCommandEvent& event)
{
    ZoomOut();
}

void Frame::OnMoveLeft(wxCommandEvent& event)
{
    params.center_x -= params.move_length*params.dx;
    calc_values();
    buffer->Clear();
    Draw();
}

void Frame::OnMoveRight(wxCommandEvent& event)
{
    params.center_x += params.move_length*params.dx;
    calc_values();
    buffer->Clear();
    Draw();
}

void Frame::OnMoveUp(wxCommandEvent& event)
{
    params.center_y -= params.move_length*params.dy;
    calc_values();
    buffer->Clear();
    Draw();
}

void Frame::OnMoveDown(wxCommandEvent& event)
{
    params.center_y += params.move_length*params.dy;
    calc_values();
    buffer->Clear();
    Draw();
}

