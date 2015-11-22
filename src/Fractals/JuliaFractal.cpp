#include "JuliaFractal.h"

namespace
{
	const long double cPmin = -0.74356;
	const long double cQmin = 0.11135;
}

JuliaFractal::JuliaFractal(QObject *parent)
	: Fractal(parent)
{}

int JuliaFractal::calcAtPoint(const double x0, const double y0) const
{
	long double vectorLength = 0;
	auto x = x0;
	auto y = y0;
	int iteration;
	for (iteration = 0;(iteration < maxIterationCount())&(vectorLength < radius()); iteration++ ) {
		auto currentX = (x+y)*(x-y) + cPmin;
		auto currentY = x*y + x*y + cQmin;
		vectorLength = currentX*currentX + currentY*currentY;
		x = currentX;
		y = currentY;
	}
	return iteration;
}
