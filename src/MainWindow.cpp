#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Fractals/MandelbrotFractal.h"
#include "Fractals/JuliaFractal.h"

#include <QRectF>
#include <QThread>
#include <QMessageBox>

namespace
{
	const int cMaxIteratinCount   = 10000;
	const int cMinIteratinCount   = 10;
	const int cMaxMatrixDimension = 10000;
	const int cMinMatrixDimension = 10;
	const double cMaxRadius       = 100.0;
	const double cMinRadius       = 0.0;
	const double cMaxPower        = 100.0;
	const double cMinPower        = 0.0;

	inline QString invalidTypeMessage(const QString &name, const QString &type)
	{
		return QString("%1 must be %2 type!").arg(name).arg(type);
	}

	inline QString notInBoundMessage(const QString &name, const int min, const int max)
	{
		return QString("%1 must be [%2..%3]!").arg(name).arg(min).arg(max);
	}
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_fractal(new JuliaFractal)
	, m_calculateThread(new QThread)
{
	ui->setupUi(this);

	m_fractal->moveToThread(m_calculateThread);

	connect(m_calculateThread, &QThread::started, m_fractal, &Fractal::startCalculation);
	connect(m_fractal, &Fractal::calculateStarted, ui->fractalWidget, [this]() {ui->fractalWidget->blockZoom(true);}, Qt::DirectConnection);
	connect(m_fractal, &Fractal::calculateFinished, ui->fractalWidget, [this](){ui->fractalWidget->blockZoom(false);}, Qt::DirectConnection);
	connect(m_fractal, &Fractal::calculateCanceled, ui->fractalWidget, [this](){ui->fractalWidget->blockZoom(false);}, Qt::DirectConnection);
	connect(m_fractal, &Fractal::calculateFinished, m_calculateThread, &QThread::quit);
	connect(m_fractal, &Fractal::calculateCanceled, m_calculateThread, &QThread::quit);
	connect(m_fractal, &Fractal::print, ui->fractalWidget, &ZoomWidget::paintImage);
	connect(m_fractal, &Fractal::progress, ui->progressBar, &QProgressBar::setValue);
	connect(ui->fractalWidget, &ZoomWidget::rectChanged, this, &MainWindow::onRectChanged);

	m_calculateThread->start();
}

MainWindow::~MainWindow()
{
	if (m_calculateThread->isRunning()) {
		m_calculateThread->quit();
	}
	m_calculateThread->deleteLater();
	delete m_fractal;
	delete ui;
}

void MainWindow::on_updatePushButton_clicked()
{
	if (m_calculateThread->isRunning()) {
		m_calculateThread->quit();
	}
	// Get settings from main window
	bool ok;
	auto top = -ui->topLineEdit->text().toDouble(&ok);
	if (isErrorExist(ok, "Top", "real"))
		return;
	auto left = ui->leftLineEdit->text().toDouble(&ok);
	if (isErrorExist(ok, "Left", "real"))
		return;
	auto right = ui->rightLineEdit->text().toDouble(&ok);
	if (isErrorExist(ok, "Right", "real"))
		return;
	auto bottom = -ui->bottomLineEdit->text().toDouble(&ok);
	if (isErrorExist(ok, "Bottom", "real"))
		return;

	auto height = bottom - top;
	if (height <= 0) {
		QMessageBox::warning(this, "Incorrect input", "Top must be greater than bottom!");
		return;
	}
	auto width = right - left;
	if (width <= 0) {
		QMessageBox::warning(this, "Incorrect input", "Right must be greater than left!");
		return;
	}

	auto matrixDimension = ui->matrixDimensionLineEdit->text().toInt(&ok);
	if (isErrorExist(ok, "Matrix dimension", "integer", matrixDimension, cMinMatrixDimension, cMaxMatrixDimension))
		return;
	auto radius = ui->radiusLineEdit->text().toDouble(&ok);
	if (isErrorExist(ok, "Radius", "real", radius, cMinRadius, cMaxRadius))
		return;
	auto power = ui->powerLineEdit->text().toDouble(&ok);
	if (isErrorExist(ok, "Power", "real", power, cMinPower, cMaxPower))
		return;
	auto maxIteration = ui->maxIterationLineEdit->text().toInt(&ok);
	if (isErrorExist(ok, "Max iteration", "integer", maxIteration, cMinIteratinCount, cMaxIteratinCount))
		return;
	auto newRect = QRectF(left, top, width, height);
	updateFractalProperty(newRect, matrixDimension, radius, power, maxIteration);
	m_calculateThread->start();
}

void MainWindow::on_stopPushButton_clicked()
{
	m_fractal->stopCalculcation();
}

void MainWindow::onRectChanged(QRectF newRect)
{
	ui->topLineEdit->setText(QString("%1").arg(-newRect.top()));
	ui->bottomLineEdit->setText(QString("%1").arg(-newRect.bottom()));
	ui->leftLineEdit->setText(QString("%1").arg(newRect.left()));
	ui->rightLineEdit->setText(QString("%1").arg(newRect.right()));
	m_fractal->setRect(newRect);
	m_calculateThread->start();
}

void MainWindow::updateFractalProperty(const QRectF &rectf, const int matrixDimension, const double radius, const double power, const double maxIteration) const
{
	m_fractal->setRect(rectf);
	m_fractal->setMatrixDimension(matrixDimension);
	m_fractal->setRadius(radius);
	m_fractal->setPower(power);
	m_fractal->setMaxIterationCount(maxIteration);
}

bool MainWindow::isErrorExist(const bool ok, const QString valueName, const QString typeName, const int value, const int min, const int max)
{
	// Check correct input
	if (isErrorExist(ok, valueName, typeName)) {
		return true;
	} else {
		// Check interval
		if ((value > max)||(value < min)) {
			QMessageBox::warning(this, "Incorrect input", notInBoundMessage(valueName, min, max));
			return true;
		}
	}
	return false;
}

bool MainWindow::isErrorExist(const bool ok, const QString valueName, const QString typeName)
{
	// Check type
	if (!ok) {
		QMessageBox::warning(this, "Incorrect input", invalidTypeMessage(valueName, typeName));
		return true;
	}
	return false;
}




