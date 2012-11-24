/**
 * ConcreteTrackListView.cpp
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
#include <QMouseEvent>
#include <string>
#include "ui_EditorWidgetBase.h"
#include "ConcreteTrackListView.hpp"

namespace cadencii {

    ConcreteTrackListView::ConcreteTrackListView(QWidget *parent) :
        EditorWidgetBase(parent) {
        const int height = 21;
        setMinimumHeight(height);
        setMaximumHeight(height);

        trackIndex = 0;
        ui->mainContent->setBackgroundBrush(QBrush(Qt::gray));
        ui->subContent->setBackgroundBrush(QBrush(Qt::gray));

        trackTabHilightBackgroundColor = new QColor[16];
        trackTabHilightBackgroundColor[0] = QColor(181, 220, 16);
        trackTabHilightBackgroundColor[1] = QColor(231, 244, 49);
        trackTabHilightBackgroundColor[2] = QColor(252, 230, 29);
        trackTabHilightBackgroundColor[3] = QColor(247, 171, 20);
        trackTabHilightBackgroundColor[4] = QColor(249, 94, 17);
        trackTabHilightBackgroundColor[5] = QColor(234, 27, 47);
        trackTabHilightBackgroundColor[6] = QColor(175, 20, 80);
        trackTabHilightBackgroundColor[7] = QColor(183, 24, 149);
        trackTabHilightBackgroundColor[8] = QColor(105, 22, 158);
        trackTabHilightBackgroundColor[9] = QColor(22, 36, 163);
        trackTabHilightBackgroundColor[10] = QColor(37, 121, 204);
        trackTabHilightBackgroundColor[11] = QColor(29, 179, 219);
        trackTabHilightBackgroundColor[12] = QColor(24, 239, 239);
        trackTabHilightBackgroundColor[13] = QColor(25, 206, 175);
        trackTabHilightBackgroundColor[14] = QColor(23, 160, 134);
        trackTabHilightBackgroundColor[15] = QColor(79, 181, 21);
        trackTabRenderButtonBackgroundColor = new QColor[16];
        trackTabRenderButtonBackgroundColor[0] = QColor(19, 143, 52);
        trackTabRenderButtonBackgroundColor[1] = QColor(158, 154, 18);
        trackTabRenderButtonBackgroundColor[2] = QColor(160, 143, 23);
        trackTabRenderButtonBackgroundColor[3] = QColor(145, 98, 15);
        trackTabRenderButtonBackgroundColor[4] = QColor(142, 52, 12);
        trackTabRenderButtonBackgroundColor[5] = QColor(142, 19, 37);
        trackTabRenderButtonBackgroundColor[6] = QColor(96, 13, 47);
        trackTabRenderButtonBackgroundColor[7] = QColor(117, 17, 98);
        trackTabRenderButtonBackgroundColor[8] = QColor(62, 15, 99);
        trackTabRenderButtonBackgroundColor[9] = QColor(13, 23, 84);
        trackTabRenderButtonBackgroundColor[10] = QColor(25, 84, 132);
        trackTabRenderButtonBackgroundColor[11] = QColor(20, 119, 142);
        trackTabRenderButtonBackgroundColor[12] = QColor(19, 142, 139);
        trackTabRenderButtonBackgroundColor[13] = QColor(17, 122, 102);
        trackTabRenderButtonBackgroundColor[14] = QColor(13, 86, 72);
        trackTabRenderButtonBackgroundColor[15] = QColor(43, 91, 12);

        connect(ui->mainContent, SIGNAL(onMousePress(QMouseEvent*)),
                this, SLOT(onMousePressSlot(QMouseEvent*)));
    }

    ConcreteTrackListView::~ConcreteTrackListView() {
        delete [] trackTabHilightBackgroundColor;
        delete [] trackTabRenderButtonBackgroundColor;
    }

    void *ConcreteTrackListView::getWidget() {
        return this;
    }

    void ConcreteTrackListView::setControllerAdapter(ControllerAdapter *controllerAdapter) {
        this->controllerAdapter = controllerAdapter;
    }

    void ConcreteTrackListView::updateWidget() {
        repaint();
    }

    void *ConcreteTrackListView::getScrollEventSender() {
        return static_cast<TrackListView *>(this);
    }

    void ConcreteTrackListView::paintMainContent(QPainter *painter, const QRect &rect) {
        static QColor borderColor(118, 123, 138);

        painter->setPen(borderColor);
        int height = ui->mainContent->getSceneHeight();
        int width = ui->mainContent->getSceneWidth();
        if (width < this->width()) {
            width = this->width();
        }
        int laneHeight = this->height();
        QRect visibleArea = ui->mainContent->getVisibleArea();
        painter->drawLine(0, 0, width, 0);
        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        if (sequence) {
            int selector_width = getTrackTabWidth();
            for (int i = 0; i < 16; i++) {
                int x = i * selector_width + visibleArea.left();
                std::string name = (i < sequence->tracks()->size())
                        ? StringUtil::toString(i + 1) + " " + sequence->track(i)->getName()
                        : "";
                paintTrackTab(painter,
                              QRect(x, height - laneHeight + 1, selector_width, laneHeight - 1),
                              QString(name.c_str()),
                              (i == trackIndex) ? true : false,
                              true/*TODO:vsq_track.getCommon().PlayMode >= 0*/,
                              false/*TODO:AppManager.getRenderRequired(i + 1)*/,
                              trackTabHilightBackgroundColor[i],
                              trackTabRenderButtonBackgroundColor[i]);
            }
        }
    }

    void ConcreteTrackListView::paintSubContent(QPainter *painter, const QRect &rect) {
        static QColor borderColor(118, 123, 138);
        painter->setPen(borderColor);
        QRect drawRect = ui->subContent->rect();
        painter->drawLine(0, 0, drawRect.width(), 0);

        const int space = 2;
        QRect textRect(drawRect.x() + 2 * space, drawRect.y() + space,
                       drawRect.width() - 4 * space, drawRect.height() - 2 * space);
        static QString text = QString::fromStdString("TRACK");
        static QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        painter->setPen(Qt::black);
        painter->drawText(textRect, text, option);
    }

    /**
     * @brief トラック選択部分の、トラック1個分の幅を調べます。pixel
     */
    int ConcreteTrackListView::getTrackTabWidth() {
        int draft = TRACK_TAB_MAX_WIDTH;
        // トラックの一覧を表示するのに利用できる最大の描画幅
        int maxTotalWidth = width();
        int numTrack = 1;
        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        if (sequence) {
            numTrack = sequence->tracks()->size();
        }
        if (draft * (numTrack - 1) <= maxTotalWidth) {
            return draft;
        } else {
            return static_cast<int>((maxTotalWidth) / (numTrack - 1.0f));
        }
    }

    void ConcreteTrackListView::setTrackIndex(int index) {
        trackIndex = index;
        updateWidget();
    }

    void ConcreteTrackListView::paintTrackTab(
            QPainter *painter, const QRect &destRect, const QString &name,
            bool selected, bool enabled, bool render_required,
            const QColor &hilight, const QColor &render_button_hilight) {
        QColor panel_color = enabled ? hilight : QColor(125, 123, 124);
        QColor panel_title = QColor::fromRgb(0, 0, 0);
        QColor button_title = selected ? QColor::fromRgb(255, 255, 255) : QColor::fromRgb(0, 0, 0);
        QColor border = selected ? QColor::fromRgb(255, 255, 255) : QColor(118, 123, 138);

        // 背景(選択されている場合)
        if (selected) {
            painter->fillRect(destRect, panel_color);
            if (render_required && enabled) {
                painter->fillRect(destRect.right() - 10, destRect.top(),
                                  10, destRect.height(), render_button_hilight);
            }
        }

        // 左縦線
        painter->setPen(border);
        painter->drawLine(destRect.left(), destRect.top(),
                          destRect.left(), destRect.bottom());
        if (0 < name.length()) {
            // 上横線
            painter->setPen(border);
            painter->drawLine(destRect.left() + 1, destRect.top(),
                              destRect.right(), destRect.top());
        }
        if (render_required) {
            painter->setPen(border);
            painter->drawLine(destRect.right() - 10, destRect.top(),
                              destRect.right() - 10, destRect.bottom());
        }

        painter->setClipRect(destRect);
        painter->setPen(panel_title);
        QRectF textRect(destRect.left() + 2, destRect.top(),
                        destRect.width() - 2, destRect.height());
        QTextOption textOption(Qt::AlignLeft | Qt::AlignVCenter);
        painter->drawText(textRect, name, textOption);
        if (render_required) {
            painter->setPen(button_title);
            QRect renderMarkRect(destRect.right() - RENDER_BUTTON_WIDTH, destRect.top(),
                                  RENDER_BUTTON_WIDTH, destRect.height());
            painter->drawText(renderMarkRect, QString("R"), textOption);
        }
        if (selected) {
            painter->setPen(border);
            painter->drawLine(destRect.right(), destRect.top(),
                              destRect.right(), destRect.bottom());
            painter->setPen(border);
            painter->drawLine(destRect.left(), destRect.bottom(),
                              destRect.right(), destRect.bottom());
        }
        painter->setClipRect(QRect(), Qt::NoClip);
    }

    void ConcreteTrackListView::onMousePressSlot(QMouseEvent *event) {
        QPoint mousePos = event->pos();
        int trackTabWidth = getTrackTabWidth();
        int trackIndex = mousePos.x() / trackTabWidth;
        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        if (sequence) {
            if (trackIndex != this->trackIndex &&
                    0 <= trackIndex &&
                    trackIndex < sequence->tracks()->size()) {
                controllerAdapter->setTrackIndex(0, trackIndex);
            }
        }
    }
}
