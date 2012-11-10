/**
 * EditorWidgetBaseMainContent.cpp
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
#include <math.h>
#include <QScrollArea>
#include <QPainter>
#include <QScrollBar>
#include <sstream>
#include "../../vsq/Timesig.hpp"
#include "EditorWidgetBase.hpp"
#include "EditorWidgetBaseMainContent.hpp"
#include "../../Settings.hpp"

namespace cadencii {

    EditorWidgetBaseMainContent::EditorWidgetBaseMainContent(QWidget *parent) :
        QGraphicsView(parent) {
        deconstructStarted = false;
        scene = new QGraphicsScene();
        setScene(scene);
        parentWidget = 0;

        this->setMouseTracking(true);
        this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        scene->setSceneRect(0, 0, 5000, 10);
    }

    EditorWidgetBaseMainContent::~EditorWidgetBaseMainContent() {
        deconstructStarted = true;
        delete scene;
    }

    QRect EditorWidgetBaseMainContent::getVisibleArea() {
        QRect result;
        QScrollBar *horizontalScroll = horizontalScrollBar();
        QScrollBar *verticalScroll = verticalScrollBar();
        if (parentWidget) {
            QSize preferredSize = parentWidget->getPreferredMainContentSceneSize();
            float preferredHeight = preferredSize.height();
            float preferredWidth = preferredSize.width();
            int x = static_cast<int>((horizontalScroll->value()
                    - horizontalScroll->minimum()) * preferredWidth / (horizontalScroll->maximum()
                    + horizontalScroll->pageStep()
                    - horizontalScroll->minimum()));
            int y = static_cast<int>((verticalScroll->value()
                    - verticalScroll->minimum()) * preferredHeight / (verticalScroll->maximum()
                    + verticalScroll->pageStep()
                    - verticalScroll->minimum()));

            int width = this->width();
            int height = this->height();
            result = QRect(x, y, width, height);
        } else {
            result = QRect(0, 0, horizontalScroll->width(), verticalScroll->height());
        }
        return result;
    }

    void EditorWidgetBaseMainContent::mouseMoveEvent(QMouseEvent *e) {
        this->parentWidget->repaint();
        QWidget::mouseMoveEvent(e);
        emit onMouseMove(e);
    }

    void EditorWidgetBaseMainContent::scrollContentsBy(int dx, int dy) {
        QGraphicsView::scrollContentsBy(dx, dy);
        if (deconstructStarted) return;
        if (dy && parentWidget) {
            parentWidget->notifyVerticalScroll();
        }
        if (dx && parentWidget) {
            parentWidget->notifyHorizontalScroll();
        }
    }

    int EditorWidgetBaseMainContent::getSceneWidth() {
        return static_cast<int>(scene->sceneRect().width());
    }

    int EditorWidgetBaseMainContent::getSceneHeight() {
        return static_cast<int>(scene->sceneRect().height());
    }

    void EditorWidgetBaseMainContent::drawForeground(QPainter *painter, const QRectF &rect) {
        QSize preferedSize = parentWidget->getPreferredMainContentSceneSize();
        scene->setSceneRect(0, 0, preferedSize.width(), preferedSize.height());

        QRect visibleArea(
                static_cast<int>(rect.x()), static_cast<int>(rect.y()),
                static_cast<int>(rect.width()), static_cast<int>(rect.height()));
        parentWidget->paintMainContent(painter, visibleArea);
    }

    void EditorWidgetBaseMainContent::setEditorWidgetBase(EditorWidgetBase *editorWidgetBase) {
        parentWidget = editorWidgetBase;
    }

    void EditorWidgetBaseMainContent::paintMeasureLines(QPainter *g, QRect visibleArea) {
        int left = visibleArea.left();
        int right = visibleArea.right();
        VSQ_NS::tick_t tickAtScreenRight
                = (VSQ_NS::tick_t)parentWidget->controllerAdapter->getTickFromX(right);

        const VSQ_NS::TimesigList *list = 0;
        static VSQ_NS::TimesigList defaultList;
        const VSQ_NS::Sequence *sequence = parentWidget->controllerAdapter->getSequence();
        if (sequence) {
            list = &sequence->timesigList;
        } else {
            list = &defaultList;
        }
        VSQ_NS::MeasureLineIterator i(list);
        QuantizeMode::QuantizeModeEnum mode = Settings::instance()->getQuantizeMode();
        if (Settings::instance()->isGridVisible() && mode != QuantizeMode::NONE) {
            VSQ_NS::tick_t assistLineStep = QuantizeMode::getQuantizeUnitTick(mode);
            i = VSQ_NS::MeasureLineIterator(list, assistLineStep);
        }
        i.reset(tickAtScreenRight);

        while (i.hasNext()) {
            VSQ_NS::MeasureLine line = i.next();
            int x = parentWidget->controllerAdapter->getXFromTick(line.tick);
            if (x < left) {
                continue;
            } else if (right < x) {
                break;
            }
            parentWidget->drawMeasureLine(g, visibleArea, x, line);
        }
    }

    void EditorWidgetBaseMainContent::paintSongPosition(QPainter *g, QRect visibleArea) {
        VSQ_NS::tick_t songPosition = parentWidget->controllerAdapter->getSongPosition();
        int x = parentWidget->controllerAdapter->getXFromTick(songPosition);
        g->setPen(QColor(0, 0, 0));
        g->drawLine(x, visibleArea.top(), x, visibleArea.bottom());
        g->setPen(QColor(0, 0, 0, 40));
        g->drawLine(x - 1, visibleArea.top(), x - 1, visibleArea.bottom());
        g->drawLine(x + 1, visibleArea.top(), x + 1, visibleArea.bottom());
    }

    void EditorWidgetBaseMainContent::mousePressEvent(QMouseEvent *event) {
        emit onMousePress(event);
    }

    void EditorWidgetBaseMainContent::mouseReleaseEvent(QMouseEvent *event) {
        emit onMouseRelease(event);
    }

    void EditorWidgetBaseMainContent::mouseDoubleClickEvent(QMouseEvent *event) {
        emit onMouseDoubleClick(event);
    }
}
