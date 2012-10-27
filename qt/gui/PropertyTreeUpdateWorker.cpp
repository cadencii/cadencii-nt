/**
 * PropertyTreeUpdateWorker.cpp
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
#include "PropertyTreeUpdateWorker.hpp"

namespace cadencii{

    PropertyTreeUpdateWorker::PropertyTreeUpdateWorker( ConcretePropertyView *parent ) :
        QThread( parent ), updateRequested( false ), parent( parent ), stopRequested( false )
    {
    }

    PropertyTreeUpdateWorker::~PropertyTreeUpdateWorker(){
        stopRequested = true;
        wait();
    }

    void PropertyTreeUpdateWorker::run(){
        while( !stopRequested ){
            mutex.lock();
            if( updateRequested ){
                emit callUpdateTree();
                updateRequested = false;
            }
            mutex.unlock();
            msleep( SLEEP_INTERVAL_MILLI_SECONDS );
        }
    }

    void PropertyTreeUpdateWorker::setControllerAdapter( ControllerAdapter *adapter ){
        controllerAdapter = adapter;
    }

    void PropertyTreeUpdateWorker::enqueueTreeUpdate(){
        mutex.lock();
        updateRequested = true;
        mutex.unlock();
    }
}
