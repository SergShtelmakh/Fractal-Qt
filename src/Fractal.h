/*!
 * \file Fractal.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef FRACTAL_H
#define FRACTAL_H

#include <QObject>
#include <QVector>
#include <QRectF>
#include <QImage>

class Fractal: public QObject
{
	Q_OBJECT

public:
	explicit Fractal(QObject *parent = 0);
	virtual ~Fractal(){}

	int maxIterationCount() const {	return m_maxIterationCount;	}
	void setMaxIterationCount(const int maxIterationCount) { m_maxIterationCount = maxIterationCount; }

	int matrixDimension() const { return m_matrixDimension;	}
	void setMatrixDimension(const int matrixDimension) { m_matrixDimension = matrixDimension; }

	double radius() const { return m_radius; }
	void setRadius(const double radius) { m_radius = radius; }

	double power() const { return m_power; }
	void setPower(const double power) {	m_power = power; }

	QRectF rectf() const { return m_rectf; }
	void setRect(const QRectF &rectf) { m_rectf = rectf; }

public slots:
	void startCalculation();
	void stopCalculcation();

signals:
	void calculateFinished();
	void calculateCanceled();
	void progress(int progress);
	void print(QImage image);

protected:
	virtual int calcAtPoint(const double x0, const double y0) const = 0 ;

private:
	void calculateMatrixLine(const int lineIndex);
	void printFractal();

	int    m_maxIterationCount;
	int    m_matrixDimension;
	double m_radius;
	double m_power;
	QRectF m_rectf;

	double m_stepX;
	double m_stepY;
	QVector<QVector<int>> m_iterationMatrix;
	bool m_isCalculationRunning;
};

#endif // FRACTAL_H
