#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "complex.h"

void add_complex(double *r0, double *i0, double r1, double i1)
{
	*r0 += r1;
	*i0 += i1;
}

void multiply_complex(double *r0, double *i0, double r1, double i1)
{
	double itmp, rtmp;
	
	rtmp = *r0;
	
	*r0 *= r1;
	
	itmp = *i0 * r1;
	itmp += rtmp * i1;
	
	*r0 += -(*i0 * i1);
	*i0 = itmp;
}

void divide_complex(double *r0, double *i0, double r1, double i1)
{
	/*double t, d;
	double rtmp, itmp;

	t = r1 / i1;
	d = i1 * (1 + t*t);
	rtmp = (*r0*t + *i0) / d;
	itmp = (*i0*t - *r0) / d;

	*r0 = rtmp;
	*i0 = itmp;
	*/

	double real, imag;
	real = *r0*r1+*i0*i1;
	imag = *i0*r1+*r0*i1;

	*r0 = real / (r1*r1+i1*i1);
	*i0 = imag / (r1*r1+i1*i1);
}

void square_complex(double *r, double *i)
{
	double tmp = 2.0 * *r * *i;
	*r = (*r+*i) * (*r-*i);
	*i = tmp;
}

void sqrt_complex(double *r, double *i)
{

}

void sin_complex(double *r, double *i)
{
	double rtmp, itmp;
	
	rtmp = sin(*r) * cosh(*i);
	itmp = cos(*r) * sinh(*i);

	*r = rtmp;
	*i = itmp;
}

void sinh_complex(double *r, double *i)
{
	double rtmp, itmp;
	
	rtmp = sinh(*r) * cos(*i);
	itmp = cosh(*r) * sin(*i);

	*r = rtmp;
	*i = itmp;
}

void cos_complex(double *r, double *i)
{
	double rtmp, itmp;
	
	rtmp = cos(*r) * cosh(*i);
	itmp = -sin(*r) * sinh(*i);

	*r = rtmp;
	*i = itmp;
}

void cosh_complex(double *r, double *i)
{
	double rtmp, itmp;

	rtmp = cosh(*r) * cos(*i);
	itmp = sinh(*r) * sin(*i);

	*r = rtmp;
	*i = itmp;
}

void tan_complex(double *r, double *i)
{
	double r1, r2, i1, i2;

	r1 = r2 = *r;
	i1 = i2 = *i;

	cos_complex(&r1, &i1);
	sin_complex(&r2, &i2);

	divide_complex(&r1, &i1, r2, i2);

	*r = r1;
	*i = i1;
}

#ifdef COMPLEX_TRYOUT
int main()
{
	double r, i;
	char s[100];

	printf("enter real part: ");
	scanf("%s", s);
	r = atof(s);
	printf("enter imaginary part: ");
	scanf("%s", s);
	i = atof(s);

	
	divide_complex(&r, &i, 1, 0);
	printf("your number divided by (1+0i): %g+%gi\n", r, i);
	add_complex(&r, &i, r, i);
	printf("your number multiplied by 2: %g+%gi\n", r, i);
}
#endif //COMPLEX_TRYOUT
