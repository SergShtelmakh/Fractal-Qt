#include "Fractal.h"

#include <QtConcurrent>
#include <QFuture>
#include <QColor>

#include <math.h>

namespace
{
	const QColor cMainColor = QColor(Qt::black);

	inline QColor fracralColor(const double fraction)
	{
		return QColor(255*fraction, 255-255*fraction, 0, 255);
	}
}

Fractal::Fractal(QObject *parent)
	: QObject(parent)
	, m_maxIterationCount(1000)
	, m_matrixDimension(700)
	, m_radius(10)
	, m_power(2)
	, m_rectf(QRectF(-2.0, -2.0, 4.0, 4.0))
{}

void Fractal::calculate()
{
	prepareToCalculate();

	for (int i = 0; (i < m_matrixDimension)&&(m_isCalculationRunning); i++) {
		calcIterationMatrixLine(i);
		emit progress(static_cast<int>((i + 1.0)*100.0/m_matrixDimension));
	}

	if (m_isCalculationRunning)
		printFractal();

	emit calculateFinished();
}

void Fractal::stopCalculcation()
{
	m_isCalculationRunning = false;
}

void Fractal::prepareToCalculate()
{
	m_iterationMatrix.clear();
	m_stepX = (m_rectf.right() - m_rectf.left())/m_matrixDimension;
	m_stepY = (m_rectf.bottom() - m_rectf.top())/m_matrixDimension;
	m_isCalculationRunning = true;
}

void Fractal::calcIterationMatrixLine(const int lineIndex)
{
	auto matrixLine = QVector<int>().fill(0, m_matrixDimension);
	int *base = matrixLine.data();

	auto result =  QtConcurrent::map(matrixLine, [&lineIndex, &base, this](int &value) {
		value = calcIterationCountAtPoint(m_rectf.left() + lineIndex*m_stepX, m_rectf.top() + (&value - base)*m_stepY);
	});
	result.waitForFinished();
	m_iterationMatrix.insert(lineIndex, matrixLine);
}

int Fractal::calcIterationCountAtPoint(const double x0, const double y0) const
{
	long double vectorLength = 0;
	long double x = 0;
	long double y = 0;
	long double fi;
	long double vectorLengthToThePower;
	int iteration;
	for (iteration = 0; (iteration < m_maxIterationCount)&(vectorLength < m_radius); iteration++) {
		vectorLength = qSqrt(x*x + y*y);
		fi = atan2(y, x);
		vectorLengthToThePower = pow(vectorLength, m_power);
		x = vectorLengthToThePower*(cos(fi*m_power)) + x0;
		y = vectorLengthToThePower*(sin(fi*m_power)) + y0;
	}
	return iteration;
}

void Fractal::printFractal()
{
	auto getColor = [this](const int x, const int y) {
		auto fraction = static_cast<double>(m_iterationMatrix[x][y])/m_maxIterationCount;
		return qSqrt(fraction) == 1.0 ? cMainColor : fracralColor(fraction);
	};

	auto image = QImage(m_matrixDimension, m_matrixDimension, QImage::Format_RGB32);
	for (int i = 0; i < m_matrixDimension; i++) {
		for (int j = 0; j < m_matrixDimension; j++) {
			image.setPixel(i, j, getColor(i, j).rgb());
		}
	}
	emit print(image);
}
