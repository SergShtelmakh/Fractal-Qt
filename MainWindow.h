#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Fractal.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_updatePushButton_clicked();

private:

    Ui::MainWindow *ui;
    Fractal *m_fractal;
    QThread *m_calculateThread;

    void updateFractalProperty(const QRectF &rectf, const int matrixDimension, const double radius, const double power);
};

#endif // MAINWINDOW_H
