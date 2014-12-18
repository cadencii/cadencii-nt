#include "../../audio/AudioMixer.hpp"
#include "../../audio/AudioGenerator.hpp"
#include "../../audio/AudioOutput.hpp"
#include "../Util.hpp"
#include "AudioGeneratorStub.hpp"
#include "AudioSenderStub.hpp"
#include "MemoryAudioOutput.hpp"
#include <cstdlib>
#include <gtest/gtest.h>

using namespace std;
using namespace cadencii::audio;

TEST(AudioMixerTest, test)
{
    const int sampleRate = 44100;
    const double passiveInputValue = 0.2;
    const uint64_t length = sampleRate + 17;

    double *activeInputFixture = new double[length];
    for( int i = 0; i < length; i++ ){
        activeInputFixture[i] = (rand() - RAND_MAX / 2) / (double)RAND_MAX;
    }

    AudioGeneratorStub activeInput( sampleRate, activeInputFixture, length );
    AudioSenderStub passiveInput( sampleRate, passiveInputValue );
    auto mixer = std::make_shared<AudioMixer>( sampleRate );
    MemoryAudioOutput output( sampleRate );
    activeInput.setReceiver(mixer);
    mixer->setReceiver( &output );
    mixer->addSource( &passiveInput );

    activeInput.start( length );

    uint64_t bufferLength = output.getBufferLength();
    EXPECT_EQ( length, bufferLength );
    double *actualLeft = new double[bufferLength];
    double *actualRight = new double[bufferLength];
    output.getResult( actualLeft, actualRight, bufferLength );
    for( uint64_t i = 0; i < bufferLength; i++ ){
        const double expected = activeInputFixture[i] + passiveInputValue;
        if( actualLeft[i] != expected || actualRight[i] != expected ){
            GTEST_FATAL_FAILURE_("waveform not equal");
            GTEST_FAIL();
        }
    }

    delete [] activeInputFixture;
    delete [] actualLeft;
    delete [] actualRight;
}
