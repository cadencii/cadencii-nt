/**
 * Controller.hpp
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
#ifndef __Controller_hpp__
#define __Controller_hpp__

#include "MainView.hpp"
#include "TrackView.hpp"

namespace cadencii{

    class Controller{
    private:
        TrackView *trackView;
        MainView *mainView;

    public:
        Controller()
            : trackView( 0 ), mainView( 0 )
        {
        }

        void setTrackView( TrackView *trackView ){
            this->trackView = trackView;
            if( mainView ){
                mainView->setTrackView( trackView );
            }
        }

        void setMainView( MainView *mainView ){
            this->mainView = mainView;
            if( this->mainView && this->trackView ){
                this->mainView->setTrackView( this->trackView );
            }
        }
    };

}

#endif
