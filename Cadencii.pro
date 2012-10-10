#-------------------------------------------------
#
# Project created by QtCreator 2012-01-15T14:31:31
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = Cadencii
TEMPLATE = app

include(qt-solutions/qtpropertybrowser/src/qtpropertybrowser.pri)

SOURCES += main.cpp\
    qt/gui/WindowMainView.cpp \
    Controller.cpp \
    qt/gui/EditorWidgetBase.cpp \
    qt/gui/EditorWidgetBaseMainContent.cpp \
    qt/gui/EditorWidgetBaseSubContent.cpp \
    qt/gui/CurveControlChangeView.cpp \
    qt/gui/PianorollTrackView.cpp \
    qt/gui/ConcreteBarCountView.cpp \
    qt/gui/ConcreteTempoView.cpp \
    qt/gui/ConcreteTimesigView.cpp \
    qt/ConcreteSettings.cpp \
    qt/gui/ConcretePropertyView.cpp \
    command/DeleteEventCommand.cpp \
    command/AddEventCommand.cpp

HEADERS  += \
    gui/TrackView.hpp \
    qt/gui/WindowMainView.hpp \
    gui/MainView.hpp \
    Controller.hpp \
    ControllerAdapter.hpp \
    gui/ControlChangeView.hpp \
    qt/gui/EditorWidgetBase.hpp \
    qt/gui/EditorWidgetBaseMainContent.hpp \
    qt/gui/EditorWidgetBaseSubContent.hpp \
    qt/gui/CurveControlChangeView.hpp \
    qt/gui/PianorollTrackView.hpp \
    gui/BarCountView.hpp \
    qt/gui/ConcreteBarCountView.hpp \
    gui/TempoView.hpp \
    qt/gui/ConcreteTempoView.hpp \
    gui/TimesigView.hpp \
    qt/gui/ConcreteTimesigView.hpp \
    enum/QuantizeMode.hpp \
    Settings.hpp \
    qt/ConcreteSettings.hpp \
    audio/AudioOutput.hpp \
    audio/AudioMixer.hpp \
    audio/AudioGenerator.hpp \
    audio/AudioSender.hpp \
    audio/AudioReceiver.hpp \
    audio/AudioUnit.hpp \
    audio/AudioFilter.hpp \
    audio/AudioSplitter.hpp \
    audio/SampleRateLinearConverter.hpp \
    audio/FilterDriver.hpp \
    audio/WavFileReader.hpp \
    qt/audio/DeviceAudioOutput.hpp \
    enum/ToolKind.hpp \
    ItemSelectionManager.hpp \
    gui/PropertyView.hpp \
    qt/gui/ConcretePropertyView.hpp \
    ItemSelectionStatusListener.hpp \
    command/AbstractCommand.hpp \
    command/EditEventCommand.hpp \
    SequenceModel.hpp \
    command/DeleteEventCommand.hpp \
    command/AddEventCommand.hpp

FORMS    += \
    qt/gui/WindowMainView.ui \
    qt/gui/EditorWidgetBase.ui

OTHER_FILES += resource/win_icon.rc

RESOURCES += \
    qt/Cadencii.qrc

mac {
    ICON = resource/icon.icns
}
win32 {
    RC_FILE = resource/win_icon.rc
}

