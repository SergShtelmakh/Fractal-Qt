#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QRectF>
#include <QThread>

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
    ui->fractalWidget->setFractal(m_fractal);
    ui->fractalWidget->setCalculateThread(m_calculateThread);
    m_calculateThread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_fractal;
    delete m_calculateThread;
}

void MainWindow::on_updatePushButton_clicked()
{
    if (m_calculateThread->isRunning()) {
        m_calculateThread->quit();
    }
    double top = -ui->topLineEdit->text().toDouble();
    double left = ui->leftLineEdit->text().toDouble();
    double right = ui->rightLineEdit->text().toDouble();
    double bottom = -ui->bottomLineEdit->text().toDouble();
    double height = bottom - top;
    double width = right - left;
    QRectF newRect = QRectF(left,top,width,height);
    int matrixDimension = ui->matrixDimensionLineEdit->text().toInt();
    double radius = ui->radiusLineEdit->text().toDouble();
    double power = ui->powerLineEdit->text().toDouble();
    updateFractalProperty(newRect, matrixDimension, radius, power);
    m_calculateThread->start();
}

void MainWindow::updateFractalProperty(const QRectF &rectf, const int matrixDimension, const double radius, const double power)
{
    m_fractal->setRect(rectf);
    m_fractal->setMatrixDimension(matrixDimension);
    m_fractal->setRadius(radius);
    m_fractal->setPower(power);
}
