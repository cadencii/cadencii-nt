#ifndef __cadencii_audio_MemoryAudioOutput_hpp__
#define __cadencii_audio_MemoryAudioOutput_hpp__

#include "../../audio/AudioOutput.hpp"
#include <vector>
#include <stdint.h>

/**
 * @brief 受け取ったオーディオ波形をメモリーにため続ける、AudioOutput の実装
 */
class MemoryAudioOutput : public cadencii::audio::AudioOutput{
private:
    std::vector<double> bufferLeft;
    std::vector<double> bufferRight;

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

#endif
