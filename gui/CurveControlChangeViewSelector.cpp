/**
 * CurveControlChangeViewSelector.cpp
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
#include <QPainter>
#include <sstream>
#include "gui/CurveControlChangeView.h"
#include "gui/CurveControlChangeViewContent.h"
#include "gui/CurveControlChangeViewSelector.h"

namespace cadencii{
    using namespace std;

    CurveControlChangeViewSelector::CurveControlChangeViewSelector( QWidget *parent ) :
        QWidget( parent )
    {
    }

    CurveControlChangeViewSelector::~CurveControlChangeViewSelector(){
    }

    void CurveControlChangeViewSelector::paintEvent( QPaintEvent *e ){
        QPainter p( this );
        //TODO:描画処理の実装がまだ
    }

    void CurveControlChangeViewSelector::setControlChangeView( CurveControlChangeView *pianoroll ){
        this->controlChangeView = pianoroll;
    }

}
