/**
 * CurveControlChangeViewContentScroller.cpp
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
#include "gui/CurveControlChangeViewContentScroller.h"
#include "gui/CurveControlChangeView.h"
#include <sstream>

namespace cadencii{
    CurveControlChangeViewContentScroller::CurveControlChangeViewContentScroller( QWidget *parent ) :
        QScrollArea( parent )
    {
        this->controlChangeView = NULL;
    }

    void CurveControlChangeViewContentScroller::setControlChangeView( CurveControlChangeView *pianoroll ){
        this->controlChangeView = pianoroll;
    }

    void CurveControlChangeViewContentScroller::scrollContentsBy( int dx, int dy ){
        QScrollArea::scrollContentsBy( dx, dy );
        if( dx && controlChangeView ){
            controlChangeView->notifyHorizontalScroll();
        }
    }
}
