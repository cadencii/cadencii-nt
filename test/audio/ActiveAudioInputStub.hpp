#ifndef __cadencii_audio_ActiveAudioInputStub_hpp__
#define __cadencii_audio_ActiveAudioInputStub_hpp__

#include "../../audio/ActiveAudioInput.hpp"

/**
 * @brief 常に同じ値の DC波形を出力する、ActiveAudioInput の実装
 */
class ActiveAudioInputStub : public cadencii::audio::ActiveAudioInput{
private:
    double value;
    cadencii::audio::Receiver *receiver;

public:
    explicit ActiveAudioInputStub( int sampleRate, double value ) :
        ActiveAudioInput( sampleRate ), receiver( 0 )
    {
        this->value = value;
    }

    void setReceiver( cadencii::audio::Receiver *receiver ){
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

#endif
