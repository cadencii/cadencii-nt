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
         * @brief 設定項目のインスタンスを取得する
         * @return 設定項目のインスタンス
         */
        static Settings &instance();

        /**
         * @brief クオンタイズモードを取得する
         */
        QuantizeMode::QuantizeModeEnum getQuantizeMode();

        /**
         * @brief クオンタイズモードを設定する
         */
        void setQuantizeMode( QuantizeMode::QuantizeModeEnum mode );

    private:
        /**
         * @brief 保存されている設定項目があればそれを読み取り、初期化する
         */
        Settings();

        /**
         * @brief デストラクタ
         */
        ~Settings();
    };

}

#endif
