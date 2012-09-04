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
    gui/PianorollTrackViewContent.cpp \
    gui/PianorollTrackView.cpp \
    gui/WindowMainView.cpp \
    Controller.cpp \
    gui/CurveControlChangeView.cpp \
    gui/CurveControlChangeViewSelector.cpp \
    gui/CurveControlChangeViewContentScroller.cpp \
    gui/CurveControlChangeViewContent.cpp

HEADERS  += \
    gui/PianorollTrackViewKeyboard.h \
    gui/PianorollTrackViewContent.h \
    gui/PianorollTrackView.h \
    TrackView.hpp \
    gui/WindowMainView.h \
    MainView.hpp \
    Controller.hpp \
    ControllerAdapter.hpp \
    ControlChangeView.hpp \
    gui/CurveControlChangeView.h \
    gui/CurveControlChangeViewSelector.h \
    gui/CurveControlChangeViewContentScroller.h \
    gui/CurveControlChangeViewContent.h

FORMS    += \
    gui/PianorollTrackView.ui \
    gui/WindowMainView.ui \
    gui/CurveControlChangeView.ui

OTHER_FILES +=

RESOURCES += \
    Cadencii.qrc

LIBS += -I"./gui/"
