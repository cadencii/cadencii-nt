/**
 * main.cpp
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
#include <QtGui/QApplication>
#include "qt/gui/WindowMainView.hpp"
#include "qt/gui/PianorollTrackView.hpp"
#include "qt/gui/CurveControlChangeView.hpp"
#include "qt/gui/ConcreteBarCountView.hpp"
#include "qt/gui/ConcreteTempoView.hpp"
#include "qt/gui/ConcreteTimesigView.hpp"
#include "qt/gui/ConcretePropertyView.hpp"
#include "qt/ConcreteSettings.hpp"
#include "Controller.hpp"

int main(int argc, char *argv[])
{
    cadencii::ConcreteSettings settings;
    cadencii::Settings::instance( &settings );

    QApplication a(argc, argv);
    cadencii::WindowMainView wv;
    cadencii::PianorollTrackView pv;
    cadencii::CurveControlChangeView cv;
    cadencii::ConcreteBarCountView bv;
    cadencii::ConcreteTempoView tempoView;
    cadencii::ConcreteTimesigView timesigView;
    cadencii::ConcretePropertyView propertyView;
    cadencii::Controller c;
    c.setMainView( &wv );
    c.setTrackView( &pv );
    c.setControlChangeView( &cv );
    c.setBarCountView( &bv );
    c.setTempoView( &tempoView );
    c.setTimesigView( &timesigView );
    c.setPropertyView( &propertyView );
    wv.show();

    return a.exec();
}
