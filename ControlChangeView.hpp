/**
 * ControlChangeView.hpp
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
#ifndef __ControlChangeView_hpp__
#define __ControlChangeView_hpp__

#include "ControllerAdapter.hpp"
#include "vsq/TimesigList.hpp"
#include "vsq/Track.hpp"

namespace cadencii{

    /**
     * @brief 画面へのコントロールチェンジの描画を担当するviewとの仲立ちを行うための抽象クラス
     */
    class ControlChangeView{
    protected:
        ControllerAdapter *controllerAdapter;

    public:
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
         * @brief ウィジェットの実体を返す
         * @return ウィジェットの実体
         */
        virtual void *getWidget() = 0;

        /**
         * @brief 描画範囲の左端の、tick 単位の時刻を設定する
         * @param drawOffset 描画範囲の左端の時刻
         */
        virtual void setDrawOffset( VSQ_NS::tick_t drawOffset ) = 0;

        void setControllerAdapter( ControllerAdapter *controllerAdapter ){
            this->controllerAdapter = controllerAdapter;
        }
    };

}
#endif
