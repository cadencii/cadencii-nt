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
#include "Controller.hpp"
#include <QFileDialog>

namespace cadencii{

    WindowMainView::WindowMainView( QWidget *parent )
        : QMainWindow( parent ),
        ui( new Ui::WindowMainView )
    {
        ui->setupUi( this );
        controllerAdapter = 0;
    }

    WindowMainView::~WindowMainView(){
        delete ui;
    }

    void WindowMainView::setTrackView( TrackView *trackView ){
        QWidget *widget = (QWidget *)trackView->getWidget();
        widget->setParent( ui->layoutWidget1 );
        ui->verticalLayout1->removeWidget( ui->pianoRoll );
        ui->pianoRoll = widget;
        ui->verticalLayout1->addWidget( widget );
    }

    void WindowMainView::setControlChangeView( ControlChangeView *controlChangeView ){
        QWidget *widget = (QWidget *)controlChangeView->getWidget();
        widget->setParent( ui->layoutWidget3 );
        ui->verticalLayout3->removeWidget( ui->curveView );
        ui->curveView = widget;
        ui->verticalLayout3->addWidget( widget );
    }

}

void cadencii::WindowMainView::on_action_open_vsq_vocaloid_midi_triggered(){
    QString filePath = QFileDialog::getOpenFileName( this );
    QFile file( filePath );
    if( file.exists() && this->controllerAdapter ){
        this->controllerAdapter->openVSQFile( std::string( filePath.toLocal8Bit().data() ) );
    }
    this->activateWindow();
}
