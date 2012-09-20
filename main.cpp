/**
 * main.cpp
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <QtGui/QApplication>
#include "gui/WindowMainView.hpp"
#include "gui/PianorollTrackView.hpp"
#include "gui/CurveControlChangeView.hpp"
#include "gui/ConcreteBarCountView.hpp"
#include "gui/ConcreteTempoView.hpp"
#include "gui/ConcreteTimesigView.hpp"
#include "Controller.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cadencii::WindowMainView wv;
    cadencii::PianorollTrackView pv;
    cadencii::CurveControlChangeView cv;
    cadencii::ConcreteBarCountView bv;
    cadencii::ConcreteTempoView tempoView;
    cadencii::ConcreteTimesigView timesigView;
    cadencii::Controller c;
    c.setMainView( &wv );
    c.setTrackView( &pv );
    c.setControlChangeView( &cv );
    c.setBarCountView( &bv );
    c.setTempoView( &tempoView );
    c.setTimesigView( &timesigView );
    wv.show();

    return a.exec();
}
