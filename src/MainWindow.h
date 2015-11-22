/*!
 * \file MainWindow.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class Fractal;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

signals:
	void stopCalculation();

private slots:
	void on_updatePushButton_clicked();
	void on_stopPushButton_clicked();
	void onRectChanged(QRectF newRect);
	void oncbFractalTypeIndexChanged(int index);

private:
	void initializeFractal();
	void updateFractalProperty(const QRectF &rectf, const int matrixDimension, const double radius, const double power, const double maxIteration);

	QScopedPointer<Ui::MainWindow> ui;
	QScopedPointer<Fractal> m_fractal;

	QThread *m_pCalculateThread;
};

#endif // MAINWINDOW_H
