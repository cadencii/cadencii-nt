/**
 * EditorWidgetBase.cpp
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
#include <sstream>
#include "EditorWidgetBase.hpp"
#include "EditorWidgetBaseMainContent.hpp"
#include "ui_EditorWidgetBase.h"

namespace cadencii {

    EditorWidgetBase::EditorWidgetBase(QWidget *parent) :
        QWidget(parent), controllerAdapter(0) {
        ui = new Ui::EditorWidgetBase;
        ui->setupUi(this);
        ui->mainContent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->mainContent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->mainContent->setBackgroundRole(QPalette::Dark);
        ui->mainContent->setEditorWidgetBase(this);
        ui->subContent->setEditorWidgetBase(this);
    }

    EditorWidgetBase::~EditorWidgetBase() {
        delete ui;
    }

    void EditorWidgetBase::notifyVerticalScroll() {
        QRect rect = ui->mainContent->getVisibleArea();
        ui->subContent->update();
    }

    void EditorWidgetBase::notifyHorizontalScroll() {
        QRect visibleRect = ui->mainContent->getVisibleArea();
        VSQ_NS::tick_t drawOffset
                = (VSQ_NS::tick_t)controllerAdapter->getTickFromX(visibleRect.x());
        controllerAdapter->drawOffsetChanged(getScrollEventSender(), drawOffset);
    }

    void EditorWidgetBase::repaint() {
        ui->mainContent->scene->update();
        ui->mainContent->repaint();
        ui->subContent->repaint();
        QWidget::repaint();
    }

    void EditorWidgetBase::setDrawOffsetInternal(VSQ_NS::tick_t drawOffset) {
        static QMutex mutex;
        if (mutex.tryLock()) {
            int xScrollTo = -controllerAdapter->getXFromTick(drawOffset);
            QScrollBar *scrollBar = ui->mainContent->horizontalScrollBar();
            int maxValue = scrollBar->maximum() + scrollBar->pageStep();
            int minValue = scrollBar->minimum();
            int contentWidth = static_cast<int>(ui->mainContent->getSceneWidth());
            int value = static_cast<int>(minValue
                    + (minValue - maxValue) * static_cast<double>(xScrollTo) / contentWidth);
            if (scrollBar->value() != value) scrollBar->setValue(value);
            mutex.unlock();
        }
    }

    void EditorWidgetBase::drawMeasureLine(
            QPainter *painter, const QRect &rect, int x,
            const VSQ_NS::MeasureLine &measureLine) {
        static QBrush barBrush(QColor::fromRgb(161, 157, 136), Qt::SolidPattern);
        static QBrush beatBrush(QColor::fromRgb(209, 204, 172), Qt::SolidPattern);
        QBrush brush = measureLine.isBorder ? barBrush : beatBrush;
        Qt::PenStyle style = measureLine.isAssistLine ? Qt::DashLine : Qt::SolidLine;
        QPen pen(brush, 1.0, style);
        painter->setPen(pen);
        painter->drawLine(x, rect.top(), x, rect.bottom());
    }

    QSize EditorWidgetBase::getPreferredMainContentSceneSize() {
        int width = controllerAdapter->getPreferredComponentWidth();
        int height = this->height();
        return QSize(width, height);
    }

    QSize EditorWidgetBase::getPreferredSubContentSceneSize() {
        return ui->subContent->size();
    }
}
