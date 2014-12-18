#ifndef __cadencii_audio_ActiveAudioInputStub_hpp__
#define __cadencii_audio_ActiveAudioInputStub_hpp__

#include "../../audio/AudioGenerator.hpp"
#include <cstdlib>
#include <iostream>

/**
 * @brief 初期化時に指定された波形を出力する、AudioGenerator の実装
 */
class AudioGeneratorStub : public cadencii::audio::AudioGenerator{
private:
    double *buffer;
    int length;
    std::shared_ptr<cadencii::audio::AudioReceiver> receiver_;

public:
    explicit AudioGeneratorStub( int sampleRate, double* buffer, int length )
        : AudioGenerator(sampleRate)
        , receiver_(nullptr)
    {
        this->buffer = buffer;
        this->length = length;
    }

    void setReceiver(std::shared_ptr<cadencii::audio::AudioReceiver> const& receiver) override
    {
        this->receiver_ = receiver;
    }

    void start( uint64_t length ){
        if( !receiver_ ) return;
        const int unitBufferLength = 2048;
        double *bufferLeft = new double[unitBufferLength];
        double *bufferRight = new double[unitBufferLength];

        // 指定された波形を push する
        int lengthFromBuffer = this->length <= length ? this->length : (int)length;
        int remainFromBuffer = lengthFromBuffer;
        uint64_t finished = 0;
        while( 0 < remainFromBuffer ){
            int draftAmount = 1 + rand() % (unitBufferLength - 1);
            int amount = remainFromBuffer <= draftAmount ? remainFromBuffer : draftAmount;
            for( int i = 0; i < amount; i++ ){
                bufferLeft[i] = this->buffer[finished + i];
                bufferRight[i] = this->buffer[finished + i];
            }
            receiver_->push( bufferLeft, bufferRight, amount, 0 );
            remainFromBuffer -= amount;
            finished += amount;
        }

        // 指定された波形が枯渇したら、無音の波形を送る
        int remainSilence = length - lengthFromBuffer;
        while( 0 < remainSilence ){
            int draftAmount = 1 + rand() % (unitBufferLength - 1);
            int amount = remainSilence <= draftAmount ? remainSilence : draftAmount;
            for( int i = 0; i < amount; i++ ){
                bufferLeft[i] = 0.0;
                bufferRight[i] = 0.0;
            }
            receiver_->push( bufferLeft, bufferRight, amount, 0 );
            remainSilence -= amount;
            finished += amount;
        }

        receiver_->flush();

        delete [] bufferLeft;
        delete [] bufferRight;
    }
};

#endif
