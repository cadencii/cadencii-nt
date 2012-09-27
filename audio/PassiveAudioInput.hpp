/**
 * PassiveAudioInput.hpp
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
#ifndef __PassiveAudioInput_hpp__
#define __PassiveAudioInput_hpp__

#include "Sender.hpp"

namespace cadencii{
namespace audio{

    /**
     * @brief PassiveAudioInput は、接続先の要求に応じて、オーディオ波形を生成するオブジェクトを表す抽象クラスです
     */
    class PassiveAudioInput : public Sender{
    public:
        explicit PassiveAudioInput( int sampleRate ) :
            Sender( sampleRate )
        {
        }
    };

}
}

#endif
