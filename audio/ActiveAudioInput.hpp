/**
 * ActiveAudioInput.hpp
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
#ifndef __ActiveAudioInput_hpp__
#define __ActiveAudioInput_hpp__

#include "Receiver.hpp"
#include <stdint.h>

namespace cadencii{
namespace audio{

    /**
     * @brief ActiveAudioInput は、オーディオ波形を生成するオブジェクトを表す抽象クラスです
     */
    class ActiveAudioInput : public AudioUnit{
    public:
        explicit ActiveAudioInput( int sampleRate ) :
            AudioUnit( sampleRate )
        {
        }

        /**
         * @brief オーディオ波形の受け取り先を設定する
         * @param[in] オーディオ波形の受け取り先
         */
        virtual void setReceiver( Receiver *receiver ) = 0;

        /**
         * @brief オーディオ波形の生成を開始する
         * @param[in] length 生成するサンプル数
         */
        virtual void start( uint64_t length ) = 0;
    };

}
}

#endif
