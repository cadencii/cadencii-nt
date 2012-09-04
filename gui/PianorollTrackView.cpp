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
        ui->keyboard->setPianoroll( this );
        ui->keyboard->notifyVerticalScroll( 0 );
        controllerAdapter = 0;
    }

    PianorollTrackView::~PianorollTrackView(){
        delete ui;
    }

    void PianorollTrackView::ensureNoteVisible( tick_t tick, tick_t length, int noteNumber ){
        int left = controllerAdapter->getXFromTick( tick );
        int right = controllerAdapter->getXFromTick( tick + length );
        int trackHeight = ui->scrollArea->getTrackHeight();
        int top = ui->scrollArea->getYFromNoteNumber( noteNumber, trackHeight );
        int bottom = top + trackHeight;

        QRect visibleArea = ui->scrollArea->getVisibleArea();
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

    void PianorollTrackView::notifyVerticalScroll(){
        QRect rect = ui->scrollArea->getVisibleArea();
        ui->keyboard->notifyVerticalScroll( rect.y() );
    }

    void PianorollTrackView::notifyHorizontalScroll(){
        QRect visibleRect = ui->scrollArea->getVisibleArea();
        tick_t drawOffset = (tick_t)controllerAdapter->getTickFromX( visibleRect.x() );
        controllerAdapter->drawOffsetChanged( (TrackView *)this, drawOffset );
    }

    void PianorollTrackView::repaint(){
        ui->scrollArea->repaint();
        ui->keyboard->repaint();
        QWidget::repaint();
    }

    void PianorollTrackView::setSequence( Sequence *sequence ){
        ui->scrollArea->setSequence( sequence );
    }

    void PianorollTrackView::setMutex( QMutex *mutex ){
        ui->scrollArea->setMutex( mutex );
    }

    void PianorollTrackView::setTimesigList( TimesigList *timesigList ){
        ui->scrollArea->setTimesigList( timesigList );
    }

    void PianorollTrackView::setTrackHeight( int trackHeight ){
        ui->scrollArea->setTrackHeight( trackHeight );
        ui->keyboard->setTrackHeight( trackHeight );
    }

    void *PianorollTrackView::getWidget(){
        return (void *)this;
    }

    void PianorollTrackView::setDrawOffset( tick_t drawOffset ){
        int xScrollTo = -controllerAdapter->getXFromTick( drawOffset );
        QScrollBar *scrollBar = ui->scrollArea->horizontalScrollBar();
        int maxValue = scrollBar->maximum() + scrollBar->pageStep();
        int minValue = scrollBar->minimum();
        int contentWidth = (int)ui->scrollArea->getSceneWidth();
        int value = minValue + (minValue - maxValue) * xScrollTo / contentWidth;
        scrollBar->setValue( value );
    }
}
