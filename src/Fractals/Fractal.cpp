#include "Fractal.h"

#include <QtConcurrent>
#include <QFuture>
#include <QColor>

namespace
{
	const int cDefaultIterationCount  = 1000;
	const int cDefaultMatrixDimension = 700;
	const double cDefaultRadius       = 10.0;
	const double cDefaultPower        = 2.0;
	const QRectF cDefaultRectF        = QRectF(-2.0, -2.0, 4.0, 4.0);

	const QColor cMainColor = QColor(Qt::black);

	inline QColor fracralColor(const double fraction)
	{
		return QColor(255 * fraction, 255 - 255*fraction, 0, 255);
	}

	inline int getProgress(const int current, const int max)
	{
		return static_cast<int>(current * 100.0 / max);
	}
}

Fractal::Fractal(QObject *parent)
	: QObject(parent)
	, m_maxIterationCount(cDefaultIterationCount)
	, m_matrixDimension(cDefaultMatrixDimension)
	, m_radius(cDefaultRadius)
	, m_power(cDefaultPower)
	, m_rectf(cDefaultRectF)
{}



void Fractal::startCalculation()
{
	m_isCalculationRunning = true;

	m_iterationMatrix.clear();
	m_stepX = (m_rectf.right()  - m_rectf.left())/m_matrixDimension;
	m_stepY = (m_rectf.bottom() - m_rectf.top()) /m_matrixDimension;

	emit calculateStarted();

	for (auto i = 0; i < m_matrixDimension; i++) {
		if (!m_isCalculationRunning) {
			emit calculateCanceled();
			return;
		}

		calculateMatrixLine(i);
		emit progress(getProgress(i + 1, m_matrixDimension));
	}

	printFractal();
	emit calculateFinished();
}

void Fractal::stopCalculcation()
{
	m_isCalculationRunning = false;
}

void Fractal::calculateMatrixLine(const int lineIndex)
{
	auto matrixLine = QVector<int>().fill(0, m_matrixDimension);
	auto base = matrixLine.data();

	auto result = QtConcurrent::map(matrixLine, [&lineIndex, &base, this](int &value) {
		auto currentX = m_rectf.left() + lineIndex*m_stepX;
		auto currentY = m_rectf.top() + (&value - base)*m_stepY;
		value = calcAtPoint(currentX, currentY);
	});
	result.waitForFinished();
	m_iterationMatrix.insert(lineIndex, matrixLine);
}

void Fractal::printFractal()
{
	auto getColor = [this](const int x, const int y) {
		auto fraction = static_cast<double>(m_iterationMatrix[x][y])/m_maxIterationCount;
		return qSqrt(fraction) == 1.0 ? cMainColor.rgb() : fracralColor(fraction).rgb();
	};

	auto image = QImage(m_matrixDimension, m_matrixDimension, QImage::Format_RGB32);
	for (auto i = 0; i < m_matrixDimension; i++) {
		for (auto j = 0; j < m_matrixDimension; j++) {
			image.setPixel(i, j, getColor(i, j));
		}
	}
	emit print(image);
}
