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
#include <algorithm>

namespace cadencii{

    CurveControlChangeView::CurveControlChangeView( QWidget *parent ) :
        EditorWidgetBase( parent )
    {
        trackIndex = 0;
        sequence = &defaultSequence;
        controlChangeName = "dyn";
        front = sequence->track[0].getCurve( controlChangeName );
        ui->scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        ui->scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    }

    void CurveControlChangeView::setTimesigList( VSQ_NS::TimesigList *timesigList ){
        ui->scrollArea->setTimesigList( timesigList );
    }

    void CurveControlChangeView::setSequence( VSQ_NS::Sequence *sequence ){
        this->sequence = sequence;
        setControlChangeName( controlChangeName );
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
        return static_cast<ControlChangeView *>( this );
    }

    QSizeF CurveControlChangeView::getPreferedSceneSize(){
        VSQ_NS::tick_t totalClocks = sequence->getTotalClocks();
        int sceneWidth = controllerAdapter->getXFromTick( totalClocks );
        int sceneHeight = height();
        return QSizeF( sceneWidth, sceneHeight );
    }

    void CurveControlChangeView::paintMainContent( QPainter *painter, const QRect &rect ){
        ui->scrollArea->paintMeasureLines( painter, rect );
        if( front ){
            paintBPList( painter, front, rect );
        }
        //TODO: front が NULL のとき、デフォルト値の描画がされない問題をなんとかする。カーブ名をfieldで持つなどなどを検討
        ui->scrollArea->paintSongPosition( painter, rect );
    }

    void CurveControlChangeView::setTrackIndex( int index ){
        trackIndex = index;
        if( front ){
            std::string name = front->getName();
            setControlChangeName( name );
        }
    }

    void CurveControlChangeView::setControlChangeName( const std::string &name ){
        controlChangeName = name;
        if( sequence ){
            if( 0 <= trackIndex && trackIndex < sequence->track.size() ){
                front = sequence->track[trackIndex].getCurve( controlChangeName );
            }else{
                front = 0;
            }
        }else{
            front = 0;
        }
    }

    //TODO:効率よく描画するようリファクタする
    void CurveControlChangeView::paintBPList( QPainter *painter, vsq::BPList *list, const QRect &rect ){
        int max = list->getMaximum();
        int min = list->getMinimum();
        int height = this->height();
        int y = (int)(height - MARGIN_BOTTOM - ((list->getDefault() - min) / (double)(max - min) * (height - MARGIN_BOTTOM - MARGIN_TOP) ));

        QPainterPath path;
        path.moveTo( -1, height - MARGIN_BOTTOM );
        path.lineTo( -1, y );

        int size = list->size();
        for( int i = 0; i < size; i++ ){
            VSQ_NS::BP point = list->get( i );
            VSQ_NS::tick_t clock = list->getKeyClock( i );
            int x = controllerAdapter->getXFromTick( clock );
            path.lineTo( x, y );
            y = (int)(height - MARGIN_BOTTOM - ((point.value - min) / (double)(max - min) * (height - MARGIN_BOTTOM - MARGIN_TOP) ));
            path.lineTo( x, y );
        }

        // スクリーンのサイズが、コンポーネントのサイズよりも小さい場合を考慮し、
        // 大きい方を左端の座標とする。
        int width = std::max( ui->scrollArea->getSceneWidth(), ui->scrollArea->width() );
        path.lineTo( width, y );
        path.lineTo( width, height - MARGIN_BOTTOM );
        painter->fillPath( path, QColor( 100, 149, 237, 150 ) );

        painter->setPen( QColor( 255, 255, 255 ) );
        painter->drawPath( path );
    }

}
