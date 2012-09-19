/**
 * ConcreteBarCountView.cpp
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
#include "gui/ConcreteBarCountView.hpp"
#include "ui_EditorWidgetBase.h"

namespace cadencii{

    ConcreteBarCountView::ConcreteBarCountView(){
        const int height = 19;
        setMinimumHeight( height );
        setMaximumHeight( height );
        ui->scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        ui->scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        ui->scrollArea->setBackgroundBrush( QBrush( Qt::lightGray ) );
        lineColor = QColor::fromRgb( 104, 104, 104 );
        sequence = 0;
        controllerAdapter = 0;
    }

    void *ConcreteBarCountView::getScrollEventSender(){
        return static_cast<BarCountView *>( this );
    }

    QSizeF ConcreteBarCountView::getPreferedSceneSize(){
        VSQ_NS::tick_t totalClocks = sequence->getTotalClocks();
        int sceneWidth = controllerAdapter->getXFromTick( totalClocks );
        int sceneHeight = height();
        return QSizeF( sceneWidth, sceneHeight );
    }

    void ConcreteBarCountView::setSequence( VSQ_NS::Sequence *sequence ){
        this->sequence = sequence;
    }

    void *ConcreteBarCountView::getWidget(){
        return this;
    }

    void ConcreteBarCountView::setControllerAdapter( ControllerAdapter *controllerAdapter ){
        this->controllerAdapter = controllerAdapter;
        ui->scrollArea->setControllerAdapter( controllerAdapter );
    }

    void ConcreteBarCountView::paintMainContent( QPainter *painter, const QRect &rect ){
        ui->scrollArea->paintMeasureLines( painter, rect );
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
            painter->drawLine( x, rect.bottom() - 5, x, rect.bottom() );
        }
    }

    void ConcreteBarCountView::paintSubContent( QPainter *painter, const QRect &rect ){
        painter->fillRect( rect, Qt::lightGray );
        painter->setPen( lineColor );
        painter->drawLine( rect.topRight(), rect.bottomRight() );
    }

}
