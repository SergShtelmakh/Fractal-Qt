#include "FractalWidget.h"

#include "Fractal.h"

#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include <QThread>

const double FractalWidget::ZOOM_STEP = 2.0;

FractalWidget::FractalWidget(QWidget *parent)
	: QWidget(parent)
	, m_fractalImage(QImage())
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

	switch (event->button()) {
	case Qt::LeftButton:
		increaseZoomRectF(event->localPos());
		break;
	case Qt::RightButton:
		decreaseZoomRectF(event->localPos());
		break;
	default:
		break;
	}

	QWidget::mousePressEvent(event);
}

void FractalWidget::increaseZoomRectF(const QPointF &localCenter)
{
	auto center = getCenterPointF(localCenter);
	auto height = m_fractal->rectf().height()/ZOOM_STEP;
	auto width = m_fractal->rectf().width()/ZOOM_STEP;
	auto newRect = QRectF(center.x() - width /2.0, center.y() - height /2.0, width, height);
	m_fractal->setRect(newRect);
	emit rectChanged(newRect);
	m_calculateThread->start();
}

void FractalWidget::decreaseZoomRectF(const QPointF &localCenter)
{
	auto center = getCenterPointF(localCenter);
	auto height = m_fractal->rectf().height()*ZOOM_STEP;
	auto width = m_fractal->rectf().width()*ZOOM_STEP;
	auto newRect = QRectF(center.x() - width /2.0, center.y() - height /2.0, width, height);
	m_fractal->setRect(newRect);
	emit rectChanged(newRect);
	m_calculateThread->start();
}

QPointF FractalWidget::getCenterPointF(const QPointF &center)
{
	auto dx = m_fractal->rectf().width()/this->rect().width();
	auto dy = m_fractal->rectf().height()/this->rect().height();
	return QPointF(m_fractal->rectf().left() + center.x()*dx, m_fractal->rectf().top() + center.y()*dy);
}
