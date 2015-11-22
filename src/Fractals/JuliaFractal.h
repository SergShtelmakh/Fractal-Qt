#ifndef JULIAFRACTAL_H
#define JULIAFRACTAL_H

#include "Fractal.h"

class JuliaFractal : public Fractal
{
public:
	JuliaFractal(QObject *parent = 0);

private:
	int calcAtPoint(const double x0, const double y0) const;
};

#endif // JULIAFRACTAL_H
