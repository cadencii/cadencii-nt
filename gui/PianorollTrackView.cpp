/**
 * PianorollTrackView.cpp
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
#include <QScrollBar>
#include "gui/PianorollTrackView.h"
#include "gui/PianorollTrackViewContent.h"
#include "ui_PianorollTrackView.h"

namespace cadencii{
    using namespace std;
    using namespace VSQ_NS;

    PianorollTrackView::PianorollTrackView( QWidget *parent ) :
        QWidget( parent ),
        ui( new Ui::PianorollTrackView )
    {
        ui->setupUi( this );
        ui->scrollArea->setBackgroundRole( QPalette::Dark );
        ui->scrollArea->setPianoroll( this );
        ui->content->setPianoroll( this );
        ui->keyboard->setPianoroll( this );
        ui->keyboard->notifyVerticalScroll( 0 );
    }

    void PianorollTrackView::ensureNoteVisible( tick_t tick, tick_t length, int noteNumber ){
        int left = ui->content->getXFromTick( tick );
        int right = ui->content->getXFromTick( tick + length );
        int trackHeight = ui->content->getTrackHeight();
        int top = ui->content->getYFromNoteNumber( noteNumber, trackHeight );
        int bottom = top + trackHeight;

        QRect visibleArea = ui->content->getVisibleArea();
        QScrollBar *horizontalScrollBar = ui->scrollArea->horizontalScrollBar();
        QScrollBar *verticalScrollBar = ui->scrollArea->verticalScrollBar();
        int dx = 0;
        int newValue = verticalScrollBar->value();
        if( visibleArea.right() < right ){
            dx = ui->scrollArea->width() - (right - left);
        }else if( left < visibleArea.left() ){
            dx = -ui->scrollArea->width() + (right - left);
        }
        if( top < visibleArea.top() || visibleArea.bottom() < bottom ){
            newValue = (bottom + top) / 2 - visibleArea.height() / 2;
        }
        if( dx ){
            int value = horizontalScrollBar->value() + dx;
            if( value < horizontalScrollBar->minimum() ){
                horizontalScrollBar->setValue( horizontalScrollBar->minimum() );
            }else if( horizontalScrollBar->maximum() < value ){
                horizontalScrollBar->setValue( horizontalScrollBar->maximum() );
            }else{
                horizontalScrollBar->setValue( value );
            }
        }
        if( verticalScrollBar->value() != newValue ){
            if( newValue < verticalScrollBar->minimum() ){
                verticalScrollBar->setValue( verticalScrollBar->minimum() );
            }else if( verticalScrollBar->maximum() < newValue ){
                verticalScrollBar->setValue( verticalScrollBar->maximum() );
            }else{
                verticalScrollBar->setValue( newValue );
            }
        }
    }

    tick_t PianorollTrackView::getSongPosition(){
        return ui->content->getSongPosition();
    }

    void PianorollTrackView::notifyVerticalScroll(){
        QRect rect = ui->content->getVisibleArea();
        ui->keyboard->notifyVerticalScroll( rect.y() );
    }

    void PianorollTrackView::repaint(){
        ui->content->repaint();
        ui->keyboard->repaint();
        QWidget::repaint();
    }

    void PianorollTrackView::setTrack( Track *track ){
        ui->content->setTrack( track );
    }

    void PianorollTrackView::setMusicalPartOffset( tick_t musicalPartOffset ){
        ui->content->setMusicalPartOffset( musicalPartOffset );
    }

    void PianorollTrackView::setMutex( QMutex *mutex ){
        ui->content->setMutex( mutex );
    }

    void PianorollTrackView::setSongPosition( tick_t songPosition, bool autoScroll ){
        tick_t lastSongPosition = ui->content->getSongPosition();
        ui->content->setSongPosition( songPosition );
        if( autoScroll ){
            int x = ui->content->getXFromTick( songPosition );
            QRect visibleArea = ui->content->getVisibleArea();
            QScrollBar *scrollBar = ui->scrollArea->horizontalScrollBar();
            int dx = 0;
            if( lastSongPosition < songPosition ){
                // 右へ移動した場合
                if( visibleArea.right() < x ){
                    dx = ui->scrollArea->width() - (x - visibleArea.right());
                }
            }else{
                // 左へ移動した場合
                if( x < visibleArea.left() ){
                    dx = -ui->scrollArea->width() + (visibleArea.left() - x);
                }
            }
            if( dx ){
                int value = scrollBar->value() + dx;
                if( value < scrollBar->minimum() ){
                    scrollBar->setValue( scrollBar->minimum() );
                }else if( scrollBar->maximum() < value ){
                    scrollBar->setValue( scrollBar->maximum() );
                }else{
                    scrollBar->setValue( scrollBar->value() + dx );
                }
            }
        }
    }

    void PianorollTrackView::setTimesigList( TimesigList *timesigList ){
        ui->content->setTimesigList( timesigList );
    }

    void PianorollTrackView::setTrackHeight( int trackHeight ){
        ui->content->setTrackHeight( trackHeight );
        ui->keyboard->setTrackHeight( trackHeight );
    }

    QWidget *PianorollTrackView::getActualWidget(){
        return (QWidget *)this;
    }
}
