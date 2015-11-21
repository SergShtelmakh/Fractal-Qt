#ifndef MANDELBROTFRACTAL_H
#define MANDELBROTFRACTAL_H

#include "Fractal.h"

class MandelbrotFractal : public Fractal
{
public:
	MandelbrotFractal();

private:
	int calcAtPoint(const double x0, const double y0) const override;

};

#endif // MANDELBROTFRACTAL_H
