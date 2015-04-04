#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T17:05:05
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FractalQt
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
        FractalWidget.cpp \
        Fractal.cpp

HEADERS  += MainWindow.h \
        FractalWidget.h \
        Fractal.h


FORMS    += MainWindow.ui
