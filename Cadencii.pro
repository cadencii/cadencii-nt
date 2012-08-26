#-------------------------------------------------
#
# Project created by QtCreator 2012-01-15T14:31:31
#
#-------------------------------------------------

QT       += core gui

TARGET = Cadencii
TEMPLATE = app


SOURCES += main.cpp\
    gui/PianorollTrackViewKeyboard.cpp \
    gui/PianorollTrackViewContentScroller.cpp \
    gui/PianorollTrackViewContent.cpp \
    gui/PianorollTrackView.cpp \
    gui/WindowMainView.cpp \
    Controller.cpp

HEADERS  += \
    gui/PianorollTrackViewKeyboard.h \
    gui/PianorollTrackViewContentScroller.h \
    gui/PianorollTrackViewContent.h \
    gui/PianorollTrackView.h \
    TrackView.hpp \
    gui/WindowMainView.h \
    MainView.hpp \
    Controller.hpp \
    ControllerAdapter.hpp

FORMS    += \
    gui/PianorollTrackView.ui \
    gui/WindowMainView.ui

OTHER_FILES +=

RESOURCES += \
    Cadencii.qrc

LIBS += -I"./gui/"
