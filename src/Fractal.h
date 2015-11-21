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
    static const unsigned int MAX_ITERATION_COUNT = 10000;
    static const unsigned int MAX_MATRIX_DIMENSION = 10000;
    static const unsigned int MAX_RADIUS = 100;
    static const unsigned int MAX_POWER = 100;
    static const unsigned int MIN_ITERATION_COUNT = 0;
    static const unsigned int MIN_MATRIX_DIMENSION = 0;
    static const unsigned int MIN_RADIUS = 0;
    static const unsigned int MIN_POWER = 1;

    explicit Fractal(QObject *parent = 0);

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

	QImage image() const { return m_image; }

public slots:
    void calculate();
    void stopCalculcation();

signals:
    void calculateFinished();
    void progress(int progress);

private:
    void prepareToCalculate();
	void calcIterationMatrixLine(const int lineIndex);
    int calcIterationCountAtPoint(const double x0, const double y0) const;
	void printFractal();

    int m_maxIterationCount;
    int m_matrixDimension;
    double m_radius;
    double m_power;
    QRectF m_rectf;

    double m_stepX;
    double m_stepY;
    QVector<QVector<int> > m_iterationMatrix;
    QImage m_image;
    bool m_isCalculationRunning;

};

#endif // FRACTAL_H
