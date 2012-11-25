/**
 * DeleteEventCommand.cpp
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
#include <vector>
#include "DeleteEventCommand.hpp"
#include "AddEventCommand.hpp"

namespace cadencii {

    DeleteEventCommand::DeleteEventCommand(int track, const std::vector<int> &idList) {
        this->track = track;
        this->idList = idList;
    }

    AbstractCommand *DeleteEventCommand::execute(VSQ_NS::Sequence *sequence) {
        VSQ_NS::Event::List *list = sequence->track(track)->events();
        std::vector<VSQ_NS::Event> eventList;

        std::vector<int>::const_iterator i = idList.begin();
        for (; i != idList.end(); ++i) {
            int id = (*i);
            int index = list->findIndexFromId(id);
            if (0 <= index) {
                const VSQ_NS::Event *item = list->get(index);
                eventList.push_back(*item);
                list->removeAt(index);
            }
        }

        return new AddEventCommand(track, eventList);
    }
}
