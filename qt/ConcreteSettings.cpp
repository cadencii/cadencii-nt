/**
 * ConcreteSettings.cpp
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
#include "qt/ConcreteSettings.hpp"
#include <QSettings>

#define CADENCII_SETTINGS_QUANTIZE_MODE "quantizeMode"
#define CADENCII_SETTINGS_GRID_VISIBLE "gridVisible"

namespace cadencii{
    using namespace std;

    ConcreteSettings::ConcreteSettings(){
        settings = new QSettings( "cadencii", "cadencii" );
        {
            QString v = settings->value( CADENCII_SETTINGS_QUANTIZE_MODE ).toString();
            quantizeMode = QuantizeMode::fromString( v.toStdString() );
        }
        {
            gridVisible = settings->value( CADENCII_SETTINGS_GRID_VISIBLE ).toBool();
        }
    }

    ConcreteSettings::~ConcreteSettings(){
        if( settings ){
            save();
            delete settings;
            settings = 0;
        }
    }

    QuantizeMode::QuantizeModeEnum ConcreteSettings::getQuantizeMode(){
        return quantizeMode;
    }

    void ConcreteSettings::setQuantizeMode( QuantizeMode::QuantizeModeEnum mode ){
        quantizeMode = mode;
        string modeText = QuantizeMode::toString( quantizeMode );
        QVariant m( modeText.c_str() );
        settings->setValue( CADENCII_SETTINGS_QUANTIZE_MODE, m );
    }

    void ConcreteSettings::save(){
        setQuantizeMode( quantizeMode );
        setGridVisible( gridVisible );
    }

    bool ConcreteSettings::isGridVisible(){
        return gridVisible;
    }

    void ConcreteSettings::setGridVisible( bool isVisible ){
        gridVisible = isVisible;
        QVariant m( isVisible );
        settings->setValue( CADENCII_SETTINGS_GRID_VISIBLE, m );
    }

}
