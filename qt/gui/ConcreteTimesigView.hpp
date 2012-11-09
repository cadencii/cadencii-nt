/**
 * ConcreteTimesigView.hpp
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
#ifndef __ConcreteTimesigView_hpp__
#define __ConcreteTimesigView_hpp__

#include "../../qt/gui/EditorWidgetBase.hpp"
#include "../../gui/TimesigView.hpp"

namespace cadencii {

    class ConcreteTimesigView : public EditorWidgetBase, public TimesigView {
    private:
        QColor backgroundColor;
        QColor lineColor;

    public:
        explicit ConcreteTimesigView(QWidget *parent = 0);

        ~ConcreteTimesigView();

        void setControllerAdapter(ControllerAdapter *controllerAdapter);

        void setDrawOffset(VSQ_NS::tick_t drawOffset);

        void *getWidget();

        void *getScrollEventSender();

        void paintMainContent(QPainter *painter, const QRect &rect);

        void drawMeasureLine(
                QPainter *painter, const QRect &rect, int x,
                const VSQ_NS::MeasureLine &measureLine);

        void paintSubContent(QPainter *painter, const QRect &rect);

        void updateWidget();
    };
}

#endif
