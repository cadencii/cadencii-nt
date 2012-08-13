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
    gui/PianorollKeyboard.cpp \
    gui/PianorollContent.cpp \
    gui/Pianoroll.cpp \
    gui/PianorollContentScroller.cpp

HEADERS  += gui/MainWindow.h \
    gui/Pianoroll.h \
    gui/PianorollContent.h \
    gui/PianorollItem.h \
    gui/PianorollContentScroller.h \
    gui/PianorollKeyboard.h

FORMS    += gui/MainWindow.ui \
    gui/Pianoroll.ui

OTHER_FILES +=

RESOURCES += \
    Cadencii.qrc

LIBS += -I"./gui/"
