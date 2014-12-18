#ifndef __cadencii_audio_PassiveAudioInputStub_hpp__
#define __cadencii_audio_PassiveAudioInputStub_hpp__

#include "../../audio/AudioSender.hpp"

/**
 * @brief 常に同じ値の DCは計を出力する、AudioSender の実装
 */
class AudioSenderStub : public cadencii::audio::AudioSender{
private:
    double value;

public:
    explicit AudioSenderStub( int sampleRate, double value ) :
        AudioSender( sampleRate )
    {
        this->value = value;
    }

    void pull( double *left, double *right, int length ) override {
        for( int i = 0; i < length; i++ ){
            left[i] = value;
            right[i] = value;
        }
    }
};

#endif
