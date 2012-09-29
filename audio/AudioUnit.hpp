/**
 * AudioUnit.hpp
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
#ifndef __AudioUnit_hpp__
#define __AudioUnit_hpp__

namespace cadencii{
namespace audio{

    class AudioUnit{
    protected:
        int sampleRate;

    public:
        explicit AudioUnit( int sampleRate ){
            this->sampleRate = sampleRate;
        }
    };

}
}

#endif
