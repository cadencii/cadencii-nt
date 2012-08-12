#-------------------------------------------------
#
# Project created by QtCreator 2012-01-15T14:31:31
#
#-------------------------------------------------

QT       += core gui

TARGET = Cadencii
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    PianorollKeyboard.cpp \
    PianorollContent.cpp \
    Pianoroll.cpp \
    PianorollContentScroller.cpp

HEADERS  += MainWindow.h \
    Pianoroll.h \
    PianorollContent.h \
    PianorollItem.h \
    PianorollContentScroller.h \
    PianorollKeyboard.h

FORMS    += MainWindow.ui \
    Pianoroll.ui

OTHER_FILES +=

RESOURCES += \
    Cadencii.qrc

LIBS += -I"./"
