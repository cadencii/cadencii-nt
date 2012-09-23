/**
 * ConcreteSettings.hpp
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
#ifndef __ConcreteSettings_hpp__
#define __ConcreteSettings_hpp__

#include "Settings.hpp"
#include <QSettings>

namespace cadencii{

    class ConcreteSettings : public Settings{
    protected:
        QuantizeMode::QuantizeModeEnum quantizeMode;
        bool gridVisible;
        bool autoScroll;
        QSettings *settings;

    public:
        ConcreteSettings();

        ~ConcreteSettings();

        QuantizeMode::QuantizeModeEnum getQuantizeMode();

        void setQuantizeMode( QuantizeMode::QuantizeModeEnum mode );

        bool isGridVisible();

        void setGridVisible( bool isVisible );

        bool isAutoScroll();

        void setAutoScroll( bool isAutoScroll );

        void save();
    };

}

#endif
