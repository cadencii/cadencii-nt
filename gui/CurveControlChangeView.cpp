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
        ui->scrollArea->setBackgroundBrush( QBrush( Qt::darkGray ) );
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
        // グラフ部分の最大値、最小値の位置の線を描く
        static QColor lineGraphBottom( 156, 161, 169 );
        static QColor lineGraphTop( 46, 47, 50 );
        painter->setPen( lineGraphBottom );
        painter->drawLine( rect.left(), rect.bottom() - MARGIN_BOTTOM,
                           rect.right(), rect.bottom() - MARGIN_BOTTOM );
        painter->setPen( lineGraphTop );
        painter->drawLine( rect.left(),  MARGIN_TOP,
                           rect.right(), MARGIN_TOP );

        ui->scrollArea->paintMeasureLines( painter, rect );
        if( front ){
            paintBPList( painter, front, rect );
        }
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

        // カーソルが描画範囲に入っていれば、カーソル位置での値を描く。
        QPoint globalCursorPos = QCursor::pos();
        QPoint globalTopLeftCornerPos = ui->scrollArea->mapToGlobal( QPoint( 0, 0 ) );
        QPoint viewportCursorPos = QPoint( globalCursorPos.x() - globalTopLeftCornerPos.x(),
                                                  globalCursorPos.y() - globalTopLeftCornerPos.y() );
        QPoint sceneCursorPos = ui->scrollArea->mapToScene( viewportCursorPos ).toPoint();
        if( MARGIN_TOP <= sceneCursorPos.y() && sceneCursorPos.y() <= height - MARGIN_BOTTOM && rect.contains( sceneCursorPos ) ){
            static QTextOption textOption( Qt::AlignRight | Qt::AlignBottom );
            int value = (height - MARGIN_BOTTOM - sceneCursorPos.y()) * (max - min) / (height - MARGIN_BOTTOM - MARGIN_TOP) + min;
            painter->drawText( QRectF( sceneCursorPos.x() - 100, sceneCursorPos.y() - 100, 100, 100 ),
                               QString( StringUtil::toString( value ).c_str() ),
                               textOption );
        }
    }

    void CurveControlChangeView::drawMeasureLine( QPainter *painter, const QRect &rect, int x, bool isBorder ){
        static QColor white100( 0, 0, 0, 100 );
        static QColor pen( 12, 12, 12 );
        if( isBorder ){
            painter->setPen( white100 );
            painter->drawLine( x, rect.height() - MARGIN_BOTTOM - 1, x, MARGIN_TOP + 1 );
        }else{
            int graphHeight = rect.height() - MARGIN_BOTTOM - MARGIN_TOP;
            int center = rect.top() + graphHeight / 2;
            painter->setPen( white100 );
            painter->drawLine( x, center - 5, x, center + 6 );
            painter->setPen( pen );
            painter->drawLine( x, MARGIN_TOP, x, MARGIN_TOP + 6 );
            painter->drawLine( x, rect.height() - MARGIN_BOTTOM, x, rect.height() - MARGIN_BOTTOM - 6 );
        }
    }

}
