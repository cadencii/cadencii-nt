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

    void CurveControlChangeView::setSequence( Sequence *sequence ){
        ui->content->setSequence( sequence );
    }

    void CurveControlChangeView::setMutex( QMutex *mutex ){
        ui->content->setMutex( mutex );
    }

    void CurveControlChangeView::setTimesigList( TimesigList *timesigList ){
        ui->content->setTimesigList( timesigList );
    }

    void *CurveControlChangeView::getWidget(){
        return (void *)this;
    }

    void CurveControlChangeView::setDrawOffset( tick_t drawOffset ){
        int xScrollTo = -controllerAdapter->getXFromTick( drawOffset );
        QScrollBar *scrollBar = ui->scrollArea->horizontalScrollBar();
        int maxValue = scrollBar->maximum() + scrollBar->pageStep();
        int minValue = scrollBar->minimum();
        int contentWidth = ui->content->width();
        int value = minValue + (minValue - maxValue) * xScrollTo / contentWidth;
        scrollBar->setValue( value );
    }

    void CurveControlChangeView::notifyHorizontalScroll(){
        QRect visibleRect = ui->content->getVisibleArea();
        tick_t drawOffset = (tick_t)controllerAdapter->getTickFromX( visibleRect.x() );
        controllerAdapter->drawOffsetChanged( (ControlChangeView *)this, drawOffset );
    }

}
