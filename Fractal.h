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

    QRectF rectf() const;
    void setRect(const QRectF &rectf);

    QImage image() const;

    double radius() const;
    void setRadius(double radius);

    double power() const;
    void setPower(double power);

    static int maxIterationCount();
    static void setMaxiterationCount(int maxIterationCount);

public slots:

    void calculate();

signals:

    void calculateFinished();
    void progress(int progress);

private:

    void prepareMatrix();
    int calcIterationCount(const double x0, const double y0) const;
    void printFractal();
    QColor getPixel(const int x, const int y) const;
    QVector<int> calcIterationMatrixLine(double lineIndex);
    void concurrentlyCalcIterationMatrixLine(int &value, int lineIndex, int *base);

    static int m_maxIterationCount;

    int m_matrixDimension;
    QVector<QVector<int> > m_iterationMatrix;
    double m_radius;
    double m_power;
    QRectF m_rectf;
    QImage m_image;
    double m_stepX;
    double m_stepY;

};

QColor ColorRepresentation(int iterationCount);

#endif // FRACTAL_H
