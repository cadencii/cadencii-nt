/**
 * TrackView.hpp
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
#ifndef __TrackView_hpp__
#define __TrackView_hpp__

#include "../ControllerAdapter.hpp"
#include <libvsq/libvsq.h>

namespace cadencii {

    /**
     * @brief 画面へのトラック描画を担当するviewとの仲立ちを行うための抽象クラス
     */
    class TrackView {
    public:
        virtual ~TrackView() {
        }

        /**
         * @brief 指定した位置の音符が可視となるようスクロールする
         * @param tick 時刻
         * @param length 音符の長さ
         * @param noteNumber ノート番号。負の値が指定された場合、縦方向のスクロールは行わない
         */
        virtual void ensureNoteVisible(
            vsq::tick_t tick, vsq::tick_t length, int noteNumber = -1) = 0;

        /**
         * @brief ウィジェットの実体を返す
         * @return ウィジェットの実体
         */
        virtual void *getWidget() = 0;

        /**
         * @brief 描画範囲の左端の、tick 単位の時刻を設定する
         * @param drawOffset 描画範囲の左端の時刻
         */
        virtual void setDrawOffset(vsq::tick_t drawOffset) = 0;

        /**
         * @brief 表示するトラックの番号を指定する
         * @param index トラックの番号
         */
        virtual void setTrackIndex(int index) = 0;

        virtual void setControllerAdapter(ControllerAdapter *controllerAdapter) = 0;

        /**
         * @brief ウィジェットを再描画する
         */
        virtual void updateWidget() = 0;

        /**
         * @brief トラックビューの幅(ピクセル単位)を取得する
         * @return トラックビューの幅(ピクセル単位)
         */
        virtual int getTrackViewWidth() = 0;
    };
}

#endif
