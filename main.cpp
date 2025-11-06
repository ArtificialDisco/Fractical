#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
using namespace std;

#include "main.h"

App app;

//IMPLEMENT_APP(App)

bool App::OnInit()
{
	Frame *main_frame = new Frame("A fractal program", wxPoint(100, 100),
		wxSize(800, 600));
	main_frame->Show(TRUE);
	SetTopWindow(main_frame);

#	ifdef __WINDOWS__
	app.c_real = std::stof(DEFAULT_C_REAL_VALUE);
	app.c_imag = std::stof(DEFAULT_C_IMAG_VALUE);
	app.z0_real = std::stof(DEFAULT_Z0_REAL_VALUE);
	app.z0_imag = std::stof(DEFAULT_Z0_IMAG_VALUE);
	app.set_type = MANDELBROT;
#	endif
	
	main_frame->init_animation(c_real, c_imag, z0_real, z0_imag, set_type);
	
	load_parameters_from_file(".config", &main_frame->params);
	main_frame->update_color_table();
	
	return TRUE;
}

void get_complex(double *r, double *im, char *s)
{
	char tmp[20];

	int i, j;

	for(i = 0; s[i] != '+' && i < 20; i++)
		tmp[i] = s[i];
	tmp[i] = '\0';

	*r = atof(tmp);

	tmp[0] = '\0';
	i++;

	for(j = 0; s[i] != 'i' && j < 20; i++, j++)
		tmp[j] = s[i];
	tmp[j] = '\0';

	*im = atof(tmp);
}

#ifndef __WINDOWS__
int main(int argc, char *argv[])
{
	app.c_real = std::stof(DEFAULT_C_REAL_VALUE);
	app.c_imag = std::stof(DEFAULT_C_IMAG_VALUE);
	app.z0_real = std::stof(DEFAULT_Z0_REAL_VALUE);
	app.z0_imag = std::stof(DEFAULT_Z0_IMAG_VALUE);
	app.set_type = MANDELBROT;
	
	int tmp;
	char s[100];
	for(tmp = 1; tmp < argc; tmp++) {
		int i;
		for(i = 2; argv[tmp][i] != '\0'; i++)
			s[i-2] = argv[tmp][i];
		s[i-2] = '\0';
		
		if(argv[tmp][1] == '=') {
			if(argv[tmp][0] == 'c' || argv[tmp][0] == 'C')
				get_complex(&app.c_real, &app.c_imag, s);
			else if(argv[tmp][0] == 'z' || argv[tmp][0] == 'Z')
				get_complex(&app.z0_real, &app.z0_imag, s);
		} else if(argv[tmp][0] == '-') {

			if(!strlen(s) && tmp < argc-1)
				strcpy(s, argv[tmp+1]);
		
			if(argv[tmp][1] == 't')
				if(!strcmp(s, "mandelbrot"))
					app.set_type = MANDELBROT;
				else if(!strcmp(s, "julia"))
					app.set_type = JULIA;
				else
					printf("-t must be followed by 'julia' or 'mandelbrot\n"
						"setting type to mandelbrot\n\n");
		}
	}

	return wxEntry(argc, argv);
}
#endif //__WINDOWS__

#ifdef __WINDOWS__
wxIMPLEMENT_APP(App);
#endif //__WINDOWS__
