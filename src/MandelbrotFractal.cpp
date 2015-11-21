#include "MandelbrotFractal.h"

#include <QtMath>

MandelbrotFractal::MandelbrotFractal()
{

}

int MandelbrotFractal::calcAtPoint(const double x0, const double y0) const
{
	long double vectorLength = 0;
	long double x = 0;
	long double y = 0;
	long double fi;
	long double vectorLengthToThePower;
	int iteration;
	for (iteration = 0; (iteration < maxIterationCount())&(vectorLength < radius()); iteration++) {
		vectorLength = qSqrt(x*x + y*y);
		fi = atan2(y, x);
		vectorLengthToThePower = pow(vectorLength, power());
		x = vectorLengthToThePower*(cos(fi*power())) + x0;
		y = vectorLengthToThePower*(sin(fi*power())) + y0;
	}
	return iteration;
}

