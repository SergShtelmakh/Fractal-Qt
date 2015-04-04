#include "FractalWidget.h"

#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include <QThread>


FractalWidget::FractalWidget(QWidget *parent) :
    QWidget(parent),
    m_fractalImage(QImage())
{}

FractalWidget::~FractalWidget()
{}

void FractalWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(rect(),m_fractalImage);
    QWidget::paintEvent(event);
}

void FractalWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_calculateThread->isRunning())
        return;
    if(event->button() == Qt::LeftButton){
        double dx = m_fractal->rect().width()/(double)this->rect().width();
        double dy = m_fractal->rect().height()/(double)this->rect().height();
        QPointF centerPos = QPointF(m_fractal->rect().left() + event->localPos().x()*dx, m_fractal->rect().top() + event->localPos().y()*dy);
        double height = m_fractal->rect().height() / 2.0;
        double width = m_fractal->rect().width() / 2.0;
        QRectF newRect = QRectF(centerPos.x() - width /2.0, centerPos.y() - height /2.0, width, height);
        m_fractal->setRect(newRect);
        m_calculateThread->start();
    }
    QWidget::mousePressEvent(event);
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

