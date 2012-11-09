/**
 * QuantizeMode.hpp
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
#ifndef __QuantizeMode_hpp__
#define __QuantizeMode_hpp__

#include <string>
#include "../vsq/StringUtil.hpp"
#include "../vsq/vsqglobal.hpp"

namespace cadencii {

    /**
     * @brief クオンタイズのモードを表す列挙子型
     */
    namespace QuantizeMode {
        enum QuantizeModeEnum {
            /**
             * @brief クオンタイズを行わない
             */
            NONE,
            /**
             * @brief 4 分音符の音価でクオンタイズする
             */
            QUARTER,
            /**
             * @brief 8 分音符の音価でクオンタイズする
             */
            EIGHTH,
            /**
             * @brief 16 分音符の音価でクオンタイズする
             */
            SIXTEENTH,
            /**
             * @brief 32 分音符の音価でクオンタイズする
             */
            THIRTY_SECOND,
            /**
             * @brief 64 分音符の音価でクオンタイズする
             */
            SIXTY_FOURTH,
            /**
             * @brief 128 分音符の音価でクオンタイズする
             */
            HUNDRED_TWENTY_EIGHTH
        };

        /**
         * @brief 列挙子型のインスタンスを文字列表現に変える
         * @param value 変換対象の列挙子型
         * @return 文字列表現
         */
        static std::string toString(QuantizeModeEnum value) {
            switch (value) {
                case NONE: {
                    return "NONE";
                }
                case QUARTER: {
                    return "QUARTER";
                }
                case EIGHTH: {
                    return "EIGHTH";
                }
                case SIXTEENTH: {
                    return "SIXTEENTH";
                }
                case THIRTY_SECOND: {
                    return "THIRTY_SECOND";
                }
                case SIXTY_FOURTH: {
                    return "SIXTY_FOURTH";
                }
                case HUNDRED_TWENTY_EIGHTH: {
                    return "HUNDRED_TWENTY_EIGHTH";
                }
                default: {
                    return "";
                }
            }
        }

        /**
         * @brief 文字列から、該当する QuantizeModeEnum の値を取得する
         * @param text 文字列
         * @return 文字列に該当する QuantizeModeEnum の値。該当するものがない場合は、NONE を返す
         */
        static QuantizeModeEnum fromString(const std::string &text) {
            std::string lower = StringUtil::toLower(text);
            if (lower == "none") {
                return QuantizeMode::NONE;
            } else if (lower == "quarter") {
                return QuantizeMode::QUARTER;
            } else if (lower == "eighth") {
                return QuantizeMode::EIGHTH;
            } else if (lower == "sixteenth") {
                return QuantizeMode::SIXTEENTH;
            } else if (lower == "thirty_second") {
                return QuantizeMode::THIRTY_SECOND;
            } else if (lower == "sixty_fourth") {
                return QuantizeMode::SIXTY_FOURTH;
            } else if (lower == "hundred_twenty_eighth") {
                return QuantizeMode::HUNDRED_TWENTY_EIGHTH;
            } else {
                return QuantizeMode::NONE;
            }
        }

        /**
         * @brief クオンタイズの際の、単位時間(tick単位)を取得する。例えば、4分音符でクオンタイズする場合、480tickを返す
         * @param mode クオンタイズ
         * @return 単位時間
         */
        static VSQ_NS::tick_t getQuantizeUnitTick(QuantizeModeEnum mode) {
            VSQ_NS::tick_t result;
            switch (mode) {
                case NONE: {
                    result = 1;
                    break;
                }
                case QUARTER: {
                    result = 480;
                    break;
                }
                case EIGHTH: {
                    result = 240;
                    break;
                }
                case SIXTEENTH: {
                    result = 120;
                    break;
                }
                case THIRTY_SECOND: {
                    result = 60;
                    break;
                }
                case SIXTY_FOURTH: {
                    result = 30;
                    break;
                }
                case HUNDRED_TWENTY_EIGHTH: {
                    result = 15;
                    break;
                }
                default: {
                    result = 1;
                    break;
                }
            }
            return result;
        }
    }
}

#endif
