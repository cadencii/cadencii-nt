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
    gui/ConcreteBarCountView.cpp \
    gui/ConcreteTempoView.cpp \
    gui/ConcreteTimesigView.cpp \
    Settings.cpp

HEADERS  += \
    TrackView.hpp \
    gui/WindowMainView.hpp \
    MainView.hpp \
    Controller.hpp \
    ControllerAdapter.hpp \
    ControlChangeView.hpp \
    gui/EditorWidgetBase.hpp \
    gui/EditorWidgetBaseMainContent.hpp \
    gui/EditorWidgetBaseSubContent.hpp \
    gui/CurveControlChangeView.hpp \
    gui/PianorollTrackView.hpp \
    BarCountView.hpp \
    gui/ConcreteBarCountView.hpp \
    TempoView.hpp \
    gui/ConcreteTempoView.hpp \
    TimesigView.hpp \
    gui/ConcreteTimesigView.hpp \
    enum/QuantizeMode.hpp \
    Settings.hpp

FORMS    += \
    gui/WindowMainView.ui \
    gui/EditorWidgetBase.ui

OTHER_FILES +=

RESOURCES += \
    Cadencii.qrc

LIBS += -I"./gui/"

mac {
    ICON = resources/icon.icns
}
