#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T02:31:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = browser
TEMPLATE = app

CONFIG += c++11

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/mainform.cpp \
    src/tabbar.cpp \
    src/tabwidget.cpp

HEADERS  += \
    src/mainwindow.h \
    src/mainform.h \
    src/tabbar.h \
    src/tabwidget.h

FORMS    += \
    src/mainwindow.ui \
    src/mainform.ui
