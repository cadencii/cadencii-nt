#-------------------------------------------------
#
# Project created by QtCreator 2012-01-15T14:31:31
#
#-------------------------------------------------

QT       += core gui

TARGET = Cadencii
TEMPLATE = app


SOURCES += main.cpp\
    gui/MainWindow.cpp \
    gui/PianorollTrackViewKeyboard.cpp \
    gui/PianorollTrackViewContentScroller.cpp \
    gui/PianorollTrackViewContent.cpp \
    gui/PianorollTrackView.cpp

HEADERS  += gui/MainWindow.h \
    gui/PianorollTrackViewKeyboard.h \
    gui/PianorollTrackViewContentScroller.h \
    gui/PianorollTrackViewContent.h \
    gui/PianorollTrackView.h

FORMS    += gui/MainWindow.ui \
    gui/PianorollTrackView.ui

OTHER_FILES +=

RESOURCES += \
    Cadencii.qrc

LIBS += -I"./gui/"
