/**
 * AudioReceiver.hpp
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
#ifndef __cadencii_audio_AudioReceiver_hpp__
#define __cadencii_audio_AudioReceiver_hpp__

#include "AudioUnit.hpp"

namespace cadencii{
namespace audio{

    /**
     * @brief オーディオ波形を受信するオブジェクトを表す抽象クラス
     */
    class AudioReceiver : public AudioUnit{
    public:
        explicit AudioReceiver( int sampleRate ) :
            AudioUnit( sampleRate )
        {
        }

        /**
         * @brief 波形を Receiver のインスタンスに渡す
         * @param[in] 左チャンネルのデータ
         * @param[in] 右チャンネルのデータ
         * @param[in] データの個数
         * @param[in] オフセット。データの offset 個目からのデータを渡す
         */
        virtual void push( double *left, double *right, int length, int offset = 0 ) = 0;

        /**
         * @brief バッファーなどにたまっているオーディオ波形をすべて処理し、バッファーなどを空にする
         */
        virtual void flush() = 0;
    };

}
}

#endif
