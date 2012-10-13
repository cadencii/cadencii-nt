/**
 * Controller.hpp
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
#ifndef __Controller_hpp__
#define __Controller_hpp__

#include "gui/MainView.hpp"
#include "gui/TrackView.hpp"
#include "gui/ControlChangeView.hpp"
#include "gui/PropertyView.hpp"
#include "ControllerAdapter.hpp"
#include "vsq/Sequence.hpp"
#include "enum/QuantizeMode.hpp"
#include "SequenceModel.hpp"

namespace cadencii{

    class Controller : public ControllerAdapter{
    private:
        TrackView *trackView;
        MainView *mainView;
        ControlChangeView *controlChangeView;
        BarCountView *barCountView;
        TempoView *tempoView;
        TimesigView *timesigView;
        PropertyView *propertyView;
        SequenceModel model;
        VSQ_NS::tick_t songPosition;

        /**
         * @brief 1 tick 時刻を何ピクセルで描画するか
         */
        double pixelPerTick;

        /**
         * @brief 選択されているツールの種類
         */
        ToolKind::ToolKindEnum toolKind;

        /**
         * @brief アイテムの選択状態を管理するマネージャオブジェクト
         */
        ItemSelectionManager itemSelectionManager;

    public:
        explicit Controller();

        void openVSQFile( const string &filePath )throw();

        void drawOffsetChanged( void *sender, VSQ_NS::tick_t offset )throw();

        /**
         * @brief 表示するトラックのインデックスを設定する
         * @param トラックのインデックス
         */
        void setTrackIndex( void *sender, int index )throw();

        VSQ_NS::tick_t getSongPosition()throw();

        int getXFromTick( VSQ_NS::tick_t tick )throw();

        double getTickFromX( int x )throw();

        void moveSongPositionStepped( bool isBackward )throw();

        int getPreferedComponentWidth()throw();

        void exportAsMusicXml( const std::string &filePath )throw();

        void setToolKind( ToolKind::ToolKindEnum kind )throw();

        ToolKind::ToolKindEnum getToolKind()throw();

        ItemSelectionManager *getItemSelectionManager()throw();

        const VSQ_NS::Sequence *getSequence()throw();

        void undo();

        void redo();

        bool canUndo();

        bool canRedo();

        void execute( AbstractCommand *command );

        void notifyItemSelectionChange()throw();

        /**
         * @brief トラックのビューを設定する
         * @param[in] trackView ビュー
         */
        void setTrackView( TrackView *trackView )throw();

        /**
         * @brief コントロールチェンジのビューを設定する
         * @param [in] controlChangeView ビュー
         */
        void setControlChangeView( ControlChangeView *controlChangeView )throw();

        /**
         * @brief 小節数のビューを設定する
         * @param [in] barCountView ビュー
         */
        void setBarCountView( BarCountView *barCountView )throw();

        /**
         * @brief テンポ変更イベントのビューを設定する
         * @param [in] tempoView ビュー
         */
        void setTempoView( TempoView *tempoView )throw();

        /**
         * @brief 拍子変更イベントのビューを設定する
         * @param [in] timesigView ビュー
         */
        void setTimesigView( TimesigView *timesigView )throw();

        /**
         * @brief メインのビューを設定する
         * @param[in] mainView ビュー
         */
        void setMainView( MainView *mainView )throw();

        /**
         * @brief プロパティーのビューを設定する
         * @param[in] propertyView ビュー
         */
        void setPropertyView( PropertyView *propertyView )throw();

        /**
         * @brief メインのビューを表示する
         */
        void showMainView();

    protected:
        /**
         * @brief tick 単位の時刻を、指定されたクオンタイズモードに応じて丸める
         * @todo 三連符かどうかのフラグを渡せるようにする
         * @todo テスト
         */
        VSQ_NS::tick_t getQuantizedTick( VSQ_NS::tick_t before, QuantizeMode::QuantizeModeEnum mode );

    private:
        /**
         * @brief sequence のインスタンスを置き換えたのち、各コンポーネントに sequence のインスタンスが置き換わったことを通知する
         * @param sequence 置き換わるインスタンス
         */
        void setupSequence( const VSQ_NS::Sequence &sequence );

        /**
         * @brief すべてのウィジェットを再描画する
         */
        void updateAllWidget();

        /**
         * @brief ソングポジションを設定する
         * @param songPosition ソングポジション
         */
        void setSongPosition( VSQ_NS::tick_t songPosition );
    };

}

#endif
