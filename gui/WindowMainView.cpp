/**
 * WindowMainView.cpp
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
#include "gui/WindowMainView.h"
#include "ui_WindowMainView.h"

namespace cadencii{

    WindowMainView::WindowMainView( QWidget *parent )
        : QMainWindow( parent ),
        ui( new ::Ui::WindowMainView )
    {
        ui->setupUi( this );
    }

    WindowMainView::~WindowMainView(){
        delete ui;
    }

    void WindowMainView::setTrackView( TrackView *trackView ){
        QWidget *actualWidget = trackView->getActualWidget();
        actualWidget->setParent( ui->layoutWidget );
        ui->verticalLayout->removeWidget( ui->pianoRoll );
        ui->pianoRoll = actualWidget;
        ui->verticalLayout->addWidget( actualWidget );
    }

}
