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
#include "WindowMainView.hpp"
#include "ConcretePropertyView.hpp"
#include "ui_WindowMainView.h"
#include "../../Controller.hpp"
#include "../../enum/QuantizeMode.hpp"
#include "../../Settings.hpp"
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

    void WindowMainView::setPropertyView( PropertyView *propertyView ){
        ConcretePropertyView *widget = static_cast<ConcretePropertyView *>( propertyView->getWidget() );
        if( ui->propertyView ){
            delete ui->propertyView;
        }

        ui->propertyView = widget;
        widget->setParent( ui->splitter_4 );
        ui->splitter_4->insertWidget( 0, widget );

        // ウィンドウ開いた時に propertyView にフォーカスが当たっていると
        // ださいので。本当は ConcretePropertyView でなんとかしたい。
        ui->pianoRoll->setFocus();
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
        reflectToolKind();
    }

    void WindowMainView::reflectSettings(){
        reflectQuantizeModeSettings();
        reflectGridVisibleSettings();
        reflectAutoScrollSettings();
    }

    void WindowMainView::reflectQuantizeModeSettings(){
        // quantizeMode
        QuantizeMode::QuantizeModeEnum quantizeMode = Settings::instance()->getQuantizeMode();
        ui->action_quantize_quarter->setChecked( quantizeMode == QuantizeMode::QUARTER );
        ui->action_quantize_eighth->setChecked( quantizeMode == QuantizeMode::EIGHTH );
        ui->action_quantize_sixteenth->setChecked( quantizeMode == QuantizeMode::SIXTEENTH );
        ui->action_quantize_thirty_second->setChecked( quantizeMode == QuantizeMode::THIRTY_SECOND );
        ui->action_quantize_sixty_fourth->setChecked( quantizeMode == QuantizeMode::SIXTY_FOURTH );
        ui->action_quantize_hundred_twenty_eighth->setChecked( quantizeMode == QuantizeMode::HUNDRED_TWENTY_EIGHTH );
        ui->action_quantize_none->setChecked( quantizeMode == QuantizeMode::NONE );
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
        ui->tool_action_toggle_grid_visible->setChecked( gridVisible );
        ui->menu_action_toggle_grid_visible->setChecked( gridVisible );
    }

    void WindowMainView::reflectAutoScrollSettings(){
        bool autoScroll = Settings::instance()->isAutoScroll();
        ui->action_toggle_auto_scroll->setChecked( autoScroll );
    }

    void WindowMainView::setQuantizeMode( bool checked, QuantizeMode::QuantizeModeEnum mode ){
        if( checked ){
            Settings::instance()->setQuantizeMode( mode );
            reflectQuantizeModeSettings();
        }
    }

    void WindowMainView::setToolKind( ToolKind::ToolKindEnum kind ){
        reflectToolKind();
    }

    void WindowMainView::reflectToolKind(){
        ToolKind::ToolKindEnum kind = controllerAdapter->getToolKind();
        ui->action_toggle_pointer_tool->setChecked( kind == ToolKind::POINTER );
        ui->action_toggle_eraser_tool->setChecked( kind == ToolKind::ERASER );
        ui->action_toggle_line_tool->setChecked( kind == ToolKind::LINE );
        ui->action_toggle_pencil_tool->setChecked( kind == ToolKind::PENCIL );
    }

    void WindowMainView::notifyCommandHistoryChanged(){
        ui->action_undo->setEnabled( controllerAdapter->canUndo() );
        ui->action_redo->setEnabled( controllerAdapter->canRedo() );
        ui->tool_action_undo->setEnabled( controllerAdapter->canUndo() );
        ui->tool_action_redo->setEnabled( controllerAdapter->canRedo() );
        updateWidget();
    }

    void WindowMainView::showWidget(){
        show();
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

void cadencii::WindowMainView::on_action_quantize_quarter_toggled( bool arg1 ){
    setQuantizeMode( arg1, QuantizeMode::QUARTER );
}

void cadencii::WindowMainView::on_action_quantize_eighth_toggled( bool arg1 ){
    setQuantizeMode( arg1, QuantizeMode::EIGHTH );
}

void cadencii::WindowMainView::on_action_quantize_sixteenth_toggled( bool arg1 ){
    setQuantizeMode( arg1, QuantizeMode::SIXTEENTH );
}

void cadencii::WindowMainView::on_action_quantize_thirty_second_toggled( bool arg1 ){
    setQuantizeMode( arg1, QuantizeMode::THIRTY_SECOND );
}

void cadencii::WindowMainView::on_action_quantize_sixty_fourth_toggled( bool arg1 ){
    setQuantizeMode( arg1, QuantizeMode::SIXTY_FOURTH );
}

void cadencii::WindowMainView::on_action_quantize_hundred_twenty_eighth_toggled( bool arg1 ){
    setQuantizeMode( arg1, QuantizeMode::HUNDRED_TWENTY_EIGHTH );
}

void cadencii::WindowMainView::on_action_quantize_none_toggled( bool arg1 ){
    setQuantizeMode( arg1, QuantizeMode::NONE );
}

void cadencii::WindowMainView::on_action_move_song_position_left_triggered(){
    controllerAdapter->moveSongPositionStepped( true );
}

void cadencii::WindowMainView::on_action_move_song_position_right_triggered(){
    controllerAdapter->moveSongPositionStepped( false );
}

void cadencii::WindowMainView::on_action_toggle_auto_scroll_toggled( bool arg1 ){
    Settings::instance()->setAutoScroll( arg1 );
}

void cadencii::WindowMainView::on_action_export_as_musicxml_triggered(){
    QString filter;
    filter += tr( "MusicXML(*.xml)" );
    filter += ";;";
    filter += tr( "All Files(*.*)" );
    QString filePath = QFileDialog::getSaveFileName( this, "", "", filter );
    controllerAdapter->exportAsMusicXml( filePath.toLocal8Bit().data() );
    activateWindow();
}

void cadencii::WindowMainView::on_action_toggle_pointer_tool_triggered(){
    controllerAdapter->setToolKind( ToolKind::POINTER );
}

void cadencii::WindowMainView::on_action_toggle_pencil_tool_triggered(){
    controllerAdapter->setToolKind( ToolKind::PENCIL );
}

void cadencii::WindowMainView::on_action_toggle_line_tool_triggered(){
    controllerAdapter->setToolKind( ToolKind::LINE );
}

void cadencii::WindowMainView::on_action_toggle_eraser_tool_triggered(){
    controllerAdapter->setToolKind( ToolKind::ERASER );
}

void cadencii::WindowMainView::on_action_undo_triggered(){
    controllerAdapter->undo();
}

void cadencii::WindowMainView::on_action_redo_triggered(){
    controllerAdapter->redo();
}

void cadencii::WindowMainView::on_tool_action_undo_triggered(){
    controllerAdapter->undo();
}

void cadencii::WindowMainView::on_tool_action_redo_triggered(){
    controllerAdapter->redo();
}

void cadencii::WindowMainView::on_action_delete_triggered(){
    controllerAdapter->removeSelectedItems();
}

void cadencii::WindowMainView::on_action_export_as_vsq_file_triggered(){
    QString filter;
    filter += tr( "VSQ Format(*.vsq)" );
    filter += ";;";
    filter += tr( "All Files(*.*)" );
    QString filePath = QFileDialog::getSaveFileName( this, "", "", filter );
    if( !filePath.isEmpty() ){
        controllerAdapter->exportAsVSQFile( filePath.toLocal8Bit().data() );
    }
    activateWindow();
}

void cadencii::WindowMainView::on_menu_action_toggle_grid_visible_toggled( bool arg1 ){
    if( Settings::instance()->isGridVisible() != arg1 ){
        Settings::instance()->setGridVisible( arg1 );
        reflectGridVisibleSettings();
        updateWidget();
    }
}

void cadencii::WindowMainView::on_tool_action_toggle_grid_visible_toggled( bool arg1 ){
    if( Settings::instance()->isGridVisible() != arg1 ){
        Settings::instance()->setGridVisible( arg1 );
        reflectGridVisibleSettings();
        updateWidget();
    }
}
