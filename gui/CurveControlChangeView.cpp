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

    void CurveControlChangeView::setTrack( Track *track ){
        ui->content->setTrack( track );
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
        QWidget *viewport = ui->scrollArea->viewport();
        QRect currentChildRect = viewport->childrenRect();
        int dx = xScrollTo - currentChildRect.x();
        int dy = 0;
        viewport->scroll( dx, dy );
    }

    void CurveControlChangeView::notifyHorizontalScroll(){
        QRect visibleRect = ui->content->getVisibleArea();
        tick_t drawOffset = (tick_t)controllerAdapter->getTickFromX( visibleRect.x() );
        controllerAdapter->drawOffsetChanged( (ControlChangeView *)this, drawOffset );
    }

}
