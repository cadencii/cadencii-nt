/**
 * MainView.hpp
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
#ifndef __MainView_hpp__
#define __MainView_hpp__

#include "TrackView.hpp"
#include "ControlChangeView.hpp"
#include "BarCountView.hpp"
#include "TempoView.hpp"
#include "TimesigView.hpp"
#include "enum/ToolKind.hpp"

namespace cadencii{

    class ControllerAdapter;

    class MainView{
    public:
        virtual void setTrackView( TrackView *trackView ) = 0;

        virtual void setControlChangeView( ControlChangeView *controlChangeView ) = 0;

        virtual void setBarCountView( BarCountView *barCountView ) = 0;

        virtual void setTempoView( TempoView *tempoView ) = 0;

        virtual void setTimesigView( TimesigView *timesigView ) = 0;

        virtual void setControllerAdapter( ControllerAdapter *controllerAdapter ) = 0;

        /**
         * @brief ウィジェットを再描画する
         */
        virtual void updateWidget() = 0;

        /**
         * @brief 選択されたツールの種類を設定する
         */
        virtual void setToolKind( ToolKind::ToolKindEnum kind ) = 0;
    };

}

#endif
