/**
 * EditorWidgetBase.hpp
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
#ifndef __EditorWidgetBase_hpp__
#define __EditorWidgetBase_hpp__

#include <QScrollArea>
#include <QMutex>
#include <map>
#include "../../vsq/MeasureLineIterator.hpp"
#include "../../vsq/TimesigList.hpp"
#include "../../vsq/Sequence.hpp"
#include "../../qt/gui/EditorWidgetBaseMainContent.hpp"
#include "../../gui/TrackView.hpp"

namespace Ui {
    class EditorWidgetBase;
}

namespace cadencii {
    class EditorWidgetBase : public QWidget {
        friend class EditorWidgetBaseMainContent;
        friend class EditorWidgetBaseSubContent;

        Q_OBJECT

    public:
        Ui::EditorWidgetBase *ui;

    protected:
        ControllerAdapter *controllerAdapter;

    public:
        explicit EditorWidgetBase(QWidget *parent = 0);

        ~EditorWidgetBase();

        /**
         * @brief オーバーライドする。ピアノロール本体と、鍵盤部分を repaint する処理を追加している。
         */
        void repaint();

        /**
         * @brief ソングポジションの移動に伴って自動スクロールするかどうかを取得する
         * @return 自動スクロールする場合は true を返す
         */
        bool isAutoScroll();

        void setDrawOffsetInternal(VSQ_NS::tick_t drawOffset);

    protected:
        virtual void paintSubContent(QPainter *painter, const QRect &rect) {
        }

        /**
         * @brief メインペインを描画する
         */
        virtual void paintMainContent(QPainter *painter, const QRect &rect) {
        }

        /**
         * @brief スクロールを ControllerAdapter に通知する際の、通知元のとして渡すウィジェットのポインタを取得する
         */
        virtual void *getScrollEventSender() = 0;

        /**
         * @brief シーンの適切なサイズを取得する
         */
        virtual QSize getPreferedSceneSize();

        /**
         * @brief スクロール領域が縦方向にスクロールしたことを PianorollContentScroller -> Pianoroll に通知する
         */
        void notifyVerticalScroll();

        /**
         * @brief スクロール領域が横方向にスクロールしたことを PianorollContentScroller -> Pianoroll に通知する
         */
        void notifyHorizontalScroll();

        /**
         * @brief 小節、拍子ごとの線を描画する。オーバーライドすることで、線の描画ロジックを変更できる。
         * @param painter 描画に使用するペインター
         * @param rect 描画対象の領域(=可視領域)
         * @param x 小節、拍子の x 座標
         * @param measureLine 小節・拍子線の情報を格納した MeasureLine のインスタンスが指定される
         */
        virtual void drawMeasureLine(
                QPainter *painter, const QRect &rect, int x,
                const VSQ_NS::MeasureLine &measureLine);
    };
}
#endif
