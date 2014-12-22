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

#include <QWidget>
#include "EditorWidgetBase.hpp"

namespace cadencii {

    class EditorWidgetBaseSubContent : public QGraphicsView {
        friend class EditorWidgetBase;

        Q_OBJECT

    protected:
        QGraphicsScene *scene;

    private:
        EditorWidgetBase *parentWidget;

        bool deconstructStarted;

    public:
        explicit EditorWidgetBaseSubContent(QWidget *parent = 0);

        ~EditorWidgetBaseSubContent();

        void drawForeground(QPainter *painter, const QRectF &rect) override;

        void setEditorWidgetBase(EditorWidgetBase *editorWidgetBase);

        void mousePressEvent(QMouseEvent *event) override;

    signals:
        /**
         * @brief Emitted when mouse pressed.
         * @param event An event, mouse position is based on scene coordinate.
         */
        void onMousePress(QMouseEvent *event);
    };
}

#endif
