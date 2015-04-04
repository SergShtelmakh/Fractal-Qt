#ifndef FRACTALWIDGET_H
#define FRACTALWIDGET_H

#include <QWidget>
#include "Fractal.h"

class FractalWidget : public QWidget
{
    Q_OBJECT

public:

    explicit FractalWidget(QWidget *parent = 0);

    void setFractal(Fractal *fractal);
    void setCalculateThread(QThread *calculateThread);

public slots:

    void paintFractalImage();

protected:

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:

    QRectF increaseZoomRectF(const QPointF &localCenter);
    QRectF decreaseZoomRectF(const QPointF &localCenter);
    QPointF getCenterPointF(const QPointF &center);

    QImage m_fractalImage;
    Fractal *m_fractal;
    QThread *m_calculateThread;

};

#endif // FRACTALWIDGET_H
