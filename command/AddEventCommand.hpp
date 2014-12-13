/**
 * AddEventCommand.hpp
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
#ifndef __cadencii_AddEventCommand_hpp__
#define __cadencii_AddEventCommand_hpp__

#include <vector>
#include "DeleteEventCommand.hpp"

namespace cadencii {

    /**
     * @brief イベントを追加する
     */
    class AddEventCommand : public AbstractCommand {
    private:
        /**
         * @brief 追加先のトラック番号
         */
        int track;

        /**
         * @brief 追加するイベントのリスト
         */
        std::vector<vsq::Event> eventList;

    public:
        /**
         * @brief コマンドを初期化する
         * @param track 追加先のトラック番号
         * @param eventList 追加するイベント乗り周防t
         */
        explicit AddEventCommand(int track, const std::vector<vsq::Event> &eventList);

        AbstractCommand *execute(vsq::Sequence *sequence);
    };
}

#endif
