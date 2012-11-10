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

namespace cadencii {

    EditorWidgetBaseSubContent::EditorWidgetBaseSubContent(QWidget *parent) :
        QGraphicsView(parent) {
        deconstructStarted = false;
        scene = new QGraphicsScene();
        setScene(scene);
        parentWidget = 0;

        setMouseTracking(true);
        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        scene->setSceneRect(0, 0, 0, 0);
    }

    EditorWidgetBaseSubContent::~EditorWidgetBaseSubContent() {
        deconstructStarted = true;
        delete scene;
    }

    void EditorWidgetBaseSubContent::setEditorWidgetBase(EditorWidgetBase *editorWidgetBase) {
        parentWidget = editorWidgetBase;
    }

    void EditorWidgetBaseSubContent::drawForeground(QPainter *painter, const QRectF &rect) {
        if (!parentWidget) return;

        QRect visibleArea(
                static_cast<int>(rect.x()), static_cast<int>(rect.y()),
                static_cast<int>(rect.width()), static_cast<int>(rect.height()));
        parentWidget->paintSubContent(painter, visibleArea);
    }
}
