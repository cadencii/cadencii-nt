/**
 * ConcreteTrackListView.hpp
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
#ifndef CADENCII_QT_GUI_CONCRETETRACKLISTVIEW_HPP_
#define CADENCII_QT_GUI_CONCRETETRACKLISTVIEW_HPP_

#include "../../gui/TrackListView.hpp"
#include "EditorWidgetBase.hpp"

namespace cadencii {

    class ConcreteTrackListView : public EditorWidgetBase, public TrackListView {
        Q_OBJECT

    protected:
        int trackIndex;

    private:
        /**
         * @brief トラック一覧のうち、トラック一つ分のタブの最大描画幅
         */
        static const int TRACK_TAB_MAX_WIDTH = 80;

        /**
         * @brief RENDERボタンの幅(px)
         */
        static const int RENDER_BUTTON_WIDTH = 10;

        /**
         * @brief トラックの背景部分の塗りつぶし色。16トラックそれぞれで異なる
         */
        QColor *trackTabHilightBackgroundColor;

        /**
         * @brief トラックをレンダリングするためのボタンの背景色。16トラックそれぞれで異なる
         */
        QColor *trackTabRenderButtonBackgroundColor;

    public:
        explicit ConcreteTrackListView(QWidget *parent = 0);

        ~ConcreteTrackListView();

        void *getWidget() override;

        void setControllerAdapter(ControllerAdapter *controllerAdapter) override;

        void updateWidget() override;

        void *getScrollEventSender() override;

        void paintMainContent(QPainter *painter, const QRect &rect) override;

        void paintSubContent(QPainter *painter, const QRect &rect) override;

        void setTrackIndex(int index);

    public slots:
        void onMousePressSlot(QMouseEvent *event);

    protected:
        /**
         * @brief トラック一覧のうち、トラック一つ分のタブの描画幅を取得する
         */
        int getTrackTabWidth();

    private:
        /**
         * @brief トラック一覧のうち、トラック一つ分のタブを表示する
         */
        void paintTrackTab(
                QPainter *g, const QRect &destRect, const QString &name,
                bool selected, bool enabled, bool render_required,
                const QColor &hilight, const QColor &render_button_hilight);
    };
}

#endif
