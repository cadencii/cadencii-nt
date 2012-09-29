/**
 * SampleRateLinearConverter.hpp
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
#ifndef __cadencii_audio_SampleRateConverter_hpp__
#define __cadencii_audio_SampleRateConverter_hpp__

#include "AudioFilter.hpp"
#include <stdint.h>
#include <cmath>

namespace cadencii{
namespace audio{

    /**
     * @brief サンプリングレートを変換するフィルター
     */
    class SampleRateLinearConverter : public AudioFilter{
    protected:
        Receiver *receiver;
        /**
         * @brief 変換前のサンプリングレート
         */
        int bRate;
        /**
         * @brief 変換後のサンプリングレート
         */
        int aRate;
        /**
         * @brief 変換後の、push 済みのサンプル数
         */
        uint64_t aFinished;
        /**
         * @brief 受け取ったサンプル数
         */
        uint64_t bFinished;
        /**
         * @brief 内部バッファーの長さ
         */
        int unitBufferLength;
        /**
         * @brief 内部バッファー
         */
        double *bufferLeft;
        /**
         * @brief 内部バッファー
         */
        double *bufferRight;

    public:
        /**
         * @brief 初期化
         * @param[in] convertFrom 変換前のサンプリングレート(sec^-1)
         * @param[in] convertTo 変換後のサンプリングレート(sec^-1)
         */
        explicit SampleRateLinearConverter( int convertFrom, int convertTo ) :
            AudioFilter( convertTo ), receiver( 0 )
        {
            bRate = convertFrom;
            aRate = convertTo;
            bFinished = 0;
            aFinished = 0;
            unitBufferLength = 4096;
            bufferLeft = new double[unitBufferLength];
            bufferRight = new double[unitBufferLength];
        }

        ~SampleRateLinearConverter(){
            delete [] bufferLeft;
            delete [] bufferRight;
        }

        void setReceiver( Receiver *receiver ){
            this->receiver = receiver;
        }

        void flush(){/* do nothing */}

        void push( double *left, double *right, int length, int offset ){
            if( !receiver ) return;

            int bufferIndex = 0;

            uint64_t aEndSample = (uint64_t)std::floor( (bFinished + length - 1) / (double)bRate * aRate );
            for( uint64_t aSample = aFinished; aSample <= aEndSample; ++aSample ){
                uint64_t bSample = (uint64_t)std::floor( aSample / (double)aRate * bRate );
                double deltaSecond = aSample / (double)aRate - bSample / (double)bRate;
                {
                    // left channenl
                    double y_ip1 = left[bSample + 1 - bFinished - offset];
                    double y_i = left[bSample - bFinished - offset];
                    double a = (y_ip1 - y_i) * bRate;
                    double value = y_i + a * deltaSecond;
                    bufferLeft[bufferIndex] = value;
                }
                {
                    // right channel
                    double y_ip1 = right[bSample + 1 - bFinished - offset];
                    double y_i = right[bSample - bFinished - offset];
                    double a = (y_ip1 - y_i) * bRate;
                    double value = y_i + a * deltaSecond;
                    bufferRight[bufferIndex] = value;
                }
                bufferIndex++;

                // flush buffers
                if( unitBufferLength == bufferIndex ){
                    bufferIndex = 0;
                    receiver->push( bufferLeft, bufferRight, unitBufferLength, 0 );
                }
            }

            // flush buffers
            if( 0 < bufferIndex ){
                receiver->push( bufferLeft, bufferRight, bufferIndex, 0 );
            }

            bFinished += length;
            aFinished = aEndSample - 1;
        }
    };

}
}

#endif
