/**
 * TrackView.hpp
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
#ifndef __TrackView_hpp__
#define __TrackView_hpp__

#include "vsq/TimesigList.hpp"
#include "vsq/Track.hpp"

namespace cadencii{

    /**
     * @brief 画面へのトラック描画を担当するviewとの仲立ちを行うための抽象クラス
     */
    class TrackView{
    public:
        /**
         * @brief ソングポジションを設定する
         * @param songPosition ソングポジション
         * @param autoScroll 自動でスクロールするかどうか
         */
        virtual void setSongPosition( VSQ_NS::tick_t songPosition, bool autoScroll ) = 0;

        /**
         * @brief テンポ変更リストを設定する
         * @param timesigList テンポ変更リスト
         */
        virtual void setTimesigList( VSQ_NS::TimesigList *timesigList ) = 0;

        /**
         * @brief 描画対象のトラックを設定する
         * @param items 描画対象のトラック
         */
        virtual void setTrack( VSQ_NS::Track *track ) = 0;

        /**
         * @brief 指定した位置の音符が可視となるようスクロールする
         * @param tick 時刻
         * @param length 音符の長さ
         * @param noteNumber ノート番号
         */
        virtual void ensureNoteVisible( VSQ_NS::tick_t tick, VSQ_NS::tick_t length, int noteNumber ) = 0;

        /**
         * @brief ウィジェットの実体を返す
         * @return ウィジェットの実体
         */
        virtual void *getWidget() = 0;
    };

}
#endif
