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
        ZoomWidget.cpp \
        Fractals/Fractal.cpp \
        Fractals/JuliaFractal.cpp \
        Fractals/MandelbrotFractal.cpp

HEADERS  += MainWindow.h \
        Fractal.h \
        MandelbrotFractal.h \
        JuliaFractal.h \
        ZoomWidget.h \
        Fractals/Fractal.h \
        Fractals/JuliaFractal.h \
        Fractals/MandelbrotFractal.h

FORMS    += MainWindow.ui

CONFIG += c++11
