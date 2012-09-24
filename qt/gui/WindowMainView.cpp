/**
 * WindowMainView.cpp
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
#include "qt/gui/WindowMainView.hpp"
#include "ui_WindowMainView.h"
#include "Controller.hpp"
#include "enum/QuantizeMode.hpp"
#include "Settings.hpp"
#include <QFileDialog>

namespace cadencii{

    WindowMainView::WindowMainView( QWidget *parent )
        : QMainWindow( parent ),
        ui( new Ui::WindowMainView )
    {
        ui->setupUi( this );
        controllerAdapter = 0;
        reflectSettings();
    }

    WindowMainView::~WindowMainView(){
        delete ui;
    }

    void WindowMainView::setTrackView( TrackView *trackView ){
        PianorollTrackView *widget = static_cast<PianorollTrackView *>( trackView->getWidget() );
        widget->setParent( ui->layoutWidget1 );
        if( ui->pianoRoll ){
            ui->verticalLayout1->removeWidget( ui->pianoRoll );
            delete ui->pianoRoll;
        }

        ui->pianoRoll = widget;
        setupPianorollWidget();
    }

    void WindowMainView::setControlChangeView( ControlChangeView *controlChangeView ){
        CurveControlChangeView *widget = static_cast<CurveControlChangeView *>( controlChangeView->getWidget() );
        widget->setParent( ui->layoutWidget3 );
        if( ui->curveView ){
            ui->verticalLayout3->removeWidget( ui->curveView );
            delete ui->curveView;
        }
        ui->curveView = widget;
        ui->verticalLayout3->addWidget( widget );
    }

    void WindowMainView::setBarCountView( BarCountView *barCountView ){
        ConcreteBarCountView *widget = static_cast<ConcreteBarCountView *>( barCountView->getWidget() );
        widget->setParent( ui->layoutWidget1 );
        if( ui->barCountView ){
            ui->verticalLayout1->removeWidget( ui->barCountView );
            delete ui->barCountView;
        }

        ui->barCountView = widget;
        setupPianorollWidget();
    }

    void WindowMainView::setTempoView( TempoView *tempoView ){
        ConcreteTempoView *widget = static_cast<ConcreteTempoView *>( tempoView->getWidget() );
        widget->setParent( ui->layoutWidget1 );
        if( ui->tempoView ){
            ui->verticalLayout1->removeWidget( ui->tempoView );
            delete ui->tempoView;
        }

        ui->tempoView = widget;
        setupPianorollWidget();
    }

    void WindowMainView::setTimesigView( TimesigView *timesigView ){
        ConcreteTimesigView *widget = static_cast<ConcreteTimesigView *>( timesigView->getWidget() );
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

    void WindowMainView::reflectSettings(){
        reflectQuantizeModeSettings();
        reflectGridVisibleSettings();
        reflectAutoScrollSettings();
    }

    void WindowMainView::reflectQuantizeModeSettings(){
        // quantizeMode
        QuantizeMode::QuantizeModeEnum quantizeMode = Settings::instance()->getQuantizeMode();
        ui->actionQuantizeQuarter->setChecked( quantizeMode == QuantizeMode::QUARTER );
        ui->actionQuantizeEighth->setChecked( quantizeMode == QuantizeMode::EIGHTH );
        ui->actionQuantizeSisteenth->setChecked( quantizeMode == QuantizeMode::SIXTEENTH );
        ui->actionQuantizeThirtySecond->setChecked( quantizeMode == QuantizeMode::THIRTY_SECOND );
        ui->actionQuantizeSixtyFourth->setChecked( quantizeMode == QuantizeMode::SIXTY_FOURTH );
        ui->actionQuantizeHundredTwentyEighth->setChecked( quantizeMode == QuantizeMode::HUNDRED_TWENTY_EIGHTH );
        ui->actionQuantizeNone->setChecked( quantizeMode == QuantizeMode::NONE );
        updateWidget();
    }

    void WindowMainView::updateWidget(){
        ui->barCountView->repaint();
        ui->tempoView->repaint();
        ui->timesigView->repaint();
        ui->pianoRoll->repaint();
        ui->curveView->repaint();
        repaint();
    }

    void WindowMainView::reflectGridVisibleSettings(){
        bool gridVisible = Settings::instance()->isGridVisible();
        ui->actionToggleGridVisible->setChecked( gridVisible );
    }

    void WindowMainView::reflectAutoScrollSettings(){
        bool autoScroll = Settings::instance()->isAutoScroll();
        ui->actionToggleAutoScroll->setChecked( autoScroll );
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

void cadencii::WindowMainView::on_actionQuantizeQuarter_toggled( bool arg1 ){
    if( arg1 ){
        Settings::instance()->setQuantizeMode( QuantizeMode::QUARTER );
        reflectQuantizeModeSettings();
    }
}

void cadencii::WindowMainView::on_actionQuantizeEighth_toggled( bool arg1 ){
    if( arg1 ){
        Settings::instance()->setQuantizeMode( QuantizeMode::EIGHTH );
        reflectQuantizeModeSettings();
    }
}

void cadencii::WindowMainView::on_actionQuantizeSisteenth_toggled( bool arg1 ){
    if( arg1 ){
        Settings::instance()->setQuantizeMode( QuantizeMode::SIXTEENTH );
        reflectQuantizeModeSettings();
    }
}

void cadencii::WindowMainView::on_actionQuantizeThirtySecond_toggled( bool arg1 ){
    if( arg1 ){
        Settings::instance()->setQuantizeMode( QuantizeMode::THIRTY_SECOND );
        reflectQuantizeModeSettings();
    }
}

void cadencii::WindowMainView::on_actionQuantizeSixtyFourth_toggled( bool arg1 ){
    if( arg1 ){
        Settings::instance()->setQuantizeMode( QuantizeMode::SIXTY_FOURTH );
        reflectQuantizeModeSettings();
    }
}

void cadencii::WindowMainView::on_actionQuantizeHundredTwentyEighth_toggled( bool arg1 ){
    if( arg1 ){
        Settings::instance()->setQuantizeMode( QuantizeMode::HUNDRED_TWENTY_EIGHTH );
        reflectQuantizeModeSettings();
    }
}

void cadencii::WindowMainView::on_actionQuantizeNone_toggled( bool arg1 ){
    if( arg1 ){
        Settings::instance()->setQuantizeMode( QuantizeMode::NONE );
        reflectQuantizeModeSettings();
    }
}

void cadencii::WindowMainView::on_actionMoveSongPositionLeft_triggered(){
    controllerAdapter->moveSongPositionStepped( true );
}

void cadencii::WindowMainView::on_actionMoveSongPositionRight_triggered(){
    controllerAdapter->moveSongPositionStepped( false );
}

void cadencii::WindowMainView::on_actionToggleGridVisible_toggled( bool arg1 ){
    Settings::instance()->setGridVisible( arg1 );
    updateWidget();
}

void cadencii::WindowMainView::on_actionToggleAutoScroll_toggled( bool arg1 ){
    Settings::instance()->setAutoScroll( arg1 );
}

void cadencii::WindowMainView::on_actionExportAsMusicXML_triggered(){
    QString filePath = QFileDialog::getSaveFileName( this );
    controllerAdapter->exportAsMusicXml( filePath.toLocal8Bit().data() );
    activateWindow();
}
