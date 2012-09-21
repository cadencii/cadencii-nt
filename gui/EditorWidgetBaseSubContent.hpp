/**
 * EditorWidgetBaseSubContent.hpp
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
#ifndef __EditorWidgetBaseSubContent_hpp__
#define __EditorWidgetBaseSubContent_hpp__

#include "gui/EditorWidgetBase.hpp"
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
