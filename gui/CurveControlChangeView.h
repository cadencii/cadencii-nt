/**
 * CurveControlChangeView.h
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
#ifndef __CurveControlChangeView_h__
#define __CurveControlChangeView_h__

#include "EditorWidgetBase.h"
#include "../ControlChangeView.hpp"
#include <QGraphicsScene>

namespace cadencii{

    class CurveControlChangeView : public EditorWidgetBase, public ControlChangeView{
    private:
        VSQ_NS::Sequence *sequence;
        int trackIndex;

    public:
        CurveControlChangeView( QWidget *parent = 0 );

        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void setSequence( VSQ_NS::Sequence *sequence );

        void *getWidget();

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void paintMainContent( QPainter *painter, const QRect &rect );

        void *getScrollEventSender();

        QSizeF getPreferedSceneSize();

        void setTrackIndex( int index );
    };

}

#endif
