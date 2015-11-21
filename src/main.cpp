#include "MainWindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QDir appDir = QFileInfo(QString::fromLocal8Bit(argv[0])).absoluteDir();
    QString libPath = appDir.absolutePath() + "/plugins";
    QApplication::addLibraryPath(libPath);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
