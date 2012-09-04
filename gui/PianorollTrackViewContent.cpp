/**
 * PianorollTrackViewContent.cpp
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
#include <QScrollBar>
#include "vsq/Timesig.hpp"
#include "gui/PianorollTrackView.h"
#include "gui/PianorollTrackViewContent.h"

namespace cadencii{
    using namespace std;
    using namespace VSQ_NS;

    PianorollTrackViewContent::PianorollTrackViewContent(QWidget *parent) :
        QGraphicsView(parent)
    {
        deconstructStarted = false;
        scene = new QGraphicsScene();
        setScene( scene );
        this->sequence = NULL;
        this->trackHeight = DEFAULT_TRACK_HEIGHT;
        mutex = NULL;

        this->defaultTimesigList.push( Timesig( 4, 4, 0 ) );
        this->measureLineIterator = new MeasureLineIterator( &defaultTimesigList );
        this->setMouseTracking( true );
        this->setAlignment( Qt::AlignLeft | Qt::AlignTop );

        scene->setSceneRect( 0, 0, 5000, getMinimumHeight() );
    }

    PianorollTrackViewContent::~PianorollTrackViewContent(){
        deconstructStarted = true;
        delete measureLineIterator;
        delete scene;
    }

    int PianorollTrackViewContent::getMinimumHeight(){
        return this->trackHeight * (NOTE_MAX - NOTE_MIN + 1);
    }

    int PianorollTrackViewContent::getNoteModuration( int noteNumber ){
        return ((noteNumber % 12) + 12) % 12;
    }

    int PianorollTrackViewContent::getNoteNumberFromY( int y, int trackHeight ){
        return NOTE_MAX - (int)::floor( (double)(y / trackHeight) );
    }

    int PianorollTrackViewContent::getNoteOctave( int noteNumber ){
        int modura = getNoteModuration( noteNumber );
        return (noteNumber - modura) / 12 - 2;
    }

    int PianorollTrackViewContent::getTrackHeight(){
        return trackHeight;
    }

    QRect PianorollTrackViewContent::getVisibleArea(){
        QRect result;
        QScrollBar *horizontalScroll = horizontalScrollBar();
        QScrollBar *verticalScroll = verticalScrollBar();
        if( sequence ){
            tick_t totalClocks = sequence->getTotalClocks();
            int virtualScreenWidth = pianoroll->controllerAdapter->getXFromTick( totalClocks );
            int x = (horizontalScroll->value() - horizontalScroll->minimum()) * virtualScreenWidth / (horizontalScroll->maximum() + horizontalScroll->pageStep() - horizontalScroll->minimum());

            int virtualScreenHeight = getMinimumHeight();
            int y = (verticalScroll->value() - verticalScroll->minimum()) * virtualScreenHeight / (verticalScroll->maximum() + verticalScroll->pageStep() - verticalScroll->minimum());

            int width = horizontalScroll->width();
            int height = verticalScroll->height();
            result = QRect( x, y, width, height );
        }else{
            result = QRect( 0, 0, horizontalScroll->width(), verticalScroll->height() );
        }
        return result;
    }

    int PianorollTrackViewContent::getYFromNoteNumber( int noteNumber, int trackHeight ){
        return (NOTE_MAX - noteNumber) * trackHeight;
    }

    void PianorollTrackViewContent::mouseMoveEvent( QMouseEvent *e ){
        this->pianoroll->repaint();
        QWidget::mouseMoveEvent( e );
    }

    void PianorollTrackViewContent::paintBackground( QPainter *g, QRect visibleArea ){
        // 背景
        int height = getYFromNoteNumber( NOTE_MIN - 1, trackHeight ) - visibleArea.y();
        g->fillRect( visibleArea.x(), visibleArea.y(),
                     visibleArea.width(), height,
                     QColor( 240, 240, 240 ) );

        // 黒鍵
        for( int noteNumber = NOTE_MIN; noteNumber <= NOTE_MAX; noteNumber++ ){
            int y = getYFromNoteNumber( noteNumber, trackHeight );
            int modura = getNoteModuration( noteNumber );

            if( visibleArea.bottom() < y ){
                continue;
            }

            // 黒鍵
            if( modura == 1 || modura == 3 || modura == 6 || modura == 8 || modura == 10 ){
                g->fillRect( visibleArea.x(), y,
                             visibleArea.width(), trackHeight + 1,
                             QColor( 212, 212, 212 ) );
            }

            // 白鍵が隣り合う部分に境界線を書く
            if( modura == 11 || modura == 4 ){
                g->setPen( QColor( 210, 203, 173 ) );
                g->drawLine( visibleArea.left(), y,
                             visibleArea.right(), y );
            }

            if( y < visibleArea.y() ){
                break;
            }
        }
    }

    void PianorollTrackViewContent::drawForeground( QPainter *painter, const QRectF &rect ){
        int minimumHeight = this->getMinimumHeight();
        QRectF currentSceneRect = sceneRect();
        if( currentSceneRect.height() != minimumHeight ){
            scene->setSceneRect( currentSceneRect.x(), currentSceneRect.y(),currentSceneRect.width(), minimumHeight );
        }
        tick_t totalClocks = sequence->getTotalClocks();
        int width = pianoroll->controllerAdapter->getXFromTick( totalClocks );
        scene->setSceneRect( currentSceneRect.x(), currentSceneRect.y(), width, currentSceneRect.height() );

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

    void PianorollTrackViewContent::paintItems( QPainter *g, QRect visibleArea ){
        if( sequence == NULL ){
            return;
        }
        Event::List *list = sequence->track[1].getEvents();//TODO:1以外を選べるように
        int count = list->size();
        int height = trackHeight - 1;

        QColor fillColor = QColor( 181, 220, 86 );
        QColor borderColor = QColor( 125, 123, 124 );

        int visibleMinX = visibleArea.left();
        int visibleMaxX = visibleArea.right();
        int visibleMinY = visibleArea.top();
        int visibleMaxY = visibleArea.bottom();

        for( int i = 0; i < count; i++ ){
            Event item = list->get( i );
            if( item.type != EventType::NOTE ) continue;
            tick_t tick = item.clock;
            int x = pianoroll->controllerAdapter->getXFromTick( tick );
            int width = pianoroll->controllerAdapter->getXFromTick( tick + item.getLength() ) - x;

            if( visibleMinX <= x + width && x <= visibleMaxX ){
                int y = getYFromNoteNumber( item.note, trackHeight ) + 1;
                if( visibleMinY <= y + height && y <= visibleMaxY ){
                    g->fillRect( x, y, width, height, fillColor );
                    g->setPen( borderColor );
                    g->drawRect( x, y, width, height );

                    Lyric lyric = item.lyricHandle.getLyricAt( 0 );
                    g->setPen( QColor( 0, 0, 0 ) );
                    g->drawText( x + 1, y + trackHeight - 2,
                                 QString::fromUtf8( (lyric.phrase + " [" + lyric.getPhoneticSymbol() + "]").c_str() ) );
                }
            }
        }
    }

    void PianorollTrackViewContent::paintMeasureLines( QPainter *g, QRect visibleArea ){
        int top = visibleArea.y();
        int bottom = getYFromNoteNumber( NOTE_MIN - 1, trackHeight );
        int left = visibleArea.x();
        int right = left + visibleArea.width();
        tick_t tickAtScreenRight = (tick_t)pianoroll->controllerAdapter->getTickFromX( right );
        measureLineIterator->reset( tickAtScreenRight );

        QColor barColor( 161, 157, 136 );
        QColor beatColor( 209, 204, 172 );
        while( measureLineIterator->hasNext() ){
            MeasureLine line = measureLineIterator->next();
            int x = pianoroll->controllerAdapter->getXFromTick( line.tick );
            if( x < left ){
                continue;
            }else if( right < x ){
                break;
            }
            if( line.isBorder ){
                g->setPen( barColor );
            }
            g->drawLine( x, top, x, bottom );
            if( line.isBorder ){
                g->setPen( beatColor );
            }
        }
    }

    void PianorollTrackViewContent::paintSongPosition( QPainter *g, QRect visibleArea ){
        tick_t songPosition = pianoroll->controllerAdapter->getSongPosition();
        int x = pianoroll->controllerAdapter->getXFromTick( songPosition );
        g->setPen( QColor( 0, 0, 0 ) );
        g->drawLine( x, visibleArea.top(), x, visibleArea.bottom() );
        g->setPen( QColor( 0, 0, 0, 40 ) );
        g->drawLine( x - 1, visibleArea.top(), x - 1, visibleArea.bottom() );
        g->drawLine( x + 1, visibleArea.top(), x + 1, visibleArea.bottom() );
    }

    void PianorollTrackViewContent::setSequence( Sequence *sequence ){
        this->sequence = sequence;
    }

    void PianorollTrackViewContent::setMutex( QMutex *mutex ){
        this->mutex = mutex;
    }

    void PianorollTrackViewContent::setTimesigList( TimesigList *timesigList ){
        MeasureLineIterator *previous = measureLineIterator;
        this->timesigList = timesigList;
        measureLineIterator = new MeasureLineIterator( this->timesigList );
        if( previous ){
            delete previous;
        }
    }

    void PianorollTrackViewContent::setTrackHeight( int trackHeight ){
        this->trackHeight = trackHeight;
        QRectF rect = sceneRect();
        scene->setSceneRect( rect.x(), rect.y(), rect.width(), getMinimumHeight() );
    }

    void PianorollTrackViewContent::setPianoroll( PianorollTrackView *pianoroll ){
        this->pianoroll = pianoroll;
    }

    void PianorollTrackViewContent::scrollContentsBy( int dx, int dy ){
        QGraphicsView::scrollContentsBy( dx, dy );
        if( deconstructStarted ) return;
        if( dy && pianoroll ){
            pianoroll->notifyVerticalScroll();
        }
        if( dx && pianoroll ){
            pianoroll->notifyHorizontalScroll();
        }
    }

    int PianorollTrackViewContent::getSceneWidth(){
        return (int)scene->sceneRect().width();
    }

}
