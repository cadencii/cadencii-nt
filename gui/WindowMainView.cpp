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
        if( ui->pianoRoll ){
            ui->verticalLayout1->removeWidget( ui->pianoRoll );
            delete ui->pianoRoll;
        }

        ui->pianoRoll = widget;
        setupPianorollWidget();
    }

    void WindowMainView::setControlChangeView( ControlChangeView *controlChangeView ){
        QWidget *widget = (QWidget *)controlChangeView->getWidget();
        widget->setParent( ui->layoutWidget3 );
        if( ui->curveView ){
            ui->verticalLayout3->removeWidget( ui->curveView );
            delete ui->curveView;
        }
        ui->curveView = widget;
        ui->verticalLayout3->addWidget( widget );
    }

    void WindowMainView::setBarCountView( BarCountView *barCountView ){
        QWidget *widget = (QWidget *)barCountView->getWidget();
        widget->setParent( ui->layoutWidget1 );
        if( ui->barCountView ){
            ui->verticalLayout1->removeWidget( ui->barCountView );
            delete ui->barCountView;
        }

        ui->barCountView = widget;
        setupPianorollWidget();
    }

    void WindowMainView::setTempoView( TempoView *tempoView ){
        QWidget *widget = (QWidget *)tempoView->getWidget();
        widget->setParent( ui->layoutWidget1 );
        if( ui->tempoView ){
            ui->verticalLayout1->removeWidget( ui->tempoView );
            delete ui->tempoView;
        }

        ui->tempoView = widget;
        setupPianorollWidget();
    }

    void WindowMainView::setTimesigView( TimesigView *timesigView ){
        QWidget *widget = (QWidget *)timesigView->getWidget();
        widget->setParent( ui->layoutWidget1 );
        if( ui->timesigView ){
            ui->verticalLayout1->removeWidget( ui->timesigView );
            delete ui->timesigView;
        }

        ui->timesigView = widget;
        setupPianorollWidget();
    }

    void WindowMainView::setupPianorollWidget(){
        QWidgetList list;
        list.push_back( ui->barCountView );
        list.push_back( ui->tempoView );
        list.push_back( ui->timesigView );
        list.push_back( ui->pianoRoll );

        // verticalLayout1 にぶら下がっているコンポーネントをすべて取り除く
        QObjectList children = ui->verticalLayout1->children();
        for( QObjectList::iterator i = children.begin(); i != children.end(); ++i ){
            QObject *object = (*i);
            if( object->isWidgetType() ){
                QWidget *widget = static_cast<QWidget *>( object );
                ui->verticalLayout1->removeWidget( widget );
            }
        }

        // 必要なコンポーネントを、順番に追加する
        for( QWidgetList::iterator i = list.begin(); i != list.end(); ++i ){
            QWidget *widget = (*i);
            widget->setParent( ui->layoutWidget1 );
            ui->verticalLayout1->addWidget( widget );
        }
    }

    void WindowMainView::setControllerAdapter( ControllerAdapter *controllerAdapter ){
        this->controllerAdapter = controllerAdapter;
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
