#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QRectF>
#include <QThread>
#include <QMessageBox>
#include "Fractal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_fractal(new Fractal),
    m_calculateThread(new QThread)
{
    ui->setupUi(this);

    m_fractal->moveToThread(m_calculateThread);

    connect(m_calculateThread, &QThread::started, m_fractal, &Fractal::calculate);
    connect(m_fractal, &Fractal::calculateFinished, m_calculateThread, &QThread::quit);
    connect(m_fractal, &Fractal::calculateFinished, ui->fractalWidget, &FractalWidget::paintFractalImage);
    connect(m_fractal, &Fractal::progress, ui->progressBar, &QProgressBar::setValue);
    connect(ui->fractalWidget, &FractalWidget::rectChanged, this, &MainWindow::onRectChanged);

    ui->fractalWidget->setFractal(m_fractal);
    ui->fractalWidget->setCalculateThread(m_calculateThread);

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
    double top = -ui->topLineEdit->text().toDouble(&ok);
    if (isErrorExist(ok, "Top", "real"))
        return;
    double left = ui->leftLineEdit->text().toDouble(&ok);
    if (isErrorExist(ok, "Left", "real"))
        return;
    double right = ui->rightLineEdit->text().toDouble(&ok);
    if (isErrorExist(ok, "Right", "real"))
        return;
    double bottom = -ui->bottomLineEdit->text().toDouble(&ok);
    if (isErrorExist(ok, "Bottom", "real"))
        return;

    double height = bottom - top;
    if (height <= 0) {
        QMessageBox::warning(this, "Incorrect input", "Top must be greater than bottom!");
        return;
    }
    double width = right - left;
    if (width <= 0) {
        QMessageBox::warning(this, "Incorrect input", "Right must be greater than left!");
        return;
    }

    int matrixDimension = ui->matrixDimensionLineEdit->text().toInt(&ok);
    if (isErrorExist(ok, "Matrix dimension", "integer", matrixDimension, Fractal::MIN_MATRIX_DIMENSION, Fractal::MAX_MATRIX_DIMENSION))
        return;
    double radius = ui->radiusLineEdit->text().toDouble(&ok);
    if (isErrorExist(ok, "Radius", "real", radius, Fractal::MIN_RADIUS, Fractal::MAX_RADIUS))
        return;
    double power = ui->powerLineEdit->text().toDouble(&ok);
    if (isErrorExist(ok, "Power", "real", power, Fractal::MIN_POWER, Fractal::MAX_POWER))
        return;
    double maxIteration = ui->maxIterationLineEdit->text().toInt(&ok);
    if (isErrorExist(ok, "Max iteration", "integer", maxIteration, Fractal::MIN_ITERATION_COUNT, Fractal::MAX_ITERATION_COUNT))
        return;
    QRectF newRect = QRectF(left, top, width, height);
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
            QMessageBox::warning(this, "Incorrect input", QString(valueName + " must be [%1..%2]!").arg(min).arg(max));
            return true;
        }
    }
    return false;
}

bool MainWindow::isErrorExist(const bool ok, const QString valueName, const QString typeName)
{
    // Check type
    if (!ok) {
        QMessageBox::warning(this, "Incorrect input", valueName + " must be " + typeName + " type!");
        return true;
    }
    return false;
}




