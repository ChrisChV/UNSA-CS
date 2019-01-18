#-------------------------------------------------
#
# Project created by QtCreator 2015-09-10T13:45:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Indexacion
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Table.cpp \
    Record.cpp

HEADERS  += mainwindow.h \
    Table.h \
    sad.h \
    Record.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    Resources.qrc
