/**
 * MainView.hpp
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __MainView_hpp__
#define __MainView_hpp__

#include "TrackView.hpp"

namespace cadencii{

    class MainView{
    public:
        virtual void setTrackView( TrackView *trackView ) = 0;
    };

}

#endif
