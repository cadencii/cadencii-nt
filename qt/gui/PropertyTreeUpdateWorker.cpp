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
