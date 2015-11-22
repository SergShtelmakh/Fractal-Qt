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
        Fractal.cpp \
        MandelbrotFractal.cpp \
        JuliaFractal.cpp \
        ZoomWidget.cpp

HEADERS  += MainWindow.h \
        Fractal.h \
        MandelbrotFractal.h \
        JuliaFractal.h \
        ZoomWidget.h

FORMS    += MainWindow.ui

CONFIG += c++11
