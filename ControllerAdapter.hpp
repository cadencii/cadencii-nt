/**
 * ControllerAdapter.hpp
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
         * @brief 現在のソングポジションを取得する
         * @param ソングポジション
         */
        virtual VSQ_NS::tick_t getSongPosition()throw() = 0;
    };

}

#endif
