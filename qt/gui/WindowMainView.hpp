/**
 * WindowMainView.hpp
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
#ifndef __WindowMainView_hpp__
#define __WindowMainView_hpp__

#include <QMainWindow>
#include "gui/MainView.hpp"
#include "enum/QuantizeMode.hpp"

namespace Ui{
    class WindowMainView;
}

namespace cadencii{
    class WindowMainView : public QMainWindow, public MainView{
        Q_OBJECT
    private:
        Ui::WindowMainView *ui;
        ControllerAdapter *controllerAdapter;

    public:
        explicit WindowMainView( QWidget *parent = 0 );
        ~WindowMainView();

        void setTrackView( TrackView *trackView );

        void setControlChangeView( ControlChangeView *controlChangeView );

        void setBarCountView( BarCountView *barCountView );

        void setTempoView( TempoView *tempoView );

        void setTimesigView( TimesigView *timesigView );

        void setPropertyView( PropertyView *propertyView );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void updateWidget();

        void setToolKind( ToolKind::ToolKindEnum kind );

        void notifyCommandHistoryChanged();

    private:
        /**
         * @brief ピアノロールウィジェットを設定する
         */
        void setupPianorollWidget();

        /**
         * @brief カスタマイズ可能な設定項目について、Settings::instance の内容をビューに反映する
         */
        void reflectSettings();

        /**
         * @brief QuantizeMode の設定内容をビューに反映する
         */
        void reflectQuantizeModeSettings();

        /**
         * @brief GridVisible の設定内容をビューに反映する
         */
        void reflectGridVisibleSettings();

        /**
         * @brief autoScroll の設定内容をビューに反映する
         */
        void reflectAutoScrollSettings();

        /**
         * @brief クオンタイズモードを設定する
         * @param checked クオンタイズモードを変更する QAction のチェックが入っているかどうか
         * @param mode クオンタイズモード
         */
        void setQuantizeMode( bool checked, QuantizeMode::QuantizeModeEnum mode );

        /**
         * @brief ツールの選択状態を画面に反映する
         */
        void reflectToolKind();

    private slots:
        void on_action_open_vsq_vocaloid_midi_triggered();
        void on_action_quantize_quarter_toggled( bool arg1 );
        void on_action_quantize_eighth_toggled( bool arg1 );
        void on_action_quantize_sixteenth_toggled( bool arg1 );
        void on_action_quantize_thirty_second_toggled( bool arg1 );
        void on_action_quantize_sixty_fourth_toggled( bool arg1 );
        void on_action_quantize_hundred_twenty_eighth_toggled( bool arg1 );
        void on_action_quantize_none_toggled( bool arg1 );
        void on_action_move_song_position_left_triggered();
        void on_action_move_song_position_right_triggered();
        void on_action_toggle_grid_visible_toggled( bool arg1 );
        void on_action_toggle_auto_scroll_toggled( bool arg1 );
        void on_action_export_as_musicxml_triggered();
        void on_action_toggle_pointer_tool_triggered();
        void on_action_toggle_pencil_tool_triggered();
        void on_action_toggle_line_tool_triggered();
        void on_action_toggle_eraser_tool_triggered();
        void on_action_undo_triggered();
        void on_action_redo_triggered();
        void on_tool_action_undo_triggered();
        void on_tool_action_redo_triggered();
    };

}
#endif // __WindowMainView_h__
