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
#include "CurveControlChangeView.h"
#include "ui_EditorWidgetBase.h"
#include <QScrollBar>

namespace cadencii{

    CurveControlChangeView::CurveControlChangeView( QWidget *parent ) :
        EditorWidgetBase( parent )
    {
        trackIndex = 0;
        ui->scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        ui->scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    }

    void CurveControlChangeView::setTimesigList( VSQ_NS::TimesigList *timesigList ){
        ui->scrollArea->setTimesigList( timesigList );
    }

    void CurveControlChangeView::setSequence( VSQ_NS::Sequence *sequence ){
        this->sequence = sequence;
    }

    void *CurveControlChangeView::getWidget(){
        return (void *)this;
    }

    void CurveControlChangeView::setDrawOffset( VSQ_NS::tick_t drawOffset ){
        setDrawOffsetInternal( drawOffset );
    }

    void CurveControlChangeView::setControllerAdapter( ControllerAdapter *controllerAdapter ){
        ControlChangeView::setControllerAdapter( controllerAdapter );
        ui->scrollArea->setControllerAdapter( controllerAdapter );
    }

    void *CurveControlChangeView::getScrollEventSender(){
        return (ControlChangeView *)this;
    }

    QSizeF CurveControlChangeView::getPreferedSceneSize(){
        VSQ_NS::tick_t totalClocks = sequence->getTotalClocks();
        int sceneWidth = controllerAdapter->getXFromTick( totalClocks );
        int sceneHeight = height();
        return QSizeF( sceneWidth, sceneHeight );
    }

    void CurveControlChangeView::paintMainContent( QPainter *painter, const QRect &rect ){
        ui->scrollArea->paintMeasureLines( painter, rect );
        ui->scrollArea->paintSongPosition( painter, rect );
    }

    void CurveControlChangeView::setTrackIndex( int index ){
        trackIndex = index;
    }

}
