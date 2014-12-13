/**
 * AddEventCommand.cpp
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
#include "AddEventCommand.hpp"
#include "DeleteEventCommand.hpp"

namespace cadencii {

    AddEventCommand::AddEventCommand(int track, const std::vector<vsq::Event> &eventList) {
        this->track = track;
        this->eventList = eventList;
    }

    AbstractCommand *AddEventCommand::execute(vsq::Sequence *sequence) {
        vsq::Event::List &list = sequence->track(track).events();
        std::vector<int> idList;

        auto i = eventList.begin();
        for (; i != eventList.end(); ++i) {
            idList.push_back(list.add((*i)));
        }
        return new DeleteEventCommand(track, idList);
    }
}
