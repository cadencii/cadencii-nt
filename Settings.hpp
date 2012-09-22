/**
 * Settings.hpp
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
#ifndef __Settings_hpp__
#define __Settings_hpp__

#include "enum/QuantizeMode.hpp"

namespace cadencii{

    /**
     * @brief カスタマイズ可能な、アプリの設定項目を保持する
     */
    class Settings{
    public:
        /**
         * @brief 設定項目を取得する。引数 newInstance が指定された場合、設定項目のインスタンスを置き換える
         * @param newInstance 置き換える設定項目のインスタンス
         * @return 設定項目
         */
        static Settings *instance( Settings *newInstance = 0 ){
            static Settings *settings;
            if( newInstance ){
                settings = newInstance;
            }
            return settings;
        }

        /**
         * @brief クオンタイズモードを取得する
         */
        virtual QuantizeMode::QuantizeModeEnum getQuantizeMode() = 0;

        /**
         * @brief クオンタイズモードを設定する
         */
        virtual void setQuantizeMode( QuantizeMode::QuantizeModeEnum mode ) = 0;

        /**
         * @brief 設定項目を保存する
         */
        virtual void save() = 0;
    };

}

#endif
