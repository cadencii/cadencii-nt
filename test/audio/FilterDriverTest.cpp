#include "../Util.hpp"
#include "../../audio/FilterDriver.hpp"
#include "MemoryAudioOutput.hpp"
#include "PassiveAudioInputStub.hpp"

using namespace std;
using namespace cadencii::audio;

class AmplifierStub : public AudioFilter{
private:
    double amplify;
    Receiver *receiver;

public:
    explicit AmplifierStub( int sampleRate, double amplify ) :
        AudioFilter( sampleRate ), receiver( 0 )
    {
        this->amplify = amplify;
    }

    void setReceiver( Receiver *receiver ){
        this->receiver = receiver;
    }

    void push( double *left, double *right, int length, int offset ){
        if( !receiver ) return;
        for( int i = i = 0; i < length; i++ ){
            left[i + offset] *= amplify;
            right[i + offset] *= amplify;
        }
        receiver->push( left, right, length, offset );
    }

    void flush(){
    }
};

class ReduceSampleFilter : public AudioFilter{
private:
    uint64_t outgoingLength;
    uint64_t incomingLength;
    Receiver *receiver;

public:
    explicit ReduceSampleFilter( int sampleRate ) :
        AudioFilter( sampleRate )
    {
        outgoingLength = 0;
        incomingLength = 0;
    }

    void push( double *left, double *right, int length, int offset ){
        vector<double> resultLeft;
        vector<double> resultRight;
        for( int i = 0; i < length; i++ ){
            uint64_t incomingIndex = incomingLength + i;
            if( incomingIndex % 2 == 0 ){
                resultLeft.push_back( left[i + offset] );
                resultRight.push_back( right[i + offset] );
            }
        }

        receiver->push( resultLeft.data(), resultRight.data(), resultLeft.size(), 0 );

        incomingLength += length;
        outgoingLength += resultLeft.size();
    }

    void flush(){
    }

    void setReceiver( Receiver *receiver ){
        this->receiver = receiver;
    }
};

class AddSampleFilter : public AudioFilter{
private:
    Receiver *receiver;

public:
    explicit AddSampleFilter( int sampleRate ) :
        AudioFilter( sampleRate )
    {
    }

    void push( double *left, double *right, int length, int offset ){
        receiver->push( left, right, length, offset );
        receiver->push( left, right, length, offset );
    }

    void flush(){
        receiver->flush();
    }

    void setReceiver( Receiver *receiver ){
        this->receiver = receiver;
    }
};

class FilterDriverTest : public CppUnit::TestCase{
public:
    void test(){
        const int sampleRate = 44100;
        const double value = 1.0;
        const double amplify = 2.0;
        PassiveAudioInputStub input( sampleRate, value );
        AmplifierStub filter( sampleRate, amplify );
        FilterDriver driver( sampleRate, &filter );
        driver.setSender( &input );

        const int length = 23;
        double *left = new double[length];
        double *right = new double[length];
        driver.pull( left, right, length );

        for( int i = 0; i < length; i++ ){
            CPPUNIT_ASSERT_EQUAL( value * amplify, left[i] );
            CPPUNIT_ASSERT_EQUAL( value * amplify, right[i] );
        }

        delete [] left;
        delete [] right;
    }

    void testWhenFilterDecreaseSampleRate(){
        const int sampleRate = 44100;
        const double value = 1.0;
        PassiveAudioInputStub input( sampleRate, value );
        ReduceSampleFilter filter( sampleRate );
        FilterDriver driver( sampleRate, &filter );
        driver.setSender( &input );

        const int length = 23;
        double *left = new double[length];
        double *right = new double[length];
        driver.pull( left, right, length );

        for( int i = 0; i < length; i++ ){
            ostringstream os;
            os << "#" << i;
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "left" + os.str(), value, left[i] );
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "right" + os.str(), value, right[i] );
        }

        delete [] left;
        delete [] right;
    }

    void testWhenFilterIncreaseSampleRate(){
        const int sampleRate = 44100;
        const double value = 1.0;
        PassiveAudioInputStub input( sampleRate, value );
        AddSampleFilter filter( sampleRate );
        FilterDriver driver( sampleRate, &filter );
        driver.setSender( &input );

        const int length = 29;
        double *left = new double[length];
        double *right = new double[length];

        driver.pull( left, right, length );
        for( int i = 0; i < length; i++ ){
            ostringstream os;
            os << "#" << i;
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "left" + os.str(), value, left[i] );
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "right" + os.str(), value, right[i] );
        }

        delete [] left;
        delete [] right;
    }

    CPPUNIT_TEST_SUITE( FilterDriverTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST( testWhenFilterDecreaseSampleRate );
    CPPUNIT_TEST( testWhenFilterIncreaseSampleRate );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( FilterDriverTest );
