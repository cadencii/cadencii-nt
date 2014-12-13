/**
 * ConcreteSingerListView.cpp
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
#include "ui_EditorWidgetBase.h"
#include "ConcreteSingerListView.hpp"

namespace cadencii {

    ConcreteSingerListView::ConcreteSingerListView(QWidget *parent) :
        EditorWidgetBase(parent) {
        const int height = 21;
        setMinimumHeight(height);
        setMaximumHeight(height);

        trackIndex = 0;
        ui->mainContent->setBackgroundBrush(QBrush(Qt::gray));
        ui->subContent->setBackgroundBrush(QBrush(Qt::gray));
    }

    void *ConcreteSingerListView::getWidget() {
        return this;
    }

    void ConcreteSingerListView::setControllerAdapter(ControllerAdapter *controllerAdapter) {
        this->controllerAdapter = controllerAdapter;
    }

    void ConcreteSingerListView::updateWidget() {
        repaint();
    }

    void *ConcreteSingerListView::getScrollEventSender() {
        return static_cast<SingerListView *>(this);
    }

    void ConcreteSingerListView::setDrawOffset(vsq::tick_t drawOffset) {
        setDrawOffsetInternal(drawOffset);
    }

    void ConcreteSingerListView::paintMainContent(QPainter *painter, const QRect &rect) {
        QRect visibleArea = ui->mainContent->getVisibleArea();
        int width = ui->mainContent->getSceneWidth();
        if (width < this->width()) {
            width = this->width();
        }

        // 背景と境界線.
        static QColor borderColor = QColor::fromRgb(118, 123, 138);
        painter->setPen(borderColor);
        painter->drawLine(visibleArea.left(), 0,
                          visibleArea.left() + this->width(), 0);
        painter->drawLine(visibleArea.left(), 0,
                          visibleArea.left(), this->height());

        vsq::Sequence const* sequence = controllerAdapter->getSequence();
        if (!sequence) {
            return;
        }
        if (trackIndex < 0 || sequence->tracks().size() <= trackIndex) {
            return;
        }

        // コンポーネントの左端位置での、歌手変更イベントを調べる.
        vsq::Track const& track = sequence->track(trackIndex);
        int offset = controllerAdapter->getXFromTick(0);
        int xAtLeft = visibleArea.x() + offset;
        int clockAtLeft = controllerAdapter->getTickFromX(xAtLeft);
        vsq::Event const* singerAtLeft = track.singerEventAt(clockAtLeft);
        if (singerAtLeft) {
            paintSinger(painter, singerAtLeft, xAtLeft, LEFT);
        }

        // 順に描画する
        vsq::EventListIndexIterator i =
                track.getIndexIterator(vsq::EventListIndexIteratorKind::SINGER);
        vsq::Event::List const& events = track.events();
        while (i.hasNext()) {
            int index = i.next();
            vsq::Event const* item = events.get(index);
            int x = controllerAdapter->getXFromTick(item->tick);
            if (x < xAtLeft) {
                continue;
            }
            if (visibleArea.right() < x) {
                break;
            }
            paintSinger(painter, item, x, DEFAULT);
        }
    }

    void ConcreteSingerListView::paintSubContent(QPainter *painter, const QRect &rect) {
        static QColor borderColor(118, 123, 138);
        painter->setPen(borderColor);
        QRect drawRect = ui->subContent->rect();
        painter->drawLine(0, 0, drawRect.width(), 0);

        const int space = 2;
        QRect textRect(drawRect.x() + 2 * space, drawRect.y() + space,
                       drawRect.width() - 4 * space, drawRect.height() - 2 * space);
        static QString text = QString::fromStdString("SINGER");
        static QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        painter->setPen(Qt::black);
        painter->drawText(textRect, text, option);
    }

    void ConcreteSingerListView::paintSinger(
            QPainter *painter, const vsq::Event *singerEvent,
            int x, SingerItemState state) {
        static QColor singerEventBorderColor = QColor::fromRgb(182, 182, 182);
        int height = this->height();

        QRect rc(x, 1, SINGER_ITEM_WIDTH, height - 2);
        switch (state) {
            case LEFT: {
                painter->fillRect(rc, Qt::lightGray);
                break;
            }
            default: {
                painter->fillRect(rc, Qt::white);
                break;
            }
        }
        painter->setPen(singerEventBorderColor);
        painter->drawRect(rc);

        painter->setPen(Qt::black);
        QString text(singerEvent->singerHandle.ids.c_str());
        static QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        QRect textRect(x + 1, 1, SINGER_ITEM_WIDTH - 1, height - 1);
        painter->drawText(textRect, text, option);
    }

    void ConcreteSingerListView::setTrackIndex(int index) {
        trackIndex = index;
        updateWidget();
    }
}
