contains( CONFIG, declarative_debug ){
    DESTDIR = $$PWD/bin/debug
}else{
    DESTDIR = $$PWD/bin/release
}
OBJECTS_DIR = $$DESTDIR
MOC_DIR = $$DESTDIR
RCC_DIR = $$DESTDIR
UI_DIR = $$DESTDIR

SOURCES += $$PWD/qt/gui/WindowMainView.cpp \
    $$PWD/Controller.cpp \
    $$PWD/qt/gui/EditorWidgetBase.cpp \
    $$PWD/qt/gui/EditorWidgetBaseMainContent.cpp \
    $$PWD/qt/gui/EditorWidgetBaseSubContent.cpp \
    $$PWD/qt/gui/CurveControlChangeView.cpp \
    $$PWD/qt/gui/PianorollTrackView.cpp \
    $$PWD/qt/gui/ConcreteBarCountView.cpp \
    $$PWD/qt/gui/ConcreteTempoView.cpp \
    $$PWD/qt/gui/ConcreteTimesigView.cpp \
    $$PWD/qt/ConcreteSettings.cpp \
    $$PWD/qt/gui/ConcretePropertyView.cpp \
    $$PWD/command/DeleteEventCommand.cpp \
    $$PWD/command/AddEventCommand.cpp \
    $$PWD/command/AbstractCommand.cpp

HEADERS  += \
    $$PWD/gui/TrackView.hpp \
    $$PWD/qt/gui/WindowMainView.hpp \
    $$PWD/gui/MainView.hpp \
    $$PWD/Controller.hpp \
    $$PWD/ControllerAdapter.hpp \
    $$PWD/gui/ControlChangeView.hpp \
    $$PWD/qt/gui/EditorWidgetBase.hpp \
    $$PWD/qt/gui/EditorWidgetBaseMainContent.hpp \
    $$PWD/qt/gui/EditorWidgetBaseSubContent.hpp \
    $$PWD/qt/gui/CurveControlChangeView.hpp \
    $$PWD/qt/gui/PianorollTrackView.hpp \
    $$PWD/gui/BarCountView.hpp \
    $$PWD/qt/gui/ConcreteBarCountView.hpp \
    $$PWD/gui/TempoView.hpp \
    $$PWD/qt/gui/ConcreteTempoView.hpp \
    $$PWD/gui/TimesigView.hpp \
    $$PWD/qt/gui/ConcreteTimesigView.hpp \
    $$PWD/enum/QuantizeMode.hpp \
    $$PWD/Settings.hpp \
    $$PWD/qt/ConcreteSettings.hpp \
    $$PWD/audio/AudioOutput.hpp \
    $$PWD/audio/AudioMixer.hpp \
    $$PWD/audio/AudioGenerator.hpp \
    $$PWD/audio/AudioSender.hpp \
    $$PWD/audio/AudioReceiver.hpp \
    $$PWD/audio/AudioUnit.hpp \
    $$PWD/audio/AudioFilter.hpp \
    $$PWD/audio/AudioSplitter.hpp \
    $$PWD/audio/SampleRateLinearConverter.hpp \
    $$PWD/audio/FilterDriver.hpp \
    $$PWD/audio/WavFileReader.hpp \
    $$PWD/qt/audio/DeviceAudioOutput.hpp \
    $$PWD/enum/ToolKind.hpp \
    $$PWD/ItemSelectionManager.hpp \
    $$PWD/gui/PropertyView.hpp \
    $$PWD/qt/gui/ConcretePropertyView.hpp \
    $$PWD/ItemSelectionStatusListener.hpp \
    $$PWD/command/AbstractCommand.hpp \
    $$PWD/command/EditEventCommand.hpp \
    $$PWD/SequenceModel.hpp \
    $$PWD/command/DeleteEventCommand.hpp \
    $$PWD/command/AddEventCommand.hpp \
    $$PWD/qt/AppContainer.hpp

FORMS    += \
    $$PWD/qt/gui/WindowMainView.ui \
    $$PWD/qt/gui/EditorWidgetBase.ui

OTHER_FILES += $$PWD/resource/win_icon.rc

mac {
    ICON = $$PWD/resource/icon.icns
}
win32 {
    RC_FILE = $$PWD/resource/win_icon.rc
}

RESOURCES += \
    $$PWD/qt/Cadencii.qrc
