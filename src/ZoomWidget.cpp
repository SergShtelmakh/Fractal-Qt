#include "ZoomWidget.h"

#include <QPainter>
#include <QMouseEvent>

namespace
{
	const QRectF cDefaultRectF      = QRectF(-1.0, -1.0, 2.0, 2.0);
	const double cDefaultZoomFactor = 2.0;
}

ZoomWidget::ZoomWidget(QWidget *parent)
	: QWidget(parent)
	, m_rectf(cDefaultRectF)
	, m_zoomBlocked(false)
	, m_zoomFactor(cDefaultZoomFactor)
{}

void ZoomWidget::paintImage(QImage image)
{
	m_image = image;
	update();
}

void ZoomWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawImage(rect(), m_image);
	QWidget::paintEvent(event);
}

void ZoomWidget::mousePressEvent(QMouseEvent *event)
{
	if (m_zoomBlocked) {
		event->ignore();
		return;
	}

	switch (event->button()) {
	case Qt::LeftButton:
		scaleRectF(event->localPos(), 1.0/m_zoomFactor);
		break;
	case Qt::RightButton:
		scaleRectF(event->localPos(), m_zoomFactor);
		break;
	default:
		break;
	}

	QWidget::mousePressEvent(event);
}

void ZoomWidget::scaleRectF(const QPointF &localCenter, const double scaleFactor)
{
	auto dx = m_rectf.width()  / this->rect().width();
	auto dy = m_rectf.height() / this->rect().height();

	auto newRectCenter = QPointF(m_rectf.left() + localCenter.x()*dx, m_rectf.top() + localCenter.y()*dy);
	auto newRectHeight = m_rectf.height()*scaleFactor;
	auto newRectWidth  = m_rectf.width() *scaleFactor;

	m_rectf = QRectF(newRectCenter.x() - newRectWidth /2.0, newRectCenter.y() - newRectHeight/2.0, newRectWidth, newRectHeight);
	emit rectChanged(m_rectf);
}
