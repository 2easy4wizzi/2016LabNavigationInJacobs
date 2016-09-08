#-------------------------------------------------
#
# Project created by QtCreator 2016-08-19T17:47:00
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Nav
TEMPLATE = app


SOURCES += main.cpp\
        nav.cpp \
    engine/graph.cpp \
    engine/node.cpp \
    engine/edge.cpp

HEADERS  += nav.h \
    engine/includes.h \
    dependencies/config.h \
    engine/graph.h \
    engine/node.h \
    engine/edge.h \
    engine/rapidxml.h

DISTFILES += \
    dependencies/rooms.xml
