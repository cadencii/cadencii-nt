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

namespace cadencii{
    using namespace std;

    ConcreteSettings::ConcreteSettings(){
        settings = new QSettings( "cadencii", "cadencii" );
        QString v = settings->value( "quantizeMode" ).toString();
        quantizeMode = QuantizeMode::fromString( v.toStdString() );
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
        settings->setValue( "quantizeMode", m );
    }

    void ConcreteSettings::save(){
        setQuantizeMode( quantizeMode );
    }

}
