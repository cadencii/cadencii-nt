/**
 * ToolKind.hpp
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
#ifndef __cadencii_enum_ToolKind_hpp__
#define __cadencii_enum_ToolKind_hpp__

namespace cadencii{

    class ToolKind{
    public:
        /**
         * @brief 編集ツールを表す列挙型
         */
        enum ToolKindEnum{
            /**
             * @brief 矢印ツール
             */
            ARROW,
            /**
             * @brief 鉛筆ツール
             */
            PENCIL,
            /**
             * @brief 直線ツール
             */
            LINE,
            /**
             * @brief 消しゴムツール
             */
            ERASER
        };
    };

}

#endif
