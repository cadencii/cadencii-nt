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
#include <QPainter>
#include <QScrollBar>
#include "vsq/Timesig.hpp"
#include "gui/CurveControlChangeView.h"
#include "gui/CurveControlChangeViewContent.h"

namespace cadencii{
    using namespace std;
    using namespace VSQ_NS;

    CurveControlChangeViewContent::CurveControlChangeViewContent( QWidget *parent ) :
        QGraphicsView( parent )
    {
        deconstructStarted = false;
        scene = new QGraphicsScene();
        setScene( scene );
        sequence = 0;
        mutex = 0;

        this->defaultTimesigList.push( Timesig( 4, 4, 0 ) );
        this->measureLineIterator = new MeasureLineIterator( &defaultTimesigList );
        this->setMouseTracking( true );
        this->setAlignment( Qt::AlignLeft | Qt::AlignTop );

        scene->setSceneRect( 0, 0, 5000, height() );
    }

    CurveControlChangeViewContent::~CurveControlChangeViewContent(){
        deconstructStarted = true;
        delete measureLineIterator;
        delete scene;
    }

    QRect CurveControlChangeViewContent::getVisibleArea(){
        QRect result;
        QScrollBar *horizontalScroll = horizontalScrollBar();
        QScrollBar *verticalScroll = verticalScrollBar();
        if( sequence ){
            tick_t totalClocks = sequence->getTotalClocks();
            double virtualScreenWidth = controlChangeView->controllerAdapter->getXFromTick( totalClocks );
            int x = (int)((horizontalScroll->value() - horizontalScroll->minimum()) * virtualScreenWidth / (horizontalScroll->maximum() + horizontalScroll->pageStep() - horizontalScroll->minimum()));

            double virtualScreenHeight = height();
            int y = (int)((verticalScroll->value() - verticalScroll->minimum()) * virtualScreenHeight / (verticalScroll->maximum() + verticalScroll->pageStep() - verticalScroll->minimum()));

            int width = horizontalScroll->width();
            int height = verticalScroll->height();
            result = QRect( x, y, width, height );
        }else{
            result = QRect( 0, 0, horizontalScroll->width(), verticalScroll->height() );
        }
        return result;
    }

    void CurveControlChangeViewContent::mouseMoveEvent( QMouseEvent *e ){
        this->controlChangeView->repaint();
        QWidget::mouseMoveEvent( e );
    }

    void CurveControlChangeViewContent::drawForeground( QPainter *painter, const QRectF &rect ){
        if( !sequence || !scene ) return;
        tick_t totalClocks = sequence->getTotalClocks();
        int sceneWidth = controlChangeView->controllerAdapter->getXFromTick( totalClocks );
        int sceneHeight = controlChangeView->height();
        scene->setSceneRect( 0, 0, sceneWidth, sceneHeight );

        QRect visibleArea( (int)rect.x(), (int)rect.y(), (int)rect.width(), (int)rect.height() );

        paintBackground( painter, visibleArea );
        paintMeasureLines( painter, visibleArea );
        if( mutex ){
            mutex->lock();
            paintItems( painter, visibleArea );
            mutex->unlock();
        }else{
            paintItems( painter, visibleArea );
        }
        paintSongPosition( painter, visibleArea );
    }

    void CurveControlChangeViewContent::paintBackground( QPainter *g, QRect visibleArea ){

    }

    void CurveControlChangeViewContent::paintItems( QPainter *g, QRect visibleArea ){
    }

    void CurveControlChangeViewContent::paintMeasureLines( QPainter *g, QRect visibleArea ){
        int top = controlChangeView->geometry().top();
        int bottom = controlChangeView->geometry().bottom();
        int left = visibleArea.x();
        int right = left + visibleArea.width();
        tick_t tickAtScreenRight = (tick_t)controlChangeView->controllerAdapter->getTickFromX( right );
        measureLineIterator->reset( tickAtScreenRight );

        QColor barColor( 161, 157, 136 );
        QColor beatColor( 209, 204, 172 );
        while( measureLineIterator->hasNext() ){
            MeasureLine line = measureLineIterator->next();
            int x = controlChangeView->controllerAdapter->getXFromTick( line.tick );
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
        int x = controlChangeView->controllerAdapter->getXFromTick( songPosition );
        g->setPen( QColor( 0, 0, 0 ) );
        g->drawLine( x, visibleArea.top(), x, visibleArea.bottom() );
        g->setPen( QColor( 0, 0, 0, 40 ) );
        g->drawLine( x - 1, visibleArea.top(), x - 1, visibleArea.bottom() );
        g->drawLine( x + 1, visibleArea.top(), x + 1, visibleArea.bottom() );
    }

    void CurveControlChangeViewContent::setSequence( Sequence *sequence ){
        this->sequence = sequence;
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

    int CurveControlChangeViewContent::getSceneWidth(){
        return (int)scene->sceneRect().width();
    }

    void CurveControlChangeViewContent::scrollContentsBy( int dx, int dy ){
        QGraphicsView::scrollContentsBy( dx, dy );
        if( deconstructStarted ) return;
        if( dx && controlChangeView ){
            controlChangeView->notifyHorizontalScroll();
        }
    }

}
