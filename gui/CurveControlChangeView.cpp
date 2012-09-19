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
#include "vsq/Track.hpp"
#include <QScrollBar>
#include <QColor>
#include <QMouseEvent>
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

        trackTabHilightBackgroundColor = new QColor[16];
        trackTabHilightBackgroundColor[0] = QColor( 181, 220, 16 ); trackTabHilightBackgroundColor[1] = QColor( 231, 244, 49 ); trackTabHilightBackgroundColor[2] = QColor( 252, 230, 29 );
        trackTabHilightBackgroundColor[3] = QColor( 247, 171, 20 ); trackTabHilightBackgroundColor[4] = QColor( 249, 94, 17 ); trackTabHilightBackgroundColor[5] = QColor( 234, 27, 47 );
        trackTabHilightBackgroundColor[6] = QColor( 175, 20, 80 ); trackTabHilightBackgroundColor[7] = QColor( 183, 24, 149 ); trackTabHilightBackgroundColor[8] = QColor( 105, 22, 158 );
        trackTabHilightBackgroundColor[9] = QColor( 22, 36, 163 ); trackTabHilightBackgroundColor[10] = QColor( 37, 121, 204 ); trackTabHilightBackgroundColor[11] = QColor( 29, 179, 219 );
        trackTabHilightBackgroundColor[12] = QColor( 24, 239, 239 ); trackTabHilightBackgroundColor[13] = QColor( 25, 206, 175 ); trackTabHilightBackgroundColor[14] = QColor( 23, 160, 134 );
        trackTabHilightBackgroundColor[15] = QColor( 79, 181, 21 );
        trackTabRenderButtonBackgroundColor = new QColor[16];
        trackTabRenderButtonBackgroundColor[0] = QColor( 19, 143, 52 ); trackTabRenderButtonBackgroundColor[1] = QColor( 158, 154, 18 ); trackTabRenderButtonBackgroundColor[2] = QColor( 160, 143, 23 );
        trackTabRenderButtonBackgroundColor[3] = QColor( 145, 98, 15 ); trackTabRenderButtonBackgroundColor[4] = QColor( 142, 52, 12 ); trackTabRenderButtonBackgroundColor[5] = QColor( 142, 19, 37 );
        trackTabRenderButtonBackgroundColor[6] = QColor( 96, 13, 47 ); trackTabRenderButtonBackgroundColor[7] = QColor( 117, 17, 98 ); trackTabRenderButtonBackgroundColor[8] = QColor( 62, 15, 99 );
        trackTabRenderButtonBackgroundColor[9] = QColor( 13, 23, 84 ); trackTabRenderButtonBackgroundColor[10] = QColor( 25, 84, 132 ); trackTabRenderButtonBackgroundColor[11] = QColor( 20, 119, 142 );
        trackTabRenderButtonBackgroundColor[12] = QColor( 19, 142, 139 ); trackTabRenderButtonBackgroundColor[13] = QColor( 17, 122, 102 ); trackTabRenderButtonBackgroundColor[14] = QColor( 13, 86, 72 );
        trackTabRenderButtonBackgroundColor[15] = QColor( 43, 91, 12 );
        borderColor = QColor::fromRgb( 118, 123, 138 );
        singerEventBorderColor = QColor::fromRgb( 182, 182, 182 );
    }

    CurveControlChangeView::~CurveControlChangeView(){
        delete [] trackTabHilightBackgroundColor;
        delete [] trackTabRenderButtonBackgroundColor;
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

        // グラフ部分の本体を描く
        ui->scrollArea->paintMeasureLines( painter, rect );
        if( front ){
            paintBPList( painter, front, rect );
        }
        ui->scrollArea->paintSongPosition( painter, rect );

        // トラック一覧の部分を描く
        paintTrackList( painter );

        // 歌手変更イベントを描く
        paintSingerList( painter );
    }

    void CurveControlChangeView::setTrackIndex( int index ){
        trackIndex = index;
        if( front ){
            std::string name = front->getName();
            setControlChangeName( name );
        }
        update();
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
        int y = getYFromValue( max, min, list->getDefault() );

        QPainterPath path;
        path.moveTo( -1, height - MARGIN_BOTTOM );
        path.lineTo( -1, y );

        int size = list->size();
        for( int i = 0; i < size; i++ ){
            VSQ_NS::BP point = list->get( i );
            VSQ_NS::tick_t clock = list->getKeyClock( i );
            int x = controllerAdapter->getXFromTick( clock );
            path.lineTo( x, y );
            y = getYFromValue( max, min, point.value );
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
            int value = getValueFromY( max, min, sceneCursorPos.y() );
            painter->drawText( QRectF( sceneCursorPos.x() - 100, sceneCursorPos.y() - 100, 100, 100 ),
                               QString( StringUtil::toString( value ).c_str() ),
                               textOption );
        }
    }

    void CurveControlChangeView::drawMeasureLine( QPainter *painter, const QRect &rect, int x, const VSQ_NS::MeasureLine &measureLine ){
        static QColor white100( 0, 0, 0, 100 );
        static QColor pen( 12, 12, 12 );
        if( measureLine.isBorder ){
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

    int CurveControlChangeView::getYFromValue( int max, int min, int value ){
        int height = this->height();
        return (int)(height - MARGIN_BOTTOM - ((value - min) / (double)(max - min) * (height - MARGIN_BOTTOM - MARGIN_TOP) ));
    }

    int CurveControlChangeView::getValueFromY( int max, int min, int y ){
        int height = this->height();
        return (height - MARGIN_BOTTOM - y) * (max - min) / (height - MARGIN_BOTTOM - MARGIN_TOP) + min;
    }

    void CurveControlChangeView::paintTrackList( QPainter *painter ){
        painter->setPen( borderColor );
        int height = ui->scrollArea->getSceneHeight();
        int width = ui->scrollArea->getSceneWidth();
        if( width < this->width() ){
            width = this->width();
        }
        QRect visibleArea = ui->scrollArea->getVisibleArea();
        painter->fillRect( visibleArea.left(), height - LANE_HEIGHT,
                     width, LANE_HEIGHT, Qt::gray );
        painter->drawLine( 0, height - LANE_HEIGHT,
                    width, height - LANE_HEIGHT );
        if( sequence ){
            int selector_width = getTrackTabWidth();
            for( int i = 0; i < 16; i++ ){
                int x = i * selector_width + visibleArea.left();
                std::string name = (i < sequence->track.size()) ? StringUtil::toString( i + 1 ) + " " + sequence->track[i].getName() : "";
                paintTrackTab( painter,
                              QRect( x, height - LANE_HEIGHT + 1, selector_width, LANE_HEIGHT - 1 ),
                              QString( name.c_str() ),
                              (i == trackIndex) ? true : false,
                              true/*TODO:vsq_track.getCommon().PlayMode >= 0*/,
                              false/*TODO:AppManager.getRenderRequired( i + 1 )*/,
                              trackTabHilightBackgroundColor[i],
                              trackTabRenderButtonBackgroundColor[i] );
            }
        }
    }

    void CurveControlChangeView::paintTrackTab( QPainter *painter, const QRect &destRect, const QString &name, bool selected, bool enabled, bool render_required, const QColor &hilight, const QColor &render_button_hilight ){
        QColor panel_color = enabled ? hilight : QColor( 125, 123, 124 );
        QColor panel_title = QColor::fromRgb( 0, 0, 0 );
        QColor button_title = selected ? QColor::fromRgb( 255, 255, 255 ) : QColor::fromRgb( 0, 0, 0 );
        QColor border = selected ? QColor::fromRgb( 255, 255, 255 ) : QColor( 118, 123, 138 );

        // 背景(選択されている場合)
        if( selected ){
            painter->fillRect( destRect, panel_color );
            if( render_required && enabled ){
                painter->fillRect( destRect.right() - 10, destRect.top(),
                             10, destRect.height(), render_button_hilight );
            }
        }

        // 左縦線
        painter->setPen( border );
        painter->drawLine( destRect.left(), destRect.top(),
                     destRect.left(), destRect.bottom() );
        if( 0 < name.length() ){
            // 上横線
            painter->setPen( border );
            painter->drawLine( destRect.left() + 1, destRect.top(),
                         destRect.right(), destRect.top() );
        }
        if( render_required ){
            painter->setPen( border );
            painter->drawLine( destRect.right() - 10, destRect.top(),
                         destRect.right() - 10, destRect.bottom() );
        }

        painter->setClipRect( destRect );
        painter->setPen( panel_title );
        QRectF textRect( destRect.left() + 2, destRect.top(), destRect.width() - 2, destRect.height() );
        QTextOption textOption( Qt::AlignLeft | Qt::AlignVCenter );
        painter->drawText( textRect, name, textOption );
        if( render_required ){
            painter->setPen( button_title );
            QRect renderMarkRect( destRect.right() - RENDER_BUTTON_WIDTH, destRect.top(),
                                  RENDER_BUTTON_WIDTH, destRect.height() );
            painter->drawText( renderMarkRect, QString( "R" ), textOption );
        }
        if( selected ){
            painter->setPen( border );
            painter->drawLine( destRect.right(), destRect.top(),
                         destRect.right(), destRect.bottom() );
            painter->setPen( border );
            painter->drawLine( destRect.left(), destRect.bottom(),
                         destRect.right(), destRect.bottom() );
        }
        painter->setClipRect( QRect(), Qt::NoClip );
    }

    /**
     * @brief トラック選択部分の、トラック1個分の幅を調べます。pixel
     */
    int CurveControlChangeView::getTrackTabWidth(){
        int draft = TRACK_TAB_MAX_WIDTH;
        // トラックの一覧を表示するのに利用できる最大の描画幅
        int maxTotalWidth = width();
        int numTrack = 1;
        if( sequence ){
            numTrack = sequence->track.size();
        }
        if( draft * (numTrack - 1) <= maxTotalWidth ){
            return draft;
        }else{
            return (int)((maxTotalWidth) / (numTrack - 1.0f));
        }
    }

    void CurveControlChangeView::mousePressEvent( QMouseEvent *event ){
        if( acceptMousePressOnTrackList( event ) ){
            event->accept();
            return;
        }
    }

    bool CurveControlChangeView::acceptMousePressOnTrackList( QMouseEvent *event ){
        QRect geometry = ui->scrollArea->geometry();
        QPoint mousePos = event->pos();
        if( geometry.contains( mousePos ) &&
            geometry.height() - LANE_HEIGHT <= mousePos.y() &&
            mousePos.y() <= geometry.height() )
        {
            int offset = mousePos.x() - geometry.x();
            int trackTabWidth = getTrackTabWidth();
            int trackIndex = offset / trackTabWidth;
            if( sequence ){
                if( trackIndex != this->trackIndex && 0 <= trackIndex && trackIndex < sequence->track.size() ){
                    controllerAdapter->setTrackIndex( 0, trackIndex );
                    return true;
                }
            }
        }
        return false;
    }

    void CurveControlChangeView::paintSingerList( QPainter *painter ){
        QRect visibleArea = ui->scrollArea->getVisibleArea();
        int height = ui->scrollArea->getSceneHeight();
        int width = ui->scrollArea->getSceneWidth();
        if( width < this->width() ){
            width = this->width();
        }

        // 背景と境界線
        painter->fillRect( visibleArea.left(), height - LANE_HEIGHT * 2,
                           width, LANE_HEIGHT, Qt::gray );
        painter->setPen( borderColor );
        painter->drawLine( visibleArea.left(), height - LANE_HEIGHT * 2,
                           visibleArea.left() + this->width(), height - LANE_HEIGHT * 2 );

        if( !sequence ){
            return;
        }
        if( trackIndex < 0 || sequence->track.size() <= trackIndex ){
            return;
        }

        // コンポーネントの左端位置での、歌手変更イベントを調べる
        int singerItemY = height - LANE_HEIGHT * 2 + 1;
        VSQ_NS::Track *track = &sequence->track[trackIndex];
        int offset = controllerAdapter->getXFromTick( 0 );
        int xAtLeft = visibleArea.x() + offset;
        int clockAtLeft = controllerAdapter->getTickFromX( xAtLeft );
        VSQ_NS::Event singerAtLeft = track->getSingerEventAt( clockAtLeft );
        if( !singerAtLeft.isEOS() ){
            paintSinger( painter, singerAtLeft, xAtLeft, singerItemY, LEFT );
        }

        // 順に描画する
        VSQ_NS::EventListIndexIterator i =
                track->getIndexIterator( VSQ_NS::EventListIndexIteratorKind::SINGER );
        VSQ_NS::Event::List *events = track->getEvents();
        while( i.hasNext() ){
            int index = i.next();
            VSQ_NS::Event item = events->get( index );
            int x = controllerAdapter->getXFromTick( item.clock );
            if( x < xAtLeft ){
                continue;
            }
            if( visibleArea.right() < x ){
                break;
            }
            paintSinger( painter, item, x, singerItemY, DEFAULT );
        }
    }

    void CurveControlChangeView::paintSinger( QPainter *painter, const VSQ_NS::Event &singerEvent, int x, int y, SingerItemState state ){
        QRect rc( x, y,
                  SINGER_ITEM_WIDTH, LANE_HEIGHT - 2 );
        switch( state ){
            case LEFT:{
                painter->fillRect( rc, Qt::lightGray );
                break;
            }
            default:{
                painter->fillRect( rc, Qt::white );
                break;
            }
        }
        painter->setPen( singerEventBorderColor );
        painter->drawRect( rc );

        painter->setPen( Qt::black );
        QString text( singerEvent.singerHandle.ids.c_str() );
        static QTextOption option( Qt::AlignLeft | Qt::AlignVCenter );
        QRect textRect( x + 1, y, SINGER_ITEM_WIDTH - 1, LANE_HEIGHT - 1 );
        painter->drawText( textRect, text, option );
    }

}
