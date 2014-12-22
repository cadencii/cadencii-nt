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

#include "../../gui/BarCountView.hpp"
#include "../../ControllerAdapter.hpp"
#include "../../qt/gui/EditorWidgetBase.hpp"

namespace cadencii {

    class ConcreteBarCountView : public EditorWidgetBase, public BarCountView {
    private:
        QColor lineColor;

    public:
        explicit ConcreteBarCountView(QWidget *parent = 0);

        ~ConcreteBarCountView();

        void *getScrollEventSender() override;

        void setControllerAdapter(ControllerAdapter *controllerAdapter) override;

        void *getWidget() override;

        void paintMainContent(QPainter *painter, const QRect &rect) override;

        void setDrawOffset(vsq::tick_t drawOffset) override;

        void drawMeasureLine(
                QPainter *painter, const QRect &rect, int x,
                const vsq::MeasureLine &measureLine) override;

        void paintSubContent(QPainter *painter, const QRect &rect) override;

        void updateWidget() override;
    };
}

#endif
