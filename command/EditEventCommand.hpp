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

#include "AbstractCommand.hpp"

namespace cadencii{

    /**
     * @brief 音符・歌手イベントを編集する
     */
    class EditEventCommand : public AbstractCommand{
    private:
        int track;
        int eventId;
        VSQ_NS::Event item;

    public:
        /**
         * @brief 初期化する
         * @param track 編集対象のトラック
         * @param eventId 編集対象のイベントID(=Event::id)
         * @param item 編集後のイベント
         */
        explicit EditEventCommand( int track, int eventId, const VSQ_NS::Event &item ){
            this->track = track;
            this->eventId = eventId;
            this->item = item;
        }

        AbstractCommand *execute( VSQ_NS::Sequence *sequence ){
            VSQ_NS::Track *target = &sequence->track[track];
            int index = target->getEvents()->findIndexFromId( eventId );
            const VSQ_NS::Event original = *(target->getEvents()->get( index ));
            VSQ_NS::Event replace = item;
            replace.id = eventId;
            target->getEvents()->set( index, replace );
            return new EditEventCommand( track, eventId, original );
        }
    };

}

#endif
