/**
 * EditorWidgetBase.cpp
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
#include "gui/EditorWidgetBase.h"
#include "gui/EditorWidgetBaseMainContent.h"
#include "ui_EditorWidgetBase.h"

namespace cadencii{
    using namespace std;
    using namespace VSQ_NS;

    EditorWidgetBase::EditorWidgetBase( QWidget *parent ) :
        QWidget( parent ), controllerAdapter( 0 ), sequence( 0 )
    {
        ui = new Ui::EditorWidgetBase;
        ui->setupUi( this );
        ui->scrollArea->setBackgroundRole( QPalette::Dark );
        ui->scrollArea->setEditorWidgetBase( this );
        ui->keyboard->setEditorWidgetBase( this );
    }

    EditorWidgetBase::~EditorWidgetBase(){
        delete ui;
    }

    void EditorWidgetBase::notifyVerticalScroll(){
        QRect rect = ui->scrollArea->getVisibleArea();
        ui->keyboard->update();
    }

    void EditorWidgetBase::notifyHorizontalScroll(){
        QRect visibleRect = ui->scrollArea->getVisibleArea();
        tick_t drawOffset = (tick_t)controllerAdapter->getTickFromX( visibleRect.x() );
        controllerAdapter->drawOffsetChanged( getScrollEventSender(), drawOffset );
    }

    void EditorWidgetBase::repaint(){
        ui->scrollArea->repaint();
        ui->keyboard->repaint();
        QWidget::repaint();
    }

    void EditorWidgetBase::setDrawOffsetInternal( tick_t drawOffset ){
        int xScrollTo = -controllerAdapter->getXFromTick( drawOffset );
        QScrollBar *scrollBar = ui->scrollArea->horizontalScrollBar();
        int maxValue = scrollBar->maximum() + scrollBar->pageStep();
        int minValue = scrollBar->minimum();
        int contentWidth = (int)ui->scrollArea->getSceneWidth();
        int value = (int)(minValue + (minValue - maxValue) * (double)xScrollTo / contentWidth);
        scrollBar->setValue( value );
    }

    void EditorWidgetBase::drawMeasureLine( QPainter *painter, const QRect &rect, int x, const MeasureLine &measureLine ){
        static QColor barColor( 161, 157, 136 );
        static QColor beatColor( 209, 204, 172 );
        if( measureLine.isBorder ){
            painter->setPen( barColor );
        }else{
            painter->setPen( beatColor );
        }
        painter->drawLine( x, rect.top(), x, rect.bottom() );
    }

}
