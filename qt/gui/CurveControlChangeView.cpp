/**
 * CurveControlChangeView.cpp
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
#include <QScrollBar>
#include <QColor>
#include <QMouseEvent>
#include <algorithm>
#include <string>
#include <vector>
#include "ui_EditorWidgetBase.h"
#include "CurveControlChangeView.hpp"
#include "../../vsq/Track.hpp"

namespace cadencii {

    CurveControlChangeView::CurveControlChangeView(QWidget *parent) :
        EditorWidgetBase(parent) {
        trackIndex = 0;
        controlChangeName = "dyn";
        front = defaultSequence.track[0].getCurve(controlChangeName);
        ui->mainContent->setBackgroundBrush(QBrush(Qt::darkGray));
        ui->subContent->setBackgroundBrush(QBrush(Qt::lightGray));
        ui->subContent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->subContent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        borderColor = QColor::fromRgb(118, 123, 138);

        connect(ui->subContent, SIGNAL(onMousePress(QMouseEvent *)),
                this, SLOT(onSubContentMousePressSlot(QMouseEvent *)));
    }

    CurveControlChangeView::~CurveControlChangeView() {
    }

    void *CurveControlChangeView::getWidget() {
        return static_cast<void *>(this);
    }

    void CurveControlChangeView::setDrawOffset(VSQ_NS::tick_t drawOffset) {
        setDrawOffsetInternal(drawOffset);
    }

    void CurveControlChangeView::setControllerAdapter(ControllerAdapter *controllerAdapter) {
        this->controllerAdapter = controllerAdapter;
    }

    void *CurveControlChangeView::getScrollEventSender() {
        return static_cast<ControlChangeView *>(this);
    }

    void CurveControlChangeView::paintMainContent(QPainter *painter, const QRect &rect) {
        // グラフ部分の最大値、最小値の位置の線を描く
        static QColor lineGraphBottom(156, 161, 169);
        static QColor lineGraphTop(46, 47, 50);
        painter->setPen(lineGraphBottom);
        painter->drawLine(rect.left(), rect.bottom() - MARGIN_BOTTOM,
                           rect.right(), rect.bottom() - MARGIN_BOTTOM);
        painter->setPen(lineGraphTop);
        painter->drawLine(rect.left(),  MARGIN_TOP,
                           rect.right(), MARGIN_TOP);

        // グラフ部分の本体を描く
        ui->mainContent->paintMeasureLines(painter, rect);
        if (front) {
            paintBPList(painter, front, rect);
        }
        ui->mainContent->paintSongPosition(painter, rect);
    }

    void CurveControlChangeView::paintSubContent(QPainter *painter, const QRect &rect) {
        if (!controllerAdapter) return;

        static QColor borderColor(41, 46, 55);
        static QColor normalTextColor(0, 0, 0);
        static QColor hilightTextColor(255, 255, 255);
        static QColor hilightBackgroundColor(108, 108, 108);

        const std::vector<std::string> *controlChangeNameList
                = getCurrentCurveNameList();

        static QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        const int boxPadding = 2;

        int count = controlChangeNameList->size();
        for (int i = 0; i < count; i++) {
            std::string name = controlChangeNameList->at(i);
            QRectF borderRect = getCurveNameRect(i);
            QRectF textRect(borderRect.x() + boxPadding,
                            borderRect.y() + boxPadding,
                            borderRect.width() - 2 * boxPadding,
                            borderRect.height() - 2 * boxPadding);
            bool isSelected = StringUtil::toLower(name) == StringUtil::toLower(controlChangeName);
            if (isSelected) {
                painter->fillRect(borderRect, hilightBackgroundColor);
            }
            painter->setPen(borderColor);
            painter->drawRect(borderRect);
            painter->setPen(isSelected ? hilightTextColor : normalTextColor);
            painter->drawText(textRect, QString::fromStdString(name), option);
        }
    }

    void CurveControlChangeView::setTrackIndex(int index) {
        trackIndex = index;
        if (front) {
            std::string name = front->getName();
            setControlChangeName(name);
        }
        updateWidget();
    }

    void CurveControlChangeView::setControlChangeName(const std::string &name) {
        controlChangeName = name;
        if (controllerAdapter) {
            const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
            if (0 <= trackIndex && trackIndex < sequence->track.size()) {
                front = sequence->track[trackIndex].getConstCurve(controlChangeName);
            } else {
                front = 0;
            }
        } else {
            front = 0;
        }
    }

    // TODO(kbinani): 効率よく描画するようリファクタする
    void CurveControlChangeView::paintBPList(
            QPainter *painter, const VSQ_NS::BPList *list, const QRect &rect) {
        int max = list->getMaximum();
        int min = list->getMinimum();
        int height = this->height();
        int y = getYFromValue(max, min, list->getDefault());

        QPainterPath path;
        path.moveTo(-1, height - MARGIN_BOTTOM);
        path.lineTo(-1, y);

        int size = list->size();
        for (int i = 0; i < size; i++) {
            VSQ_NS::BP point = list->get(i);
            VSQ_NS::tick_t clock = list->getKeyClock(i);
            int x = controllerAdapter->getXFromTick(clock);
            path.lineTo(x, y);
            y = getYFromValue(max, min, point.value);
            path.lineTo(x, y);
        }

        // スクリーンのサイズが、コンポーネントのサイズよりも小さい場合を考慮し、
        // 大きい方を左端の座標とする。
        int width = std::max(ui->mainContent->getSceneWidth(), ui->mainContent->width());
        path.lineTo(width, y);
        path.lineTo(width, height - MARGIN_BOTTOM);
        painter->fillPath(path, QColor(100, 149, 237, 150));

        painter->setPen(QColor(255, 255, 255));
        painter->drawPath(path);

        // カーソルが描画範囲に入っていれば、カーソル位置での値を描く。
        QPoint globalCursorPos = QCursor::pos();
        QPoint globalTopLeftCornerPos = ui->mainContent->mapToGlobal(QPoint(0, 0));
        QPoint viewportCursorPos = QPoint(globalCursorPos.x() - globalTopLeftCornerPos.x(),
                                          globalCursorPos.y() - globalTopLeftCornerPos.y());
        QPoint sceneCursorPos = ui->mainContent->mapToScene(viewportCursorPos).toPoint();
        if (MARGIN_TOP <= sceneCursorPos.y() &&
                sceneCursorPos.y() <= height - MARGIN_BOTTOM &&
                rect.contains(sceneCursorPos)) {
            static QTextOption textOption(Qt::AlignRight | Qt::AlignBottom);
            int value = getValueFromY(max, min, sceneCursorPos.y());
            painter->drawText(QRectF(sceneCursorPos.x() - 100, sceneCursorPos.y() - 100, 100, 100),
                              QString(StringUtil::toString(value).c_str()),
                              textOption);
        }
    }

    void CurveControlChangeView::drawMeasureLine(
            QPainter *painter, const QRect &rect, int x, const VSQ_NS::MeasureLine &measureLine) {
        static QColor white100(0, 0, 0, 100);
        static QColor pen(12, 12, 12);
        if (measureLine.isBorder) {
            painter->setPen(white100);
            painter->drawLine(x, rect.height() - MARGIN_BOTTOM - 1, x, MARGIN_TOP + 1);
        } else {
            int graphHeight = rect.height() - MARGIN_BOTTOM - MARGIN_TOP;
            int center = rect.top() + MARGIN_TOP + graphHeight / 2;
            int scaleLineHeight = measureLine.isAssistLine ? 3 : 6;

            painter->setPen(white100);
            painter->drawLine(x, center - scaleLineHeight - 1, x, center + scaleLineHeight);
            if (!measureLine.isAssistLine) {
                painter->setPen(pen);
            }
            painter->drawLine(x, MARGIN_TOP, x, MARGIN_TOP + scaleLineHeight);
            painter->drawLine(x, rect.height() - MARGIN_BOTTOM,
                              x, rect.height() - MARGIN_BOTTOM - scaleLineHeight);
        }
    }

    int CurveControlChangeView::getYFromValue(int max, int min, int value) {
        int height = this->height();
        return static_cast<int>(height - MARGIN_BOTTOM
                - ((value - min) / static_cast<double>(max - min)
                   * (height - MARGIN_BOTTOM - MARGIN_TOP)));
    }

    int CurveControlChangeView::getValueFromY(int max, int min, int y) {
        int height = this->height();
        return (height - MARGIN_BOTTOM - y) * (max - min) / (height - MARGIN_BOTTOM - MARGIN_TOP)
                + min;
    }

    void CurveControlChangeView::updateWidget() {
        repaint();
    }

    QSize CurveControlChangeView::getPreferredSubContentSceneSize() {
        const std::vector<std::string> *controlChangeNameList = getCurrentCurveNameList();
        int width = ui->subContent->width();
        int height = 2 * CURVE_NAME_MARGIN
                + (CURVE_NAME_HEIGHT + CURVE_NAME_SPACE) * controlChangeNameList->size();
        return QSize(width, height);
    }

    QRectF CurveControlChangeView::getCurveNameRect(int index) {
        return QRectF(CURVE_NAME_MARGIN,
                      CURVE_NAME_MARGIN + index * (CURVE_NAME_HEIGHT + CURVE_NAME_SPACE),
                      ui->subContent->width() - 2 * CURVE_NAME_MARGIN,
                      CURVE_NAME_HEIGHT);
    }

    const std::vector<std::string> *CurveControlChangeView::getCurrentCurveNameList() {
        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        const VSQ_NS::Track *track = &sequence->track[trackIndex];
        return track->curveNameList();
    }

    void CurveControlChangeView::onSubContentMousePressSlot(QMouseEvent *event) {
        QPointF point = ui->subContent->mapToScene(event->pos());
        const std::vector<std::string> *curveNameList = getCurrentCurveNameList();
        int count = curveNameList->size();
        for (int i = 0; i < count; i++) {
            QRectF rect = getCurveNameRect(i);
            if (rect.contains(point)) {
                std::string name = curveNameList->at(i);
                setControlChangeName(name);
                updateWidget();
                break;
            }
        }
    }
}
