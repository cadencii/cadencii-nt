/**
 * EditorWidgetBaseSubContent.cpp
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
#include "EditorWidgetBaseSubContent.hpp"
#include "ui_EditorWidgetBase.h"

namespace cadencii{

    EditorWidgetBaseSubContent::EditorWidgetBaseSubContent( QWidget *parent ) :
        QWidget( parent )
    {
        parentWidget = 0;
    }

    EditorWidgetBaseSubContent::~EditorWidgetBaseSubContent(){
    }

    void EditorWidgetBaseSubContent::setEditorWidgetBase( EditorWidgetBase *editorWidgetBase ){
        parentWidget = editorWidgetBase;
    }

    void EditorWidgetBaseSubContent::paintEvent( QPaintEvent *event ){
        QWidget::paintEvent( event );
        if( parentWidget ){
            QPainter p( this );
            parentWidget->paintSubContent( &p, geometry() );
        }
    }

}
