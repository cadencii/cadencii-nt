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

namespace cadencii {

    class ConcreteTempoView : public EditorWidgetBase, public TempoView {
    private:
        QColor backgroundColor;
        QColor lineColor;

    public:
        explicit ConcreteTempoView(QWidget *parent = 0);

        ~ConcreteTempoView();

        void *getScrollEventSender() override;

        void *getWidget() override;

        void setControllerAdapter(ControllerAdapter *controllerAdapter) override;

        void setDrawOffset(vsq::tick_t drawOffset) override;

        void paintMainContent(QPainter *painter, const QRect &rect) override;

        void paintSubContent(QPainter *painter, const QRect &rect) override;

        void drawMeasureLine(
                QPainter *painter, const QRect &rect, int x,
                const vsq::MeasureLine &measureLine) override;

        void updateWidget() override;
    };
}

#endif
