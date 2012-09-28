#ifndef __cadencii_audio_PassiveAudioInputStub_hpp__
#define __cadencii_audio_PassiveAudioInputStub_hpp__

#include "../../audio/PassiveAudioInput.hpp"

/**
 * @brief 常に同じ値の DCは計を出力する、PassiveAudioInput の実装
 */
class PassiveAudioInputStub : public cadencii::audio::PassiveAudioInput{
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

#endif
