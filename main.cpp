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
#include "gui/WindowMainView.h"
#include "gui/PianorollTrackView.h"
#include "Controller.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cadencii::WindowMainView w;
    cadencii::PianorollTrackView p;
    cadencii::Controller c;
    c.setMainView( &w );
    c.setTrackView( &p );
    w.show();
    
    return a.exec();
}
