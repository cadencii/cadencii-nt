INCLUDEPATH += ../vsq/include ext/gtest/include ext/gtest
mac:INCLUDEPATH += /usr/local/include
CONFIG += c++11

SOURCES += \
    enum/QuantizeModeTest.cpp \
    main.cpp \
    audio/AudioMixerTest.cpp \
    audio/AudioSplitterTest.cpp \
    audio/SampleRateLinearConverterTest.cpp \
    audio/FilterDriverTest.cpp \
    audio/WavFileReaderTest.cpp \
    ItemSelectionManagerTest.cpp \
    command/EditEventCommandTest.cpp \
    sequence/SequenceModelTest.cpp \
    command/AddEventCommandTest.cpp \
    command/DeleteEventCommandTest.cpp \
    PropertyValueProxyTest.cpp \
    sequence/io/XVSQFileReaderTest.cpp \
    sequence/io/XVSQFileWriterTest.cpp \
    sequence/io/XVSQSpecTest.cpp \
    ext/gtest/src/gtest_main.cc \
    ext/gtest/src/gtest-all.cc \
    ../vsq/src/*.cpp \

HEADERS += \
    Util.hpp \
    audio/AudioSenderStub.hpp \
    audio/MemoryAudioOutput.hpp \
    audio/AudioGeneratorStub.hpp
