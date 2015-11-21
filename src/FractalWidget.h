/*!
 * \file FractalWidget.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef FRACTALWIDGET_H
#define FRACTALWIDGET_H

#include <QWidget>
class Fractal;

class FractalWidget : public QWidget
{
    Q_OBJECT

public:

    static const double ZOOM_STEP ;

    explicit FractalWidget(QWidget *parent = 0);

    void setFractal(Fractal *fractal);
    void setCalculateThread(QThread *calculateThread);

signals:

    void rectChanged(QRectF newRectf);

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
