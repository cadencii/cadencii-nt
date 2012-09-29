#include "../../audio/AudioMixer.hpp"
#include "../../audio/ActiveAudioInput.hpp"
#include "../../audio/AudioOutput.hpp"
#include "../../audio/PassiveAudioInput.hpp"
#include "../Util.hpp"
#include "ActiveAudioInputStub.hpp"
#include "PassiveAudioInputStub.hpp"
#include "MemoryAudioOutput.hpp"
#include <cstdlib>

using namespace std;
using namespace cadencii::audio;

class AudioMixerTest : public CppUnit::TestCase{
public:
    void test(){
        const int sampleRate = 44100;
        const double passiveInputValue = 0.2;
        const uint64_t length = sampleRate + 17;

        double *activeInputFixture = new double[length];
        for( int i = 0; i < length; i++ ){
            activeInputFixture[i] = (rand() - RAND_MAX / 2) / (double)RAND_MAX;
        }

        ActiveAudioInputStub activeInput( sampleRate, activeInputFixture, length );
        PassiveAudioInputStub passiveInput( sampleRate, passiveInputValue );
        AudioMixer mixer( sampleRate );
        MemoryAudioOutput output( sampleRate );
        activeInput.setReceiver( &mixer );
        mixer.setReceiver( &output );
        mixer.addSource( &passiveInput );

        activeInput.start( length );

        uint64_t bufferLength = output.getBufferLength();
        CPPUNIT_ASSERT_EQUAL( length, bufferLength );
        double *actualLeft = new double[bufferLength];
        double *actualRight = new double[bufferLength];
        output.getResult( actualLeft, actualRight, bufferLength );
        for( uint64_t i = 0; i < bufferLength; i++ ){
            const double expected = activeInputFixture[i] + passiveInputValue;
            if( actualLeft[i] != expected || actualRight[i] != expected ){
                CPPUNIT_FAIL( "waveform not equal" );
            }
        }

        delete [] activeInputFixture;
        delete [] actualLeft;
        delete [] actualRight;
    }

    CPPUNIT_TEST_SUITE( AudioMixerTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( AudioMixerTest );
