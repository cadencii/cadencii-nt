/**
 * AudioFilter.hpp
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
#ifndef __AudioFilter_hpp__
#define __AudioFilter_hpp__

#include "Receiver.hpp"

namespace cadencii{
namespace audio{

    /**
     * @brief AudioFilter は、オーディオ波形を加工するオブジェクトを表す抽象クラスです
     */
    class AudioFilter : public Receiver{
    public:
        explicit AudioFilter( int sampleRate ) :
            Receiver( sampleRate )
        {
        }

        /**
         * @brief オーディオ波形の受け取り先を設定する
         * @param[in] オーディオ波形の受け取り先
         */
        virtual void setReceiver( Receiver *receiver ) = 0;
    };

}
}

#endif
