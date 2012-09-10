/**
 * EditorWidgetBaseMainContent.h
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __EditorWidgetBaseMainContent_h__
#define __EditorWidgetBaseMainContent_h__

#include <map>
#include <QWidget>
#include <QMutex>
#include <QGraphicsView>
#include "ControllerAdapter.hpp"
#include "vsq/TimesigList.hpp"
#include "vsq/MeasureLineIterator.hpp"
#include "vsq/Sequence.hpp"

namespace cadencii{

    class EditorWidgetBase;

    class EditorWidgetBaseMainContent : public QGraphicsView{
        friend class EditorWidgetBase;

        Q_OBJECT

    protected:
        ControllerAdapter *controllerAdapter;

    private:
        EditorWidgetBase *parentWidget;

        /**
         * 拍ごとの線を描画するための、拍子変更情報
         */
        VSQ_NS::TimesigList *timesigList;

        /**
         * @brief デフォルトの拍子変更情報
         */
        VSQ_NS::TimesigList defaultTimesigList;

        /**
         * @brief 拍ごとの線を描画するために、timesigList から作成した反復子
         */
        VSQ_NS::MeasureLineIterator *measureLineIterator;

        QGraphicsScene *scene;

        bool deconstructStarted;

    public:
        explicit EditorWidgetBaseMainContent( QWidget *parent = 0 );

        ~EditorWidgetBaseMainContent();

        /**
         * @brief テンポ変更リストを設定する
         * @param timesigList テンポ変更リスト
         */
        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void setEditorWidgetBase( EditorWidgetBase *editorWidgetBase );

        /**
         * オーバーライドする。再描画処理が追加される
         */
        void mouseMoveEvent( QMouseEvent *e );

        void drawForeground( QPainter *painter, const QRectF &rect );

        /**
         * スクロールされた結果、可視状態となっている領域を取得する
         */
        QRect getVisibleArea();

        /**
         * @brief ソングポジションを設定する
         */
        void setSongPosition( VSQ_NS::tick_t songPosition );

        /**
         * @brief ソングポジションを取得する
         */
        VSQ_NS::tick_t getSongPosition();

        /**
         * @brief ミューテックスを設定する
         * @param mutex ミューテックス
         */
        void setMutex( QMutex *mutex );

        void scrollContentsBy( int dx, int dy );

        int getSceneWidth();

        void paintMeasureLines( QPainter *g, QRect visibleArea );

        void paintSongPosition( QPainter *g, QRect visibleArea );

    protected:
        /**
         * @brief 小節、拍子ごとの線を描画する。オーバーライドすることで、線の描画ロジックを変更できる。
         * @param painter 描画に使用するペインター
         * @param rect 描画対象の領域(=可視領域)
         * @param x 小節、拍子の x 座標
         * @param isBorder 小節の区切り線であれば true が指定される
         */
        virtual void drawMeasureLine( QPainter *painter, const QRect &rect, int x, bool isBorder );
    };

}
#endif
