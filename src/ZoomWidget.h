/*!
 * \file FractalWidget.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <QWidget>

class ZoomWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ZoomWidget(QWidget *parent = 0);

	void setRectF(const QRectF &rectf) { m_rectf = rectf; }
	void blockZoom(const bool blocked) { m_zoomBlocked = blocked; }
	void setZoomFactor(const double factor) { m_zoomFactor = factor; }

signals:
	void rectChanged(QRectF newRectf);

public slots:
	void paintImage(QImage image);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

private:
	void scaleRectF(const QPointF &localCenter, const double scaleFactor);

	QImage m_image;
	QRectF m_rectf;
	bool   m_zoomBlocked;
	double m_zoomFactor;
};

#endif // ZOOMWIDGET_H
