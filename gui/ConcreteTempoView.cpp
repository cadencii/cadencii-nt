/**
 * ConcreteBarTempoView.cpp
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
#include "gui/ConcreteTempoView.hpp"
#include "ui_EditorWidgetBase.h"
#include <QWidget>

namespace cadencii{

    ConcreteTempoView::ConcreteTempoView( QWidget *parent ) :
        EditorWidgetBase( parent )
    {
        backgroundColor = QColor( Qt::lightGray );
        lineColor = QColor::fromRgb( 104, 104, 104 );
        const int height = 19;
        setMinimumHeight( height );
        setMaximumHeight( height );
        ui->scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        ui->scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        ui->scrollArea->setBackgroundBrush( QBrush( backgroundColor ) );
    }

    ConcreteTempoView::~ConcreteTempoView(){
    }

    void *ConcreteTempoView::getScrollEventSender(){
        return static_cast<TempoView *>( this );
    }

    QSizeF ConcreteTempoView::getPreferedSceneSize(){
        VSQ_NS::tick_t totalClocks = sequence->getTotalClocks();
        int sceneWidth = controllerAdapter->getXFromTick( totalClocks );
        int sceneHeight = height();
        return QSizeF( sceneWidth, sceneHeight );
    }

    void *ConcreteTempoView::getWidget(){
        return (void *)this;
    }

    void ConcreteTempoView::setControllerAdapter( ControllerAdapter *controllerAdapter ){
        this->controllerAdapter = controllerAdapter;
    }

    void ConcreteTempoView::setSequence( VSQ_NS::Sequence *sequence ){
        this->sequence = sequence;
    }

    void ConcreteTempoView::setDrawOffset( VSQ_NS::tick_t drawOffset ){
        setDrawOffsetInternal( drawOffset );
    }

    void ConcreteTempoView::paintMainContent( QPainter *painter, const QRect &rect ){
        // 小節ごとの線
        ui->scrollArea->paintMeasureLines( painter, rect );

        // 直上のコンポーネントとの区切り線
        painter->setPen( lineColor );
        painter->drawLine( rect.topLeft(), rect.topRight() );

        // テンポ変更イベント
        if( sequence ){
            // テンポ値の描かれる幅は drawWindow ピクセル以下と見なす
            const int drawWindow = 200;
            painter->setPen( Qt::black );
            VSQ_NS::TempoList *tempoList = &sequence->tempoList;
            VSQ_NS::TempoList::Iterator i = tempoList->iterator();
            while( i.hasNext() ){
                VSQ_NS::Tempo tempo = i.next();
                int x = controllerAdapter->getXFromTick( tempo.clock ) + 5;
                if( x + drawWindow < rect.left() ){
                    continue;
                }
                if( rect.right() < x ){
                    break;
                }
                float value = 60e6 / tempo.tempo;
                QString text( StringUtil::toString( value, "%.2f" ).c_str() );
                QRectF textRect( x, rect.top(), drawWindow, rect.height() );
                static QTextOption option( Qt::AlignLeft | Qt::AlignVCenter );
                painter->drawText( textRect, text, option );
            }
        }

        // ソングポジション
        ui->scrollArea->paintSongPosition( painter, rect );
    }

    void ConcreteTempoView::paintSubContent( QPainter *painter, const QRect &rect ){
        painter->fillRect( rect, backgroundColor );
        painter->setPen( lineColor );
        painter->drawLine( rect.topLeft(), rect.topRight() );
        painter->drawLine( rect.topRight(), rect.bottomRight() );

        painter->setPen( Qt::black );
        static QString text( "TEMPO" );
        static QTextOption option( Qt::AlignLeft | Qt::AlignVCenter );
        QRectF textRect( rect.left() + 10, rect.top(), rect.width() - 10, rect.height() );
        painter->drawText( textRect, text, option );
    }

    void ConcreteTempoView::drawMeasureLine( QPainter *painter, const QRect &rect, int x, const VSQ_NS::MeasureLine &measureLine ){
        painter->setPen( lineColor );
        if( measureLine.isBorder ){
            painter->drawLine( x, rect.top(), x, rect.bottom() );
        }else{
            painter->drawLine( x, rect.bottom() - 5, x, rect.bottom() );
        }
    }

}
