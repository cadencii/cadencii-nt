#include "../../audio/AudioMixer.hpp"
#include "../../audio/ActiveAudioInput.hpp"
#include "../../audio/AudioOutput.hpp"
#include "../../audio/PassiveAudioInput.hpp"
#include "../Util.hpp"
#include "ActiveAudioInputStub.hpp"
#include "PassiveAudioInputStub.hpp"
#include "MemoryAudioOutput.hpp"

using namespace std;
using namespace cadencii::audio;

class AudioMixerTest : public CppUnit::TestCase{
public:
    void test(){
        const int sampleRate = 44100;
        const double activeInputValue = 0.1;
        const double passiveInputValue = -0.2;
        ActiveAudioInputStub activeInput( sampleRate, activeInputValue );
        PassiveAudioInputStub passiveInput( sampleRate, passiveInputValue );
        AudioMixer mixer( sampleRate );
        MemoryAudioOutput output( sampleRate );
        activeInput.setReceiver( &mixer );
        mixer.setReceiver( &output );
        mixer.addSource( &passiveInput );

        uint64_t length = sampleRate + 100;
        activeInput.start( length );

        uint64_t bufferLength = output.getBufferLength();
        CPPUNIT_ASSERT_EQUAL( length, bufferLength );
        double *actualLeft = new double[bufferLength];
        double *actualRight = new double[bufferLength];
        output.getResult( actualLeft, actualRight, bufferLength );
        const double expected = activeInputValue + passiveInputValue;
        for( uint64_t i = 0; i < bufferLength; i++ ){
            if( actualLeft[i] != expected || actualRight[i] != expected ){
                CPPUNIT_FAIL( "waveform not equal" );
            }
        }
        delete [] actualLeft;
        delete [] actualRight;
    }

    CPPUNIT_TEST_SUITE( AudioMixerTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( AudioMixerTest );
