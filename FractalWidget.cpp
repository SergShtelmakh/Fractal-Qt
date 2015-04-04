#include "FractalWidget.h"

#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include <QThread>

FractalWidget::FractalWidget(QWidget *parent) :
    QWidget(parent),
    m_fractalImage(QImage())
{}

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
    if (event->button() == Qt::LeftButton) {
        m_fractal->setRect(increaseZoomRectF(event->localPos()));
        m_calculateThread->start();
    }
    if (event->button() == Qt::RightButton) {
        m_fractal->setRect(decreaseZoomRectF(event->localPos()));
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

void FractalWidget::setCalculateThread(QThread *calculateThread)
{
    m_calculateThread = calculateThread;
}

void FractalWidget::setFractal(Fractal *fractal)
{
    m_fractal = fractal;
}

void FractalWidget::paintFractalImage()
{
    m_fractalImage = m_fractal->image();
    update();
}

