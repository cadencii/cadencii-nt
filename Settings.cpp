/**
 * Settings.cpp
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
#include "Settings.hpp"
#include <QSettings>

static QSettings *settings = 0;

namespace cadencii{
    using namespace std;

    Settings &Settings::instance(){
        static Settings i;
        return i;
    }

    Settings::Settings(){
        settings = new QSettings( "cadencii", "cadencii" );
    }

    Settings::~Settings(){
        if( settings ){
            delete settings;
            settings = 0;
        }
    }

    QuantizeMode::QuantizeModeEnum Settings::getQuantizeMode(){
        QString v = settings->value( "quantizeMode" ).toString();
        return QuantizeMode::fromString( v.toStdString() );
    }

    void Settings::setQuantizeMode( QuantizeMode::QuantizeModeEnum mode ){
        string modeText = QuantizeMode::toString( mode );
        QVariant m( modeText.c_str() );
        settings->setValue( "quantizeMode", m );
    }

}
