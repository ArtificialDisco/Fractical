#include <ctype.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
using namespace std;

#include "parameters.h"
#include "main.h"

string keywords[] = {
    "center_x", "center_y",
    "dx", "dy",
    "iterations",
    "zoom_factor",
    "move_length",
    "set_type",
    "color_scheme",
    "c_real", "c_imag",
    "z0_real", "z0_imag"
};
int nkeywords = 13;

void lowercase(char *s)
{
    for(int i = 0; s[i] != '\0'; i++)
        s[i] = tolower(s[i]);
}

bool is_keyword(string word)
{
    for(int i = 0; i < nkeywords; i++)
        if(keywords[i] == word)
            return TRUE;
    return FALSE;
}

void load_parameters_from_file(string filename, parameters *p)
{
    string keyword, value;

    ifstream file;
    file.open(filename);
    if (file.is_open()) {
        cout << "OPEN!" << endl;
        while (file >> keyword) {
            if (is_keyword(keyword)) {
                file >> value;
            } else {
                cout << "Error: parameter " << keyword << " is unkown" << endl;
                break;
            }

            if (keyword == "center_x") {
                p->center_x = stof(value);
            } else if (keyword == "center_y") {
                p->center_y = stof(value);
            } else if (keyword == "dx") {
                p->dx = stof(value);
            } else if (keyword == "dy") {
                p->dy = stof(value);
            } else if (keyword == "iterations") {
                p->iterations = stof(value);
            } else if (keyword == "zoom_factor") {
                p->zoom_factor = stof(value);
            } else if (keyword == "move_length") {
                p->move_length = stof(value);
            } else if (keyword == "c_real") {
                p->c_real = stof(value);
            } else if (keyword == "c_imag") {
                p->c_imag = stof(value);
            } else if (keyword == "z0_real") {
                p->z0_real = stof(value);
            } else if (keyword == "z0_imag") {
                p->z0_imag = stof(value);
            } else if (keyword == "set_type") {
                std::transform(value.begin(), value.end(), value.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                if (value == "mandelbrot") {
                    p->set_type = MANDELBROT;
                } else if (value == "julia") {
                    p->set_type = JULIA;
                } else {
                    cout << value << " is invalid for parameter 'set_type'"
                         << endl;
                }
            } else if (keyword == "color_scheme") {
                if (value == "dannis_favorite" || value == "rainbow" ||
                    value == "grayscale" || value == "bifurcating") {
                    p->color_scheme = value;
                } else {
                    cout << value << " is invalid for parameter 'color_scheme'\n"
                         << endl;
                }
            }
        }
    }
}

void save_parameters_to_file(string filename, parameters *p)
{
    FILE *file;

    if((file = fopen(filename.c_str(), "w")) == NULL) {
      fprintf(stderr, "error: can't open file '%s' for writing",
              filename.c_str());
      return;
    }

    fprintf(file, "center_x %.30g\n", p->center_x);
    fprintf(file, "center_y %.30g\n", p->center_y);
    fprintf(file, "dx %.30g\n", p->dx);
    fprintf(file, "dy %.30g\n", p->dy);

    fprintf(file, "iterations %i\n", p->iterations);
    fprintf(file, "zoom_factor %i\n", p->zoom_factor);
    fprintf(file, "move_length %i\n", p->move_length);

    fprintf(file, "set_type %s\n", (p->set_type == MANDELBROT) ? "mandelbrot"
        : "julia");
    fprintf(file, "color_scheme %s\n", p->color_scheme.c_str());

    fprintf(file, "c_real %.30g\n", p->c_real);
    fprintf(file, "c_imag %.30g\n", p->c_imag);
    fprintf(file, "z0_real %.30g\n", p->z0_real);
    fprintf(file, "z0_imag %.30g\n", p->z0_imag);
}

void copy_parameters(parameters *to, parameters from)
{
    to->center_x = from.center_x;
    to->center_y = from.center_y;
    to->dx = from.dx;
    to->dy = from.dy;

    to->iterations = from.iterations;
    to->zoom_factor = from.zoom_factor;
    to->move_length = from.move_length;

    to->set_type = from.set_type;
    to->color_scheme = from.color_scheme;

    to->c_real = from.c_real;
    to->c_imag = from.c_imag;
    to->z0_real = from.z0_real;
    to->z0_imag = from.z0_imag;
}
