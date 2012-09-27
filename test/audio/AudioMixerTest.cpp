#include "../Util.hpp"
#include "../../audio/AudioMixer.hpp"
#include "../../audio/ActiveAudioInput.hpp"
#include "../../audio/AudioOutput.hpp"
#include "../../audio/PassiveAudioInput.hpp"

using namespace std;
using namespace cadencii::audio;

class AudioMixerTest : public CppUnit::TestCase{
    /**
     * @brief 常に同じ値の DC波形を出力する、ActiveAudioInput の実装
     */
    class ActiveAudioInputStub : public ActiveAudioInput{
    private:
        double value;
        Receiver *receiver;

    public:
        explicit ActiveAudioInputStub( int sampleRate, double value ) :
            ActiveAudioInput( sampleRate ), receiver( 0 )
        {
            this->value = value;
        }

        void setReceiver( Receiver *receiver ){
            this->receiver = receiver;
        }

        void start( uint64_t length ){
            if( !receiver ) return;
            const int unitBufferLength = 1024;
            double *bufferLeft = new double[unitBufferLength];
            double *bufferRight = new double[unitBufferLength];
            for( int i = 0; i < unitBufferLength; i++ ){
                bufferLeft[i] = value;
                bufferRight[i] = value;
            }
            uint64_t remain = length;
            while( 0 < remain ){
                int amount = unitBufferLength <= remain ? unitBufferLength : (int)remain;
                receiver->push( bufferLeft, bufferRight, amount, 0 );
                remain -= amount;
            }
            delete [] bufferLeft;
            delete [] bufferRight;
        }
    };

    /**
     * @brief 常に同じ値の DCは計を出力する、PassiveAudioInput の実装
     */
    class PassiveAudioInputStub : public PassiveAudioInput{
    private:
        double value;

    public:
        explicit PassiveAudioInputStub( int sampleRate, double value ) :
            PassiveAudioInput( sampleRate )
        {
            this->value = value;
        }

        void pull( double *left, double *right, int length ){
            for( int i = 0; i < length; i++ ){
                left[i] = value;
                right[i] = value;
            }
        }
    };

    /**
     * @brief 受け取ったオーディオ波形をメモリーにため続ける、AudioOutput の実装
     */
    class MemoryAudioOutput : public AudioOutput{
    private:
        vector<double> bufferLeft;
        vector<double> bufferRight;

    public:
        explicit MemoryAudioOutput( int sampleRate ) :
            AudioOutput( sampleRate )
        {
        }

        uint64_t getBufferLength(){
            return (uint64_t)bufferLeft.size();
        }

        void getResult( double *left, double *right, uint64_t length ){
            uint64_t size = getBufferLength();
            for( uint64_t i = 0; i < size; i++ ){
                left[i] = bufferLeft[i];
                right[i] = bufferRight[i];
            }
        }

        void push( double *left, double *right, int length, int offset ){
            for( int i = 0; i < length; i++ ){
                bufferLeft.push_back( left[i + offset] );
                bufferRight.push_back( right[i + offset] );
            }
        }

        void flush(){
        }
    };

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
