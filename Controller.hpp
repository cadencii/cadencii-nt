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
        VSQ_NS::Sequence sequence;

    public:
        explicit Controller();

        void openVSQFile( const string &filePath )throw();

        void drawOffsetChanged( void *sender, VSQ_NS::tick_t offset )throw();

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
         * @brief メインのビューを設定する
         * @param[in] mainView ビュー
         */
        void setMainView( MainView *mainView )throw();
    };

}

#endif
