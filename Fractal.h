#ifndef FRACTAL_H
#define FRACTAL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QRectF>
#include <QImage>

class Fractal: public QObject
{
    Q_OBJECT

public:

    explicit Fractal(QObject *parent = 0);

    int matrixDimension() const;
    void setMatrixDimension(int matrixDimension);

    QRectF rect() const;
    void setRect(const QRectF &rect);

    QImage image() const;

    double radius() const;
    void setRadius(double radius);

    double power() const;
    void setPower(double power);

    static int maxIterationCount();

public slots:

    void calculate();

signals:

    void calculateFinished();
    void progress(int progress);

private:

    void prepareMatrix();
    QVector<int> calcIterationMatrixLine(const double x0, const double minY, const double stepY);
    int calcIterationCount(const double x0, const double y0) const;
    void printFractal();
    QColor getPixel(const int x, const int y) const;

    static int m_maxIterationCount;

    int m_matrixDimension;
    QVector<QVector<int> > m_iterationMatrix;
    double m_radius;
    double m_power;
    QRectF m_rect;
    QImage m_image;

};

QColor ColorRepresentation(int iterationCount);

#endif // FRACTAL_H
