/**
 * ConcreteBarCountView.cpp
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2.0 as published by the Free Software Foundation.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "qt/gui/ConcreteBarCountView.hpp"
#include "ui_EditorWidgetBase.h"

namespace cadencii{

    ConcreteBarCountView::ConcreteBarCountView( QWidget *parent ) :
        EditorWidgetBase( parent )
    {
        const int height = 20;
        setMinimumHeight( height );
        setMaximumHeight( height );
        ui->scrollArea->setBackgroundBrush( QBrush( Qt::lightGray ) );
        lineColor = QColor::fromRgb( 104, 104, 104 );
    }

    ConcreteBarCountView::~ConcreteBarCountView(){
    }

    void *ConcreteBarCountView::getScrollEventSender(){
        return static_cast<BarCountView *>( this );
    }

    void ConcreteBarCountView::setSequence( VSQ_NS::Sequence *sequence ){
        this->sequence = sequence;
    }

    void *ConcreteBarCountView::getWidget(){
        return this;
    }

    void ConcreteBarCountView::setControllerAdapter( ControllerAdapter *controllerAdapter ){
        this->controllerAdapter = controllerAdapter;
    }

    void ConcreteBarCountView::paintMainContent( QPainter *painter, const QRect &rect ){
        ui->scrollArea->paintMeasureLines( painter, rect );

        // 直下のコンポーネントとの区切り線
        painter->setPen( lineColor );
        painter->drawLine( rect.bottomLeft(), rect.bottomRight() );

        ui->scrollArea->paintSongPosition( painter, rect );
    }

    void ConcreteBarCountView::setDrawOffset( VSQ_NS::tick_t drawOffset ){
        setDrawOffsetInternal( drawOffset );
    }

    void ConcreteBarCountView::drawMeasureLine( QPainter *painter, const QRect &rect, int x, const VSQ_NS::MeasureLine &measureLine ){
        painter->setPen( lineColor );
        if( measureLine.isBorder ){
            painter->drawLine( x, rect.top(), x, rect.bottom() );

            int preMeasure = 1;
            if( sequence ){
                preMeasure = sequence->getPreMeasure();
            }
            int barCount = measureLine.barCount - preMeasure + 1;
            QRectF textRect( x + 5, rect.top(), 100, rect.height() );
            QString text( StringUtil::toString( barCount ).c_str() );
            static QTextOption option( Qt::AlignLeft | Qt::AlignVCenter );
            painter->setPen( Qt::black );
            painter->drawText( textRect, text, option );
        }else{
            int lineHeight = measureLine.isAssistLine ? 3 : 6;
            painter->drawLine( x, rect.bottom() - lineHeight, x, rect.bottom() );
        }
    }

    void ConcreteBarCountView::paintSubContent( QPainter *painter, const QRect &rect ){
        painter->fillRect( rect, Qt::lightGray );
        painter->setPen( lineColor );
        painter->drawLine( rect.bottomLeft(), rect.bottomRight() );
        painter->drawLine( rect.topRight(), rect.bottomRight() );
    }

    void ConcreteBarCountView::updateWidget(){
        repaint();
    }

}
