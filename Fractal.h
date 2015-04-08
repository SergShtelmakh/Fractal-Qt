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

    int maxIterationCount() const;
    void setMaxIterationCount(const int maxIterationCount);

    int matrixDimension() const;
    void setMatrixDimension(const int matrixDimension);

    double radius() const;
    void setRadius(const double radius);

    double power() const;
    void setPower(const double power);

    QRectF rectf() const;
    void setRect(const QRectF &rectf);


    QImage image() const;

public slots:

    void calculate();
    void stopCalculcation();

signals:

    void calculateFinished();
    void progress(int progress);

private:

    void prepareToCalculate();
    QVector<int> calcIterationMatrixLine(const int lineIndex);
    int calcIterationCountAtPoint(const double x0, const double y0) const;
    void printFractal();
    QColor getPixel(const int x, const int y) const;

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
