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
#include "vsq/Sequence.hpp"

namespace cadencii{

    class Controller{
    private:
        TrackView *trackView;
        MainView *mainView;
        VSQ_NS::Sequence sequence;

    public:
        explicit Controller();

        /**
         * @brief トラックのビューを設定する
         * @param[in] trackView ビュー
         */
        void setTrackView( TrackView *trackView )throw();

        /**
         * @brief メインのビューを設定する
         * @param[in] mainView ビュー
         */
        void setMainView( MainView *mainView )throw();

        /**
         * @brief VSQ ファイルを開く
         * @param[in] 開くVSQのパス
         */
        void openVSQFile( const string &filePath )throw();
    };

}

#endif
