/**
 * CurveControlChangeViewContent.h
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
#ifndef __CurveControlChangeViewContent_h__
#define __CurveControlChangeViewContent_h__

#include <map>
#include <QMutex>
#include <QGraphicsView>
#include "vsq/TimesigList.hpp"
#include "vsq/MeasureLineIterator.hpp"
#include "vsq/Sequence.hpp"

namespace cadencii{

    class CurveControlChangeView;

    class CurveControlChangeViewContent : public QGraphicsView{
        Q_OBJECT

    private:
        CurveControlChangeView *controlChangeView;

        /**
         * @brief 描画されるシーケンス
         */
        VSQ_NS::Sequence *sequence;

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

        /**
         * @brief 描画アイテムのリストをロックするための Mutex
         */
        QMutex *mutex;

        QGraphicsScene *scene;

        bool deconstructStarted;

    public:
        explicit CurveControlChangeViewContent( QWidget *parent = 0 );

        ~CurveControlChangeViewContent();

        /**
         * @brief 描画対象のシーケンスを設定する
         * @param items 描画対象のシーケンス
         */
        void setSequence( VSQ_NS::Sequence *sequence );

        /**
         * @brief テンポ変更リストを設定する
         * @param timesigList テンポ変更リスト
         */
        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void drawForeground( QPainter *painter, const QRectF &rect );

        /**
         * オーバーライドする。再描画処理が追加される
         */
        void mouseMoveEvent( QMouseEvent *e );

        void scrollContentsBy( int dx, int dy );

        /**
         * @brief このインスタンスを持っているコントロールチェンジビューワを設定する
         * @param controlChangeView コントロールチェンジビューワ
         */
        void setControlChangeView( CurveControlChangeView *controlChangeView );

        /**
         * @brief スクロールされた結果、可視状態となっている領域を取得する
         */
        QRect getVisibleArea();

        /**
         * @brief ミューテックスを設定する
         * @param mutex ミューテックス
         */
        void setMutex( QMutex *mutex );

        int getSceneWidth();

    private:
        /**
         * ピアノロールのバックグラウンドを描画する
         */
        void paintBackground( QPainter *g, QRect visibleArea );

        /**
         * アイテムを描画する
         */
        void paintItems( QPainter *g, QRect visibleArea );

        /**
         * 1拍ごとの線を描画する
         */
        void paintMeasureLines( QPainter *g, QRect visibleArea );

        /**
         * @brief ソングポジションを描画する
         */
        void paintSongPosition( QPainter *g, QRect visibleArea );
    };

}
#endif
