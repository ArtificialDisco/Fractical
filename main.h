#ifndef PAINT_H
#define PAINT_H

#include <wx/wx.h>

#include <stdio.h>
#include <stdlib.h>

#include "parameters.h"
#include "frame.h"

class App : public wxApp
{
	virtual bool OnInit();

public:
	double c_real, c_imag;
	double z0_real, z0_imag;
	fractal_type set_type;
};

#endif //PAINT_H
