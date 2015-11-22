#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Fractals/MandelbrotFractal.h"
#include "Fractals/JuliaFractal.h"

#include <QRectF>
#include <QThread>
#include <QMessageBox>

namespace
{
	// QPair<T min, T max>
	const QPair<int, int> cMaxIterationPair    = qMakePair(10, 10000);
	const QPair<int, int> cMatrixDimensionPair = qMakePair(10, 10000);
	const QPair<double, double> cRadiusPair    = qMakePair(0.0, 100.0);
	const QPair<double, double> cPowerPair     = qMakePair(0.0, 100.0);

	const QString csInteger         = "Integer";
	const QString csDouble          = "Double";

	inline QString invalidTypeMessage(const QString &name, const QString &type)
	{
		return QString("%1 must be %2 type!").arg(name).arg(type);
	}

	template<class T>
	inline QString notInBoundMessage(const QString &name, const QPair<T, T> &pair)
	{
		return QString("%1 must be [%2..%3]!").arg(name).arg(pair.first).arg(pair.second);
	}

	template<class T>
	inline bool inBound(const T &val, const QPair<T, T> &pair)
	{
		return (pair.first <= val)&&(val <= pair.second);
	}
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_fractal(new MandelbrotFractal)
	, m_pCalculateThread(new QThread)
{
	ui->setupUi(this);

	initializeFractal();

	connect(ui->fractalWidget, &ZoomWidget::rectChanged, this, &MainWindow::onRectChanged);
	connect(ui->cbFractalType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::oncbFractalTypeIndexChanged);

	m_pCalculateThread->start();
}

MainWindow::~MainWindow()
{
	if (m_pCalculateThread->isRunning()) {
		m_pCalculateThread->quit();
	}
	m_pCalculateThread->deleteLater();
}

void MainWindow::on_updatePushButton_clicked()
{
	auto incorrectInputWarning = [this](const bool notOk,const QString &text) {
		if (notOk)
			QMessageBox::warning(this, "Incorrect input", text);
		return notOk;
	};

	if (m_pCalculateThread->isRunning()) {
		m_pCalculateThread->quit();
	}

	bool ok;
	auto matrixDimension = ui->matrixDimensionLineEdit->text().toInt(&ok);
	if (incorrectInputWarning(!ok, invalidTypeMessage("Matrix dimension", csInteger))
			|| incorrectInputWarning(!inBound(matrixDimension, cMatrixDimensionPair), notInBoundMessage("Matrix dimension", cMatrixDimensionPair)))
		return;

	auto radius = ui->radiusLineEdit->text().toDouble(&ok);
	if (incorrectInputWarning(!ok, invalidTypeMessage("Radius", csDouble))
			|| incorrectInputWarning(!inBound(radius, cRadiusPair), notInBoundMessage("Radius", cRadiusPair)))
		return;

	auto power = ui->powerLineEdit->text().toDouble(&ok);
	if (incorrectInputWarning(!ok, invalidTypeMessage("Power", csDouble))
			|| incorrectInputWarning(!inBound(power, cPowerPair), notInBoundMessage("Power", cPowerPair)))
		return;

	auto maxIteration = ui->maxIterationLineEdit->text().toInt(&ok);
	if (incorrectInputWarning(!ok, invalidTypeMessage("Max iteration", csInteger))
			|| incorrectInputWarning(!inBound(maxIteration, cMaxIterationPair), notInBoundMessage("Max iteration", cMaxIterationPair)))
		return;


	auto top = -ui->topLineEdit->text().toDouble(&ok);
	if (incorrectInputWarning(!ok, invalidTypeMessage("Top",   csDouble)))
		return;

	auto left = ui->leftLineEdit->text().toDouble(&ok);
	if (incorrectInputWarning(!ok, invalidTypeMessage("Left",  csDouble)))
		return;

	auto right = ui->rightLineEdit->text().toDouble(&ok);
	if (incorrectInputWarning(!ok, invalidTypeMessage("Right", csDouble)))
		return;

	auto bottom = -ui->bottomLineEdit->text().toDouble(&ok);
	if (incorrectInputWarning(!ok, invalidTypeMessage("Bottom",csDouble)))
		return;

	auto height = bottom - top;
	if (incorrectInputWarning(height <= 0 ,"Top must be greater than bottom!" ))
		return;

	auto width = right - left;
	if (incorrectInputWarning(width <= 0  ,"Right must be greater than left!" ))
		return;

	auto newRect = QRectF(left, top, width, height);
	updateFractalProperty(newRect, matrixDimension, radius, power, maxIteration);
}

void MainWindow::on_stopPushButton_clicked()
{
	m_fractal->stopCalculcation();
}

void MainWindow::onRectChanged(QRectF newRect)
{
	ui->topLineEdit->setText   (QString("%1").arg(-newRect.top()));
	ui->bottomLineEdit->setText(QString("%1").arg(-newRect.bottom()));
	ui->leftLineEdit->setText  (QString("%1").arg( newRect.left()));
	ui->rightLineEdit->setText (QString("%1").arg( newRect.right()));

	m_fractal->setRect(newRect);
	m_pCalculateThread->start();
}

void MainWindow::oncbFractalTypeIndexChanged(int index)
{
	if (m_pCalculateThread->isRunning()) {
		m_fractal->stopCalculcation();

		QEventLoop e;
		connect(m_fractal.data(), &Fractal::calculateCanceled, &e, [&e](){e.exit(0);});
		e.exec();
	}

	switch (index) {
	case 0:
		m_fractal.reset(new MandelbrotFractal);
		break;
	case 1:
		m_fractal.reset(new JuliaFractal);
		break;
	default:
		return;
	}
	initializeFractal();
	on_updatePushButton_clicked();
}

void MainWindow::initializeFractal()
{
	m_fractal->moveToThread(m_pCalculateThread);
	connect(m_pCalculateThread, &QThread::started,           m_fractal.data(),   &Fractal::startCalculation);
	connect(m_fractal.data(),   &Fractal::calculateFinished, m_pCalculateThread, &QThread::quit);
	connect(m_fractal.data(),   &Fractal::calculateCanceled, m_pCalculateThread, &QThread::quit);

	connect(m_fractal.data(), &Fractal::calculateStarted,  ui->fractalWidget, [this](){ui->fractalWidget->blockZoom(true); }, Qt::DirectConnection);
	connect(m_fractal.data(), &Fractal::calculateFinished, ui->fractalWidget, [this](){ui->fractalWidget->blockZoom(false);}, Qt::DirectConnection);
	connect(m_fractal.data(), &Fractal::calculateCanceled, ui->fractalWidget, [this](){ui->fractalWidget->blockZoom(false);}, Qt::DirectConnection);

	connect(m_fractal.data(), &Fractal::print,    ui->fractalWidget, &ZoomWidget::paintImage);
	connect(m_fractal.data(), &Fractal::progress, ui->progressBar,   &QProgressBar::setValue);

}

void MainWindow::updateFractalProperty(const QRectF &rectf, const int matrixDimension, const double radius, const double power, const double maxIteration)
{
	m_fractal->setRect(rectf);
	m_fractal->setMatrixDimension(matrixDimension);
	m_fractal->setRadius(radius);
	m_fractal->setPower(power);
	m_fractal->setMaxIterationCount(maxIteration);
	m_pCalculateThread->start();
}
