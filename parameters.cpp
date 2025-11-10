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

void make_lowercase(string& s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}

void load_parameters_from_file(string filename, Parameters *p)
{
    string keyword, value;

    ifstream file;
    file.open(filename);
    if (file.is_open()) {
        while (file >> keyword) {
            if (is_keyword(keyword)) {
                file >> value;
            } else {
                cerr << "Error: parameter " << keyword << " is unkown" << endl;
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
                make_lowercase(value);
                if (value == "mandelbrot") {
                    p->set_type = MANDELBROT;
                } else if (value == "julia") {
                    p->set_type = JULIA;
                } else {
                    cerr << value << " is invalid for parameter 'set_type'"
                         << endl;
                }
            } else if (keyword == "color_scheme") {
                make_lowercase(value);
                if (value == "dannis_favorite" || value == "rainbow" ||
                    value == "grayscale" || value == "bifurcating") {
                    p->color_scheme = value;
                } else {
                    cerr << value << " is invalid for parameter 'color_scheme'\n"
                         << endl;
                }
            }
        }
    }
}

void save_parameters_to_file(string filename, Parameters *p)
{
    ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Error: Can't open file " << filename << " for writing."
             << endl;
        return;
    }

    file << "center_x " << p->center_x << endl;
    file << "center_y " << p->center_y << endl;
    file << "dx " << p->dx << endl;
    file << "dy " << p->dy << endl;

    file << "iterations " << p->iterations << endl;
    file << "zoom_factor " << p->zoom_factor << endl;
    file << "move_length " << p->move_length << endl;

    file << (p->set_type == MANDELBROT ? "mandelbrot" : "julia") << endl;
    file << "color_scheme " << p->color_scheme << endl;

    file << "c_real " << p->c_real << endl;
    file << "c_imag " << p->c_imag << endl;
    file << "z0_real " << p->z0_real << endl;
    file << "z0_imag " << p->z0_imag << endl;
}

void copy_parameters(Parameters *to, Parameters from)
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
