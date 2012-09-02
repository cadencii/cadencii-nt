/**
 * CurveControlChangeView.cpp
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
#include "gui/CurveControlChangeView.h"
#include "gui/CurveControlChangeViewContent.h"
#include "ui_CurveControlChangeView.h"
#include <QDebug> // for debug

namespace cadencii{
    using namespace std;
    using namespace VSQ_NS;

    CurveControlChangeView::CurveControlChangeView( QWidget *parent ) :
        QWidget( parent ),
        ui( new Ui::CurveControlChangeView )
    {
        ui->setupUi( this );
        ui->scrollArea->setBackgroundRole( QPalette::Dark );
        ui->scrollArea->setControlChangeView( this );
        ui->content->setControlChangeView( this );
        ui->keyboard->setControlChangeView( this );
        controllerAdapter = 0;
    }

    CurveControlChangeView::~CurveControlChangeView(){
        delete ui;
    }

    void CurveControlChangeView::repaint(){
        ui->content->repaint();
        ui->keyboard->repaint();
        QWidget::repaint();
    }

    void CurveControlChangeView::setTrack( Track *track ){
        ui->content->setTrack( track );
    }

    void CurveControlChangeView::setMutex( QMutex *mutex ){
        ui->content->setMutex( mutex );
    }

    void CurveControlChangeView::setSongPosition( tick_t songPosition, bool autoScroll ){
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

    void CurveControlChangeView::setTimesigList( TimesigList *timesigList ){
        ui->content->setTimesigList( timesigList );
    }

    void *CurveControlChangeView::getWidget(){
        return (void *)this;
    }

    void CurveControlChangeView::setDrawOffset( tick_t drawOffset ){
        int xScrollTo = -ui->content->getXFromTick( drawOffset );
        QWidget *viewport = ui->scrollArea->viewport();
        QRect currentChildRect = viewport->childrenRect();
        int dx = xScrollTo - currentChildRect.x();
        int dy = 0;
        viewport->scroll( dx, dy );
    }

    void CurveControlChangeView::notifyHorizontalScroll(){
        QRect visibleRect = ui->content->getVisibleArea();
        tick_t drawOffset = (tick_t)ui->content->getTickFromX( visibleRect.x() );
        controllerAdapter->drawOffsetChanged( (ControlChangeView *)this, drawOffset );
    }
}
