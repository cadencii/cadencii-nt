/**
 * ItemSelectionStatusListener.hpp
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
#ifndef __cadencii_ItemSelectionStatusListener_hpp__
#define __cadencii_ItemSelectionStatusListener_hpp__

namespace cadencii {

    /**
     * @brief アイテムの選択状態をモニターする
     */
    class ItemSelectionStatusListener {
    public:
        virtual ~ItemSelectionStatusListener() {
        }

        /**
         * @brief アイテムの選択状態が変化した場合に呼ばれる
         */
        virtual void statusChanged() = 0;
    };
}

#endif
