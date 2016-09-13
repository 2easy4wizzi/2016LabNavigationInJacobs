#-------------------------------------------------
#
# Project created by QtCreator 2016-08-19T17:47:00
#
#-------------------------------------------------

QT  += core gui multimedia multimediawidgets
QT  += xml

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
    engine/graph.h \
    engine/node.h \
    engine/edge.h \
    engine/rapidxml.h \
    engine/videoinfo.h

DISTFILES += \
    dependencies/rooms.xml \
    dependencies/config.xml
