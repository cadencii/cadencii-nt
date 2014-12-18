#include "../Util.hpp"
#include "../../audio/AudioSplitter.hpp"
#include "AudioGeneratorStub.hpp"
#include "MemoryAudioOutput.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace cadencii::audio;

TEST(AudioSplitterTest, test)
{
    const int sampleRate = 44100;

    uint64_t length = sampleRate + 13;
    double *activeInputFixture = new double[length];
    for( int i = 0; i < length; i++ ){
        activeInputFixture[i] = (rand() - RAND_MAX / 2) / (double)RAND_MAX;
    }
    AudioGeneratorStub input( sampleRate, activeInputFixture, length );

    auto splitter = std::make_shared<AudioSplitter>(sampleRate);
    MemoryAudioOutput out1( sampleRate );
    MemoryAudioOutput out2( sampleRate );
    input.setReceiver(splitter);
    splitter->addReceiver( &out1 );
    splitter->addReceiver( &out2 );

    input.start( length );

    EXPECT_EQ( length, out1.getBufferLength() );
    EXPECT_EQ( length, out2.getBufferLength() );

    double *actual1Left = new double[length];
    double *actual1Right = new double[length];
    double *actual2Left = new double[length];
    double *actual2Right = new double[length];
    out1.getResult( actual1Left, actual1Right, length );
    out2.getResult( actual2Left, actual2Right, length );
    for( uint64_t i = 0; i < length; i++ ){
        if( actual1Left[i] != activeInputFixture[i] || actual1Right[i] != activeInputFixture[i] ){
            GTEST_FATAL_FAILURE_("waveform not equal");
            GTEST_FAIL();
            break;
        }
        if( actual2Left[i] != activeInputFixture[i] || actual2Right[i] != activeInputFixture[i] ){
            GTEST_FATAL_FAILURE_("waveform not equal");
            GTEST_FAIL();
            break;
        }
    }

    delete [] activeInputFixture;
    delete [] actual1Left;
    delete [] actual1Right;
    delete [] actual2Left;
    delete [] actual2Right;
}
