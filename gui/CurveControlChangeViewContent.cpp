/**
 * CurveControlChangeViewContent.cpp
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
#include <sstream>
#include <math.h>
#include <QScrollArea>
#include <QPainter>
#include "vsq/Timesig.hpp"
#include "gui/CurveControlChangeView.h"
#include "gui/CurveControlChangeViewContent.h"

namespace cadencii{
    using namespace std;
    using namespace VSQ_NS;

    CurveControlChangeViewContent::CurveControlChangeViewContent(QWidget *parent) :
        QWidget(parent)
    {
        this->track = NULL;
        this->pixelPerTick = 0.2;
        mutex = NULL;

        this->defaultTimesigList.push( Timesig( 4, 4, 0 ) );
        this->measureLineIterator = new MeasureLineIterator( &defaultTimesigList );
        this->setMouseTracking( true );

        this->setMinimumWidth( 5000 );
    }

    CurveControlChangeViewContent::~CurveControlChangeViewContent(){
        delete measureLineIterator;
    }

    QRect CurveControlChangeViewContent::getPaintArea(){
        QScrollArea *scroll = (QScrollArea *)this->parent();
        if( scroll ){
            QRect rect = scroll->childrenRect();
            int x = -rect.x() - 1;
            int y = -rect.y() - 1;
            int width = scroll->width() + 2;
            int height = scroll->height() + 2;
            return QRect( x, y, width, height );
        }else{
            return QRect( -1, -1, this->width() + 2, this->height() + 2 );
        }
    }

    double CurveControlChangeViewContent::getTickFromX( int x ){
        return (x - 5) / pixelPerTick;
    }

    QRect CurveControlChangeViewContent::getVisibleArea(){
        QRect rect = this->getPaintArea();
        return QRect( rect.x() + 1, rect.y() + 1, rect.width() - 2, rect.height() - 2 );
    }

    int CurveControlChangeViewContent::getXFromTick( tick_t tick ){
        return (int)(tick * pixelPerTick) + 5;
    }

    void CurveControlChangeViewContent::mouseMoveEvent( QMouseEvent *e ){
        this->controlChangeView->repaint();
        QWidget::mouseMoveEvent( e );
    }

    void CurveControlChangeViewContent::paintBackground( QPainter *g, QRect visibleArea ){
    }

    void CurveControlChangeViewContent::paintEvent( QPaintEvent * ){
        setMinimumHeight( controlChangeView->height() );
        QPainter p( this );

        QRect visibleArea = this->getPaintArea();

        paintBackground( &p, visibleArea );
        paintMeasureLines( &p, visibleArea );
        if( mutex ){
            mutex->lock();
            paintItems( &p, visibleArea );
            mutex->unlock();
        }else{
            paintItems( &p, visibleArea );
        }
        paintSongPosition( &p, visibleArea );
    }

    void CurveControlChangeViewContent::paintItems( QPainter *g, QRect visibleArea ){
    }

    void CurveControlChangeViewContent::paintMeasureLines( QPainter *g, QRect visibleArea ){
        int top = controlChangeView->geometry().top();
        int bottom = controlChangeView->geometry().bottom();
        int left = visibleArea.x();
        int right = left + visibleArea.width();
        tick_t tickAtScreenRight = (tick_t)getTickFromX( right );
        measureLineIterator->reset( tickAtScreenRight );

        QColor barColor( 161, 157, 136 );
        QColor beatColor( 209, 204, 172 );
        while( measureLineIterator->hasNext() ){
            MeasureLine line = measureLineIterator->next();
            int x = getXFromTick( line.tick );
            if( x < left ){
                continue;
            }else if( right < x ){
                break;
            }
            if( line.isBorder ){
                g->setPen( barColor );
            }else{
                g->setPen( beatColor );
            }
            g->drawLine( x, top, x, bottom );
        }
    }

    void CurveControlChangeViewContent::paintSongPosition( QPainter *g, QRect visibleArea ){
        tick_t songPosition = controlChangeView->controllerAdapter->getSongPosition();
        int x = getXFromTick( songPosition );
        g->setPen( QColor( 0, 0, 0 ) );
        g->drawLine( x, visibleArea.top(), x, visibleArea.bottom() );
        g->setPen( QColor( 0, 0, 0, 40 ) );
        g->drawLine( x - 1, visibleArea.top(), x - 1, visibleArea.bottom() );
        g->drawLine( x + 1, visibleArea.top(), x + 1, visibleArea.bottom() );
    }

    void CurveControlChangeViewContent::setTrack( Track *track ){
        this->track = track;
    }

    void CurveControlChangeViewContent::setMutex( QMutex *mutex ){
        this->mutex = mutex;
    }

    void CurveControlChangeViewContent::setTimesigList( TimesigList *timesigList ){
        MeasureLineIterator *previous = measureLineIterator;
        this->timesigList = timesigList;
        measureLineIterator = new MeasureLineIterator( this->timesigList );
        if( previous ){
            delete previous;
        }
    }

    void CurveControlChangeViewContent::setControlChangeView( CurveControlChangeView *controlChangeView ){
        this->controlChangeView = controlChangeView;
    }

}
