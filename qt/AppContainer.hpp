/**
 * AppContainer.hpp
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
#ifndef CADENCII_QT_APPCONTAINER_HPP_
#define CADENCII_QT_APPCONTAINER_HPP_

#include "qt/gui/WindowMainView.hpp"
#include "qt/gui/pianoroll_track_view/PianorollTrackView.hpp"
#include "qt/gui/CurveControlChangeView.hpp"
#include "qt/gui/ConcreteBarCountView.hpp"
#include "qt/gui/ConcreteTempoView.hpp"
#include "qt/gui/ConcreteTimesigView.hpp"
#include "qt/gui/concrete_property_view/ConcretePropertyView.hpp"
#include "qt/gui/ConcreteSingerListView.hpp"
#include "qt/gui/ConcreteTrackListView.hpp"
#include "Controller.hpp"

namespace cadencii {

    class AppContainer {
    public:
        cadencii::WindowMainView windowMainView;
        cadencii::PianorollTrackView pianorollTrackView;
        cadencii::CurveControlChangeView *curveControlChangeView;
        cadencii::ConcreteBarCountView concreteBarCountView;
        cadencii::ConcreteTempoView concreteTempoView;
        cadencii::ConcreteTimesigView concreteTimesigView;
        cadencii::ConcretePropertyView concretePropertyView;
        cadencii::ConcreteSingerListView concreteSingerListView;
        cadencii::ConcreteTrackListView *concreteTrackListView;
        cadencii::Controller controller;

    public:
        explicit AppContainer() {
            curveControlChangeView = new CurveControlChangeView();
            concreteTrackListView = new ConcreteTrackListView();
            controller.setMainView(&windowMainView);
            controller.setTrackView(&pianorollTrackView);
            controller.setControlChangeView(curveControlChangeView);
            controller.setBarCountView(&concreteBarCountView);
            controller.setTempoView(&concreteTempoView);
            controller.setTimesigView(&concreteTimesigView);
            controller.setPropertyView(&concretePropertyView);
            controller.setSingerListView(&concreteSingerListView);
            controller.setTrackListView(concreteTrackListView);
        }

        ~AppContainer() {
            if (curveControlChangeView) {
                delete curveControlChangeView;
                curveControlChangeView = 0;
            }
            if (concreteTrackListView) {
                delete concreteTrackListView;
                concreteTrackListView = 0;
            }
        }
    };
}

#endif
