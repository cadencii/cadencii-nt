/**
 * Controller.cpp
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
#include "Controller.hpp"
#include "vsq/VSQFileReader.hpp"
#include "vsq/FileInputStream.hpp"

namespace cadencii{

    Controller::Controller()
        : trackView( 0 ), mainView( 0 ), controlChangeView( 0 ), sequence( "Miku", 1, 4, 4, 500000 ), songPosition( 0 )
    {
    }

    void Controller::setTrackView( TrackView *trackView )throw(){
        this->trackView = trackView;
        if( this->trackView ){
            this->trackView->setControllerAdapter( this );
        }

        if( mainView ){
            mainView->setTrackView( this->trackView );
        }
    }

    void Controller::setControlChangeView( ControlChangeView *controlChangeView )throw(){
        this->controlChangeView = controlChangeView;
        if( this->controlChangeView ){
            this->controlChangeView->setControllerAdapter( this );
        }
        if( mainView ){
            mainView->setControlChangeView( this->controlChangeView );
        }
    }

    void Controller::setMainView( MainView *mainView )throw(){
        this->mainView = mainView;
        if( this->mainView ){
            this->mainView->setControllerAdapter( this );
        }

        if( this->mainView && this->trackView ){
            this->mainView->setTrackView( this->trackView );
        }
        if( this->mainView && this->controlChangeView ){
            this->mainView->setControlChangeView( this->controlChangeView );
        }
    }

    void Controller::openVSQFile( const ::std::string &filePath )throw(){
        VSQ_NS::VSQFileReader reader;
        VSQ_NS::FileInputStream stream( filePath );
        reader.read( sequence, &stream, "Shift_JIS" );
        stream.close();

        trackView->setTrack( &sequence.track[1] );
        controlChangeView->setTrack( &sequence.track[1] );
    }

    void Controller::drawOffsetChanged( void *sender, VSQ_NS::tick_t offset )throw(){
        if( sender == (void *)trackView && controlChangeView ){
            controlChangeView->setDrawOffset( offset );
        }else if( sender == (void *)controlChangeView && trackView ){
            trackView->setDrawOffset( offset );
        }
    }

    VSQ_NS::tick_t Controller::getSongPosition()throw(){
        return songPosition;
    }

}
