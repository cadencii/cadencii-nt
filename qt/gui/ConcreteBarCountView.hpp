/**
 * ConcreteBarCountView.hpp
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
#ifndef __ConcreteBarCountView_hpp__
#define __ConcreteBarCountView_hpp__

#include "gui/BarCountView.hpp"
#include "ControllerAdapter.hpp"
#include "qt/gui/EditorWidgetBase.hpp"

namespace cadencii{

    class ConcreteBarCountView : public EditorWidgetBase, public BarCountView{
    private:
        QColor lineColor;

    public:
        ConcreteBarCountView( QWidget *parent = 0 );

        ~ConcreteBarCountView();

        void *getScrollEventSender();

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void *getWidget();

        void paintMainContent( QPainter *painter, const QRect &rect );

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        void drawMeasureLine( QPainter *painter, const QRect &rect, int x, const VSQ_NS::MeasureLine &measureLine );

        void paintSubContent( QPainter *painter, const QRect &rect );

        void updateWidget();
    };

}

#endif
