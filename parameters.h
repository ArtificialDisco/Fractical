#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
using namespace std;

enum { MANDELBROT, JULIA };
typedef int fractal_type;

enum { DANNIS_FAVORITE, RAINBOW, GRAYSCALE, BIFURCATING };
typedef int color_enum;

typedef struct {
    double center_x, center_y;
    double dx, dy;

    int iterations;
    int zoom_factor;
    int move_length;

    //enums:
    fractal_type set_type;
    string color_scheme;

    //variables for the equation 
    //  "z[n+1]=z[n]^2+C"
    double c_real, c_imag;
    double z0_real, z0_imag;
} parameters;

void lowercase(char*);
bool is_keyword(char* word);
void load_parameters_from_file(std::string, parameters*);
void save_parameters_to_file(std::string, parameters*);
void copy_parameters(parameters*, parameters);

#endif //PARAMETERS_H
