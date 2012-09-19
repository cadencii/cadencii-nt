#-------------------------------------------------
#
# Project created by QtCreator 2012-01-15T14:31:31
#
#-------------------------------------------------

QT       += core gui

TARGET = Cadencii
TEMPLATE = app


SOURCES += main.cpp\
    gui/WindowMainView.cpp \
    Controller.cpp \
    gui/EditorWidgetBase.cpp \
    gui/EditorWidgetBaseMainContent.cpp \
    gui/EditorWidgetBaseSubContent.cpp \
    gui/CurveControlChangeView.cpp \
    gui/PianorollTrackView.cpp \
    gui/ConcreteBarCountView.cpp

HEADERS  += \
    TrackView.hpp \
    gui/WindowMainView.h \
    MainView.hpp \
    Controller.hpp \
    ControllerAdapter.hpp \
    ControlChangeView.hpp \
    gui/EditorWidgetBase.h \
    gui/EditorWidgetBaseMainContent.h \
    gui/EditorWidgetBaseSubContent.h \
    gui/CurveControlChangeView.h \
    gui/PianorollTrackView.h \
    BarCountView.hpp \
    gui/ConcreteBarCountView.hpp

FORMS    += \
    gui/WindowMainView.ui \
    gui/EditorWidgetBase.ui

OTHER_FILES +=

RESOURCES += \
    Cadencii.qrc

LIBS += -I"./gui/"
