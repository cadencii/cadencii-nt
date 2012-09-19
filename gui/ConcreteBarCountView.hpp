/**
 * ConcreteBarCountView.h
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
#ifndef __ConcreteBarCountView_hpp__
#define __ConcreteBarCountView_hpp__

#include "BarCountView.hpp"
#include "ControllerAdapter.hpp"
#include "vsq/Sequence.hpp"

namespace cadencii{

    class ConcreteBarCountView : public BarCountView{
    private:
        VSQ_NS::Sequence *sequence;
        ControllerAdapter *controllerAdapter;

    public:
        ConcreteBarCountView();

        void *getScrollEventSender();

        QSizeF getPreferedSceneSize();

        void setSequence( VSQ_NS::Sequence *sequence );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void *getWidget();

        void paintMainContent( QPainter *painter, const QRect &rect );

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        void drawMeasureLine( QPainter *painter, const QRect &rect, int x, const VSQ_NS::MeasureLine &measureLine );
    };

}

#endif
