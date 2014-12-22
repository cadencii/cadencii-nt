/**
 * EditorWidgetBaseMainContent.hpp
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
#ifndef __EditorWidgetBaseMainContent_hpp__
#define __EditorWidgetBaseMainContent_hpp__

#include <QWidget>
#include <QMutex>
#include <QGraphicsView>
#include <map>
#include "../../ControllerAdapter.hpp"
#include <libvsq/libvsq.h>

namespace cadencii {

    class EditorWidgetBase;

    class EditorWidgetBaseMainContent : public QGraphicsView {
        friend class EditorWidgetBase;

        Q_OBJECT

    protected:
        QGraphicsScene *scene;

    private:
        EditorWidgetBase *parentWidget;

        bool deconstructStarted;

    public:
        explicit EditorWidgetBaseMainContent(QWidget *parent = 0);

        ~EditorWidgetBaseMainContent();

        void setEditorWidgetBase(EditorWidgetBase *editorWidgetBase);

        void mousePressEvent(QMouseEvent *event) override;

        /**
         * オーバーライドする。再描画処理が追加される
         */
        void mouseMoveEvent(QMouseEvent *e) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void drawForeground(QPainter *painter, const QRectF &rect) override;

        /**
         * スクロールされた結果、可視状態となっている領域を取得する
         */
        QRect getVisibleArea();

        /**
         * @brief ソングポジションを設定する
         */
        void setSongPosition(vsq::tick_t songPosition);

        /**
         * @brief ソングポジションを取得する
         */
        vsq::tick_t getSongPosition();

        /**
         * @brief ミューテックスを設定する
         * @param mutex ミューテックス
         */
        void setMutex(QMutex *mutex);

        void scrollContentsBy(int dx, int dy) override;

        /**
         * @brief 仮想スクリーンの幅を取得する
         * @return 仮想スクリーンの幅(pixel)
         */
        int getSceneWidth();

        /**
         * @brief 仮想スクリーンの高さを取得する
         * @return 仮想スクリーンの高さ(pixel)
         */
        int getSceneHeight();

        void paintMeasureLines(QPainter *g, QRect visibleArea);

        void paintSongPosition(QPainter *g, QRect visibleArea);

    signals:
        /**
         * @brief ui->PianoRoll での MousePress イベントが起きたとき呼ばれる
         */
        void onMousePress(QMouseEvent *event);

        /**
         * @brief ui->PianoRoll での MouseMove イベントが起きたとき呼ばれる
         */
        void onMouseMove(QMouseEvent *event);

        /**
         * @brief ui->PianoRoll での MouseRelease イベントが起きたとき呼ばれる
         */
        void onMouseRelease(QMouseEvent *event);

        /**
         * @brief Emitted when mouse double click event on ui->pianoRoll.
         */
        void onMouseDoubleClick(QMouseEvent *event);
    };
}
#endif
