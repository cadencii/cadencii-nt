/**
 * EditEventCommand.hpp
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
#ifndef __cadencii_command_EditEventCommand_hpp__
#define __cadencii_command_EditEventCommand_hpp__

#include <map>
#include "AbstractCommand.hpp"

namespace cadencii {

    /**
     * @brief 音符・歌手イベントを編集する
     */
    class EditEventCommand : public AbstractCommand {
    private:
        int track;
        std::map<int, std::shared_ptr<vsq::Event>> itemList;

    public:
        /**
         * @brief 初期化する
         * @param track 編集対象のトラック
         * @param eventId 編集対象のイベントID(=Event::id)
         * @param item 編集後のイベント
         */
        EditEventCommand(int track, int eventId, vsq::Event const& item) {
            this->track = track;
            itemList[eventId] = std::make_shared<vsq::Event>(item);
        }

        /**
         * @brief 初期化する
         * @todo public に昇格する
         * @param track 編集対象のトラック
         * @param itemList アイテムIDとvsq::Eventのインスタンスのマップ
         */
        EditEventCommand(int track, std::map<int, std::shared_ptr<vsq::Event>> const& itemList) {
            this->track = track;
            this->itemList = itemList;
        }

        AbstractCommand *execute(vsq::Sequence *sequence) override {
            vsq::Track & target = sequence->track(track);
            auto i = itemList.begin();
            std::map<int, std::shared_ptr<vsq::Event>> originalItemList;
            for (; i != itemList.end(); ++i) {
                int eventId = i->first;
                std::shared_ptr<vsq::Event> const& item = i->second;
                int index = target.events().findIndexFromId(eventId);
                vsq::Event original = *target.events().get(index);
                originalItemList[eventId] = std::make_shared<vsq::Event>(original);
                vsq::Event replace = *item;
                replace.id = eventId;
                target.events().set(index, replace);
            }
            return new EditEventCommand(track, originalItemList);
        }
    };
}

#endif
