/**
 * AudioSender.hpp
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
#ifndef __cadencii_audio_Sender_hpp__
#define __cadencii_audio_Sender_hpp__

#include "AudioUnit.hpp"

namespace cadencii {
namespace audio {

    /**
     * @brief オーディオ波形を要求に応じて生成するオブジェクトを表す抽象クラス
     */
    class AudioSender : public AudioUnit {
    public:
        explicit AudioSender(int sampleRate) :
            AudioUnit(sampleRate) {
        }

        /**
         * @brief オーディオ波形の生成を要求し、バッファーに受け取る
         * @param[out] left 左チャンネルバッファー
         * @param[out] right 右チャンネルのバッファー
         * @param[in] length 要求する波形の長さ
         */
        virtual void pull(double *left, double *right, int length) = 0;
    };
}
}

#endif
