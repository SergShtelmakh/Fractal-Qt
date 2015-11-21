#include "FractalWidget.h"

#include "Fractal.h"

#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include <QThread>

namespace
{
	const double cZoomFactor = 2.0;
}

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

void FractalWidget::paintFractalImage(QImage image)
{
	m_fractalImage = image;
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
		scaleRectF(event->localPos(), 1 / cZoomFactor);
		break;
	case Qt::RightButton:
		scaleRectF(event->localPos(), cZoomFactor);
		break;
	default:
		break;
	}

	QWidget::mousePressEvent(event);
}

void FractalWidget::scaleRectF(const QPointF &localCenter, const double scaleFactor)
{
	auto dx = m_fractal->rectf().width()  / this->rect().width();
	auto dy = m_fractal->rectf().height() / this->rect().height();

	auto newRectCenter = QPointF(m_fractal->rectf().left() + localCenter.x()*dx, m_fractal->rectf().top() + localCenter.y()*dy);
	auto newRectHeight = m_fractal->rectf().height()*scaleFactor;
	auto newRectWidth  = m_fractal->rectf().width() *scaleFactor;

	auto newRect = QRectF(newRectCenter.x() - newRectWidth /2.0, newRectCenter.y() - newRectHeight/2.0, newRectWidth, newRectHeight);
	m_fractal->setRect(newRect);
	emit rectChanged(newRect);
	m_calculateThread->start();
}
