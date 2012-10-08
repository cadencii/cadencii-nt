/**
 * ControlChangeView.hpp
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
#ifndef __ControlChangeView_hpp__
#define __ControlChangeView_hpp__

#include "ControllerAdapter.hpp"
#include "vsq/TimesigList.hpp"

namespace cadencii{

    /**
     * @brief 画面へのコントロールチェンジの描画を担当するviewとの仲立ちを行うための抽象クラス
     */
    class ControlChangeView{
    public:
        /**
         * @brief ウィジェットの実体を返す
         * @return ウィジェットの実体
         */
        virtual void *getWidget() = 0;

        /**
         * @brief 描画範囲の左端の、tick 単位の時刻を設定する
         * @param drawOffset 描画範囲の左端の時刻
         */
        virtual void setDrawOffset( VSQ_NS::tick_t drawOffset ) = 0;

        /**
         * @brief 表示するトラックの番号を指定する
         * @param index トラックの番号
         */
        virtual void setTrackIndex( int index ) = 0;

        /**
         * @brief 表示するコントロールチェンジの名前を取得する
         */
        virtual void setControlChangeName( const std::string &name ) = 0;

        virtual void setControllerAdapter( ControllerAdapter *controllerAdapter ) = 0;

        /**
         * @brief ウィジェットを再描画する
         */
        virtual void updateWidget() = 0;
    };

}
#endif
