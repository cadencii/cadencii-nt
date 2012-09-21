/**
 * ControllerAdapter.hpp
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
#ifndef __ControllerAdapter_hpp__
#define __ControllerAdapter_hpp__

#include "vsq/vsqglobal.hpp"
#include <string>

namespace cadencii{

    class ControllerAdapter{
    public:
        /**
         * @brief VSQ ファイルを開く
         * @param[in] 開くVSQのパス
         */
        virtual void openVSQFile( const std::string &filePath )throw() = 0;

        /**
         * @brief スクロールされるなどして、時間方向の描画範囲がかわった場合に、他のコンポーネントに対してそれを通知したい場合に呼び出す。
         * @param sender 描画範囲が変わったコンポーネント
         * @param offset tick 単位の描画オフセット
         */
        virtual void drawOffsetChanged( void *sender, VSQ_NS::tick_t offset )throw() = 0;

        /**
         * @brief 表示するトラックの番号が変更になった場合に、他のコンポーネントに対してそれを通知したい場合に呼び出す。
         * @param sender 変更が最初におこったコンポーネント
         * @param index トラックの番号
         */
        virtual void setTrackIndex( void *sender, int index )throw() = 0;

        /**
         * @brief 現在のソングポジションを取得する
         * @param ソングポジション
         */
        virtual VSQ_NS::tick_t getSongPosition()throw() = 0;

        /**
         * @brief tick 単位の時刻から、描画時の x 座標を取得する
         * @param tick 時刻
         * @return x 座標
         */
        virtual int getXFromTick( VSQ_NS::tick_t tick )throw() = 0;

        /**
         * @brief x 座標から、tick 単位の時刻を取得する
         */
        virtual double getTickFromX( int x )throw() = 0;
    };

}

#endif
