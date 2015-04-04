#include "Fractal.h"

#include <math.h>
#include <QImage>

int Fractal::m_maxIterationCount = 500;

Fractal::Fractal(QObject *parent) :
    QObject(parent),
    m_matrixDimension(700),
    m_radius(10),
    m_power(2),
    m_rect(QRectF(-2.0, -2.0, 4.0, 4.0)),
    m_image(QImage(m_matrixDimension, m_matrixDimension, QImage::Format_RGB32))
{}

void Fractal::calculate()
{
    this->prepareMatrix();
    double stepX = (m_rect.right() - m_rect.left()) / m_matrixDimension;
    double stepY = (m_rect.bottom() - m_rect.top()) / m_matrixDimension;
    for (double i = 0; i < m_matrixDimension; i++) {
       m_iterationMatrix.insert(i,calcIterationMatrixLine(m_rect.left() + i*stepX,m_rect.top(),stepY));
       emit progress(static_cast<int>((i + 1.0)*100.0/static_cast<double>(m_matrixDimension)));
    }
    this->printFractal();
    emit calculateFinished();
}

QVector<int> Fractal::calcIterationMatrixLine(const double x0, const double minY, const double stepY)
{
    QVector<int> matrixLine;
    matrixLine.fill(0, m_matrixDimension);
    for (double i = 0; i < m_matrixDimension; i++) {
        matrixLine[i] = calcIterationCount(x0, minY + i*stepY);
    }
    return matrixLine;
}


int Fractal::calcIterationCount(const double x0, const double y0) const
{
    double vectorLength = 0;
    double x = 0;
    double y = 0;
    double fi;
    double vectorLengthToThePower;
    int iteration;
    for (iteration = 0; (iteration < m_maxIterationCount)&(vectorLength < m_radius); iteration++) {
        vectorLength = sqrt(x*x + y*y);
        fi = atan2(y, x);
        vectorLengthToThePower = pow(vectorLength,m_power);
        x = vectorLengthToThePower*(cos(fi*m_power)) + x0;
        y = vectorLengthToThePower*(sin(fi*m_power)) + y0;
    }
    return iteration;
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

double Fractal::power() const
{
    return m_power;
}

void Fractal::setPower(double power)
{
    m_power = power;
}

double Fractal::radius() const
{
    return m_radius;
}

void Fractal::setRadius(double radius)
{
    m_radius = radius;
}

QImage Fractal::image() const
{
    return m_image;
}

QRectF Fractal::rect() const
{
    return m_rect;
}

void Fractal::setRect(const QRectF &rect)
{
    m_rect = rect;
}

int Fractal::maxIterationCount()
{
    return m_maxIterationCount;
}

int Fractal::matrixDimension() const
{
    return m_matrixDimension;
}

void Fractal::setMatrixDimension(int matrixDimension)
{
    m_matrixDimension = matrixDimension;
}

QColor Fractal::getPixel(const int x, const int y) const
{
    return ColorRepresentation(m_iterationMatrix[x][y]);
}

QColor ColorRepresentation(int iterationCount)
{
    double fraction = static_cast<double>(iterationCount)/static_cast<double>(Fractal::maxIterationCount());
    fraction = pow(fraction, 0.5);
    if (fraction == 1.0) {
        return QColor(Qt::black);
    } else {
        return QColor(255*fraction, 255-255*fraction, 0, 255);
    }
}
