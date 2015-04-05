#include "FractalWidget.h"

#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include <QThread>

FractalWidget::FractalWidget(QWidget *parent) :
    QWidget(parent),
    m_fractalImage(QImage())
{}

void FractalWidget::setFractal(Fractal *fractal)
{
    m_fractal = fractal;
}

void FractalWidget::setCalculateThread(QThread *calculateThread)
{
    m_calculateThread = calculateThread;
}

void FractalWidget::paintFractalImage()
{
    m_fractalImage = m_fractal->image();
    update();
}

void FractalWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(rect(), m_fractalImage);
    QWidget::paintEvent(event);
}

void FractalWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_calculateThread->isRunning())
        return;

    QRectF newRect;
    if (event->button() == Qt::LeftButton)
        newRect = increaseZoomRectF(event->localPos());
    if (event->button() == Qt::RightButton)
        newRect = decreaseZoomRectF(event->localPos());
    if (m_fractal->rectf() != newRect) {
        m_fractal->setRect(newRect);
        emit rectChanged(newRect);
        m_calculateThread->start();
    }

    QWidget::mousePressEvent(event);
}

QRectF FractalWidget::increaseZoomRectF(const QPointF &localCenter)
{
    QPointF center = getCenterPointF(localCenter);
    double height = m_fractal->rectf().height()/2.0;
    double width = m_fractal->rectf().width()/2.0;
    return QRectF(center.x() - width /2.0, center.y() - height /2.0, width, height);
}

QRectF FractalWidget::decreaseZoomRectF(const QPointF &localCenter)
{
    QPointF center = getCenterPointF(localCenter);
    double height = m_fractal->rectf().height()*2.0;
    double width = m_fractal->rectf().width()*2.0;
    return QRectF(center.x() - width /2.0, center.y() - height /2.0, width, height);
}

QPointF FractalWidget::getCenterPointF(const QPointF &center)
{
    double dx = m_fractal->rectf().width()/static_cast<double>(this->rect().width());
    double dy = m_fractal->rectf().height()/static_cast<double>(this->rect().height());
    return QPointF(m_fractal->rectf().left() + center.x()*dx, m_fractal->rectf().top() + center.y()*dy);
}
