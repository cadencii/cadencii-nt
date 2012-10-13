/**
 * ConcreteTempoView.hpp
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
#ifndef __ConcreteTempoView_hpp__
#define __ConcreteTempoView_hpp__

#include "../../gui/TempoView.hpp"
#include "../../qt/gui/EditorWidgetBase.hpp"

namespace cadencii{

    class ConcreteTempoView : public EditorWidgetBase, public TempoView{
    private:
        QColor backgroundColor;
        QColor lineColor;

    public:
        ConcreteTempoView( QWidget *parent = 0 );

        ~ConcreteTempoView();

        void *getScrollEventSender();

        void *getWidget();

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        void paintMainContent( QPainter *painter, const QRect &rect );

        void paintSubContent( QPainter *painter, const QRect &rect );

        void drawMeasureLine( QPainter *painter, const QRect &rect, int x, const VSQ_NS::MeasureLine &measureLine );

        void updateWidget();
    };

}

#endif
