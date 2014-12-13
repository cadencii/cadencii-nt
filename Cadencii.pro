#-------------------------------------------------
#
# Project created by QtCreator 2012-01-15T14:31:31
#
#-------------------------------------------------

QT       += core widgets multimedia

TARGET = Cadencii
TEMPLATE = app

include(qt-property-browser/src/qtpropertybrowser.pri)
include(cadencii_common.pri)

SOURCES += main.cpp

HEADERS +=

INCLUDEPATH += . /usr/local/include ./vsq/include

CONFIG += c++11
