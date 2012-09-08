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
        QWidget( parent )
    {
        ui = new Ui::EditorWidgetBase;
        ui->setupUi( this );
        ui->scrollArea->setBackgroundRole( QPalette::Dark );
        ui->scrollArea->setEditorWidgetBase( this );
    }

    EditorWidgetBase::~EditorWidgetBase(){
        delete ui;
    }

    void EditorWidgetBase::notifyVerticalScroll(){
        QRect rect = ui->scrollArea->getVisibleArea();
        //TODO:ui->keyboard->notifyVerticalScroll( rect.y() );
    }

    void EditorWidgetBase::notifyHorizontalScroll(){
        QRect visibleRect = ui->scrollArea->getVisibleArea();
        tick_t drawOffset = (tick_t)ui->scrollArea->controllerAdapter->getTickFromX( visibleRect.x() );
        ui->scrollArea->controllerAdapter->drawOffsetChanged( getScrollEventSender(), drawOffset );
    }

    void EditorWidgetBase::repaint(){
        ui->scrollArea->repaint();
        ui->keyboard->repaint();
        QWidget::repaint();
    }

    void EditorWidgetBase::setDrawOffsetInternal( tick_t drawOffset ){
        int xScrollTo = -ui->scrollArea->controllerAdapter->getXFromTick( drawOffset );
        QScrollBar *scrollBar = ui->scrollArea->horizontalScrollBar();
        int maxValue = scrollBar->maximum() + scrollBar->pageStep();
        int minValue = scrollBar->minimum();
        int contentWidth = (int)ui->scrollArea->getSceneWidth();
        int value = (int)(minValue + (minValue - maxValue) * (double)xScrollTo / contentWidth);
        scrollBar->setValue( value );
    }

}
