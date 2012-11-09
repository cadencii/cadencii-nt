/**
 * AudioGenerator.hpp
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
#ifndef __cadencii_audio_AudioGenerator_hpp__
#define __cadencii_audio_AudioGenerator_hpp__

#include <stdint.h>
#include "AudioReceiver.hpp"

namespace cadencii {
namespace audio {

    /**
     * @brief AudioGenerator は、オーディオ波形を生成するオブジェクトを表す抽象クラスです
     */
    class AudioGenerator : public AudioUnit {
    public:
        explicit AudioGenerator(int sampleRate) :
            AudioUnit(sampleRate) {
        }

        /**
         * @brief オーディオ波形の受け取り先を設定する
         * @param[in] receiver オーディオ波形の受け取り先
         */
        virtual void setReceiver(AudioReceiver *receiver) = 0;

        /**
         * @brief オーディオ波形の生成を開始する
         * @param[in] length 生成するサンプル数
         */
        virtual void start(uint64_t length) = 0;
    };
}
}

#endif
