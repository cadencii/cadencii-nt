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
#define CADENCII_SETTINGS_AUTO_SCROLL "autoScroll"

namespace cadencii{
    using namespace std;

    ConcreteSettings::ConcreteSettings(){
        settings = new QSettings( "cadencii", "cadencii" );
        {
            QVariant d( QuantizeMode::toString( QuantizeMode::QUARTER ).c_str() );
            QString v = settings->value( CADENCII_SETTINGS_QUANTIZE_MODE, d ).toString();
            quantizeMode = QuantizeMode::fromString( v.toStdString() );
        }
        gridVisible = settings->value( CADENCII_SETTINGS_GRID_VISIBLE, QVariant( false ) ).toBool();
        autoScroll = settings->value( CADENCII_SETTINGS_AUTO_SCROLL, QVariant( true ) ).toBool();
    }

    ConcreteSettings::~ConcreteSettings(){
        if( settings ){
            save();
            delete settings;
            settings = 0;
        }
    }

    void ConcreteSettings::save(){
        setQuantizeMode( quantizeMode );
        setGridVisible( gridVisible );
        setAutoScroll( autoScroll );
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

    bool ConcreteSettings::isGridVisible(){
        return gridVisible;
    }

    void ConcreteSettings::setGridVisible( bool isVisible ){
        gridVisible = isVisible;
        QVariant m( gridVisible );
        settings->setValue( CADENCII_SETTINGS_GRID_VISIBLE, m );
    }

    bool ConcreteSettings::isAutoScroll(){
        return autoScroll;
    }

    void ConcreteSettings::setAutoScroll( bool isAutoScroll ){
        autoScroll = isAutoScroll;
        QVariant m( autoScroll );
        settings->setValue( CADENCII_SETTINGS_AUTO_SCROLL, m );
    }

}
