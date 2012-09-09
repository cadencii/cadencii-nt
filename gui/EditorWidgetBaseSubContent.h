/**
 * EditorWidgetBaseSubContent.h
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
#ifndef __EditorWidgetBaseSubContent_h__
#define __EditorWidgetBaseSubContent_h__

#include "gui/EditorWidgetBase.h"
#include <QWidget>

namespace cadencii{

    class EditorWidgetBaseSubContent : public QWidget{
        Q_OBJECT

    private:
        EditorWidgetBase *parentWidget;

    public:
        EditorWidgetBaseSubContent( QWidget *parent = 0 );

        ~EditorWidgetBaseSubContent();

        void paintEvent( QPaintEvent *event );

        void setEditorWidgetBase( EditorWidgetBase *editorWidgetBase );
    };

}

#endif
