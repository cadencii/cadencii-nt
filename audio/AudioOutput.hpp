/**
 * AudioOutput.hpp
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
#ifndef __AudioOutput_hpp__
#define __AudioOutput_hpp__

#include "AudioReceiver.hpp"

namespace cadencii {
namespace audio {

    /**
     * @brief AudioOutput は、オーディオ波形の最終的に受け取り、何らかの出力を行うオブジェクトを表す抽象クラスです
     */
    class AudioOutput : public AudioReceiver {
    public:
        explicit AudioOutput(int sampleRate) :
            AudioReceiver(sampleRate) {
        }
    };
}
}

#endif
