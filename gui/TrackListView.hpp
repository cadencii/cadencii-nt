/**
 * TrackListView.hpp
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
#ifndef CADENCII_GUI_TRACKLISTVIEW_HPP_
#define CADENCII_GUI_TRACKLISTVIEW_HPP_

#include "../ControllerAdapter.hpp"

namespace cadencii {

    /**
     * @brief Track list view.
     */
    class TrackListView {
    public:
        /**
         * @brief Set an instance of adapter.
         */
        virtual void setControllerAdapter(ControllerAdapter *controllerAdapter) = 0;

        /**
         * @brief Get an widget instance.
         */
        virtual void *getWidget() = 0;

        /**
         * @brief Repaint component.
         */
        virtual void updateWidget() = 0;

        /**
         * @brief Set track index.
         * @param index [in] Track index.
         */
        virtual void setTrackIndex(int index) = 0;
    };
}

#endif
