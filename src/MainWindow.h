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

class Fractal;

namespace Ui {
class MainWindow;
}

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

private:

    void updateFractalProperty(const QRectF &rectf, const int matrixDimension, const double radius, const double power, const double maxIteration) const;
    bool isErrorExist(const bool ok, const QString valueName, const QString typeName, const int value, const int min, const int max);
    bool isErrorExist(const bool ok, const QString valueName, const QString typeName);

    Ui::MainWindow *ui;
    Fractal *m_fractal;
    QThread *m_calculateThread;

};

#endif // MAINWINDOW_H
