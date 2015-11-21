#include "MainWindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
	auto appDir = QFileInfo(QString::fromLocal8Bit(argv[0])).absoluteDir();
	auto libPath = appDir.absolutePath() + "/plugins";
    QApplication::addLibraryPath(libPath);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
