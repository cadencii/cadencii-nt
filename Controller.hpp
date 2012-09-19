/**
 * Controller.hpp
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
#ifndef __Controller_hpp__
#define __Controller_hpp__

#include "MainView.hpp"
#include "TrackView.hpp"
#include "ControlChangeView.hpp"
#include "ControllerAdapter.hpp"
#include "vsq/Sequence.hpp"

namespace cadencii{

    class Controller : public ControllerAdapter{
    private:
        TrackView *trackView;
        MainView *mainView;
        ControlChangeView *controlChangeView;
        BarCountView *barCountView;
        VSQ_NS::Sequence sequence;
        VSQ_NS::tick_t songPosition;

        /**
         * @brief 1 tick 時刻を何ピクセルで描画するか
         */
        double pixelPerTick;

    public:
        explicit Controller();

        void openVSQFile( const string &filePath )throw();

        void drawOffsetChanged( void *sender, VSQ_NS::tick_t offset )throw();

        VSQ_NS::tick_t getSongPosition()throw();

        int getXFromTick( VSQ_NS::tick_t tick )throw();

        double getTickFromX( int x )throw();

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
         * @brief メインのビューを設定する
         * @param[in] mainView ビュー
         */
        void setMainView( MainView *mainView )throw();

        /**
         * @brief 表示するトラックのインデックスを設定する
         * @param トラックのインデックス
         */
        void setTrackIndex( void *sender, int index )throw();

    private:
        /**
         * @brief コンポーネントに sequence のインスタンスを設定する
         */
        void setupSequence();
    };

}

#endif
