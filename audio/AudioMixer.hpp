/**
 * AudioMixer.hpp
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2.0 as published by the Free Software Foundation.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __AudioMixer_hpp__
#define __AudioMixer_hpp__

#include "AudioReceiver.hpp"
#include "AudioSender.hpp"
#include <vector>

namespace cadencii{
namespace audio{

    /**
     * @brief AudioMixer は、複数のオーディオ波形をミックスするクラスです
     */
    class AudioMixer : public AudioReceiver{
    protected:
        AudioReceiver *receiver;
        std::vector<AudioSender *> sourceList;
        const static int unitBufferLength = 1024;
        double *bufferLeft;
        double *bufferRight;
        double *temporaryBufferLeft;
        double *temporaryBufferRight;

    public:
        explicit AudioMixer( int sampleRate ) :
            AudioReceiver( sampleRate ), receiver( 0 )
        {
            bufferLeft = new double[unitBufferLength];
            bufferRight = new double[unitBufferLength];
            temporaryBufferLeft = new double[unitBufferLength];
            temporaryBufferRight = new double[unitBufferLength];
        }

        ~AudioMixer(){
            delete [] bufferLeft;
            delete [] bufferRight;
            delete [] temporaryBufferLeft;
            delete [] temporaryBufferRight;
        }

        /**
         * @brief オーディオ波形の受け取り先を設定する
         * @param[in] オーディオ波形の受け取り先
         */
        void setReceiver( AudioReceiver *receiver ){
            this->receiver = receiver;
        }

        /**
         * @brief ミックスする音源を追加する
         * @param[in] sender ミックス対象の音源
         */
        void addSource( AudioSender *sender ){
            sourceList.push_back( sender );
        }

        void push( double *left, double *right, int length, int offset ){
            int remain = length;
            int finished = 0;

            while( 0 < remain ){
                int amount = unitBufferLength <= remain ? unitBufferLength : remain;
                for( int i = 0; i < amount; i++ ){
                    int index = i + offset + finished;
                    bufferLeft[i] = left[index];
                    bufferRight[i] = right[index];
                }
                for( int j = 0; j < sourceList.size(); j++ ){
                    sourceList[j]->pull( temporaryBufferLeft, temporaryBufferRight, amount );
                    for( int i = 0; i < amount; i++ ){
                        bufferLeft[i] += temporaryBufferLeft[i];
                        bufferRight[i] += temporaryBufferRight[i];
                    }
                }
                if( receiver ){
                    receiver->push( bufferLeft, bufferRight, amount, 0 );
                }
                remain -= amount;
                finished += amount;
            }
        }

        void flush(){
        }
    };

}
}

#endif
