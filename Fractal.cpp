#include "Fractal.h"

#include <math.h>
#include <QImage>
#include <QtConcurrent>
#include <QFuture>

Fractal::Fractal(QObject *parent) :
    QObject(parent),
    m_maxIterationCount(1000),
    m_matrixDimension(700),
    m_radius(10),
    m_power(2),
    m_rectf(QRectF(-2.0, -2.0, 4.0, 4.0))
{}

int Fractal::maxIterationCount()
{
    return m_maxIterationCount;
}

void Fractal::setMaxIterationCount(int maxIterationCount)
{
    m_maxIterationCount = maxIterationCount;
}

int Fractal::matrixDimension() const
{
    return m_matrixDimension;
}

void Fractal::setMatrixDimension(int matrixDimension)
{
    m_matrixDimension = matrixDimension;
}

double Fractal::radius() const
{
    return m_radius;
}

void Fractal::setRadius(double radius)
{
    m_radius = radius;
}

double Fractal::power() const
{
    return m_power;
}

void Fractal::setPower(double power)
{
    m_power = power;
}

QRectF Fractal::rectf() const
{
    return m_rectf;
}

void Fractal::setRect(const QRectF &rect)
{
    m_rectf = rect;
}

QImage Fractal::image() const
{
    return m_image;
}

void Fractal::calculate()
{
    m_isCalculationRunning = true;
    this->prepareMatrix();
    m_stepX = (m_rectf.right() - m_rectf.left()) / m_matrixDimension;
    m_stepY = (m_rectf.bottom() - m_rectf.top()) / m_matrixDimension;
    for (double i = 0; i < m_matrixDimension; i++) {
        m_iterationMatrix.insert(i,calcIterationMatrixLine(i));
        emit progress(static_cast<int>((i + 1.0)*100.0/static_cast<double>(m_matrixDimension)));
        if (!m_isCalculationRunning)
            return;
    }
    this->printFractal();
    emit calculateFinished();
}

void Fractal::stopCalculcation()
{
    m_isCalculationRunning = false;
}

void Fractal::prepareMatrix()
{
    int max = m_maxIterationCount;
    m_iterationMatrix.clear();
    for (double i = 0; i < m_matrixDimension; i++) {
        QVector<int> currentVector = QVector<int>(m_matrixDimension, max);
        m_iterationMatrix.append(currentVector);
    }
}

QVector<int> Fractal::calcIterationMatrixLine(double lineIndex)
{
    QVector<int> matrixLine;
    matrixLine.fill(0, m_matrixDimension);
    int *base = matrixLine.data();
    QFuture<void> result =  QtConcurrent::map(matrixLine,[&lineIndex,&base,this](int &value) {
        value = calcIterationCount(m_rectf.left() + lineIndex*m_stepX, m_rectf.top() + (&value - base)*m_stepY);
    });
    result.waitForFinished();
    return matrixLine;
}

int Fractal::calcIterationCount(const double x0, const double y0) const
{
    long double vectorLength = 0;
    long double x = 0;
    long double y = 0;
    long double fi;
    long double vectorLengthToThePower;
    int iteration;
    for (iteration = 0; (iteration < m_maxIterationCount)&(vectorLength < m_radius); iteration++) {
        vectorLength = sqrt(x*x + y*y);
        fi = atan2(y, x);
        vectorLengthToThePower = pow(vectorLength, m_power);
        x = vectorLengthToThePower*(cos(fi*m_power)) + x0;
        y = vectorLengthToThePower*(sin(fi*m_power)) + y0;
    }
    return iteration;
}

void Fractal::printFractal()
{
    QImage image = QImage(m_matrixDimension, m_matrixDimension, QImage::Format_RGB32);
    for (int i = 0; i < m_matrixDimension; i++) {
        for (int j = 0; j < m_matrixDimension; j++) {
            image.setPixel(i, j, getPixel(i, j).rgb());
        }
    }
    m_image = image;
}

QColor Fractal::getPixel(const int x, const int y) const
{
    double fraction = static_cast<double>(m_iterationMatrix[x][y])/static_cast<double>(m_maxIterationCount);
    fraction = pow(fraction, 0.5);
    if (fraction == 1.0) {
        return QColor(Qt::black);
    } else {
        return QColor(255*fraction, 255-255*fraction, 0, 255);
    }
}
