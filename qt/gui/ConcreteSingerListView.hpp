/**
 * ConcreteSingerListView.hpp
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
#ifndef CADENCII_QT_GUI_CONCRETESINGERLISTVIEW_HPP_
#define CADENCII_QT_GUI_CONCRETESINGERLISTVIEW_HPP_

#include "../../gui/SingerListView.hpp"
#include "EditorWidgetBase.hpp"

namespace cadencii {

    class ConcreteSingerListView : public EditorWidgetBase, public SingerListView {
        Q_OBJECT

    private:
        /**
         * @brief 歌手変更イベントの表示状態を表す
         */
        enum SingerItemState {
            /**
             * @brief デフォルト
             */
            DEFAULT,
            /**
             * @brief 左端に、薄い色で表示された状態
             */
            LEFT
        };

    private:
        /**
         * @brief 歌手変更イベントの表示矩形の幅
         */
        static const int SINGER_ITEM_WIDTH = 66;

        int trackIndex;

    public:
        explicit ConcreteSingerListView(QWidget *parent = 0);

        void *getWidget();

        void setControllerAdapter(ControllerAdapter *controllerAdapter);

        void updateWidget();

        void *getScrollEventSender();

        void setDrawOffset(vsq::tick_t drawOffset);

        void paintMainContent(QPainter *painter, const QRect &rect);

        void paintSubContent(QPainter *painter, const QRect &rect);

        void setTrackIndex(int index);

    private:
        /**
         * @brief 歌手の一覧を描画する
         */
        void paintSingerList(QPainter *painter);

        /**
         * @brief 歌手変更イベントを指定された位置に描画する
         */
        void paintSinger(
                QPainter *painter, const vsq::Event *singerEvent,
                int x, SingerItemState state);
    };
}

#endif
