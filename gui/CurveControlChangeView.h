/**
 * CurveControlChangeView.h
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
#ifndef __CurveControlChangeView_h__
#define __CurveControlChangeView_h__

#include "EditorWidgetBase.h"
#include "../ControlChangeView.hpp"
#include <QGraphicsScene>

namespace cadencii{

    class CurveControlChangeView : public EditorWidgetBase, public ControlChangeView{
    private:
        /**
         * @brief 歌手変更イベントの表示状態を表す
         */
        enum SingerItemState{
            /**
             * @brief デフォルト
             */
            DEFAULT,
            /**
             * @brief 左端に、薄い色で表示された状態
             */
            LEFT
        };

        VSQ_NS::Sequence *sequence;
        int trackIndex;
        /**
         * @brief 手前に表示している BPList
         */
        VSQ_NS::BPList *front;
        /**
         * @brief 歌手変更イベントを描画するレーン、トラック一覧を描画するレーンの高さ（共通）
         */
        static const int LANE_HEIGHT = 19;
        /**
         * @brief コンポーネントの上端と、グラフの最大値の位置の距離
         */
        static const int MARGIN_TOP = 8;
        /**
         * @brief コンポーネントの下端と、グラフの最小の位置の距離
         */
        static const int MARGIN_BOTTOM = 42;
        /**
         * @brief RENDERボタンの幅(px)
         */
        static const int RENDER_BUTTON_WIDTH = 10;
        /**
         * @brief トラック一覧のうち、トラック一つ分のタブの最大描画幅
         */
        static const int TRACK_TAB_MAX_WIDTH = 80;
        /**
         * @brief 歌手変更イベントの表示矩形の幅
         */
        static const int SINGER_ITEM_WIDTH = 66;
        /**
         * @brief デフォルトで描画するシーケンス
         */
        VSQ_NS::Sequence defaultSequence;
        /**
         * @brief 表示中のコントロールチェンジの名前
         */
        std::string controlChangeName;
        /**
         * @brief トラックの背景部分の塗りつぶし色。16トラックそれぞれで異なる
         */
        QColor *trackTabHilightBackgroundColor;
        /**
         * @brief トラックをレンダリングするためのボタンの背景色。16トラックそれぞれで異なる
         */
        QColor *trackTabRenderButtonBackgroundColor;
        /**
         * @brief コンポーネント間の区切り線の色
         */
        QColor borderColor;
        /**
         * @brief 歌手変更を表すボックスの枠線の色
         */
        QColor singerEventBorderColor;

    public:
        CurveControlChangeView( QWidget *parent = 0 );

        ~CurveControlChangeView();

        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void setSequence( VSQ_NS::Sequence *sequence );

        void *getWidget();

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void paintMainContent( QPainter *painter, const QRect &rect );

        void *getScrollEventSender();

        QSizeF getPreferedSceneSize();

        void setTrackIndex( int index );

        void setControlChangeName( const std::string &name );

        void mousePressEvent( QMouseEvent *event );

    protected:
        void drawMeasureLine( QPainter *painter, const QRect &rect, int x, bool isBorder );

    private:
        void paintBPList( QPainter *painter, VSQ_NS::BPList *list, const QRect &rect );

        /**
         * @brief コントロールチェンジの値から、描画するY座標を取得する
         * @param max コントロールチェンジの最大値
         * @param min コントロールチェンジの最小値
         * @param value コントロールチェンジの値
         */
        int getYFromValue( int max, int min, int value );

        /**
         * @brief コンポーネントのY座標から、コントロールチェンジの値を取得する
         * @param max コントロールチェンジの最大値
         * @param min コントロールチェンジの最小値
         * @param y コンポーネントのY座標
         */
        int getValueFromY( int max, int min, int y );

        /**
         * @brief トラック一覧の部分を描画する
         * @param painter 描画に使う QPainter
         */
        void paintTrackList( QPainter *painter );

        /**
         * @brief トラック一覧のうち、トラック一つ分のタブを表示する
         */
        void paintTrackTab( QPainter *g, const QRect &destRect, const QString &name, bool selected, bool enabled, bool render_required, const QColor &hilight, const QColor &render_button_hilight );

        /**
         * @brief トラック一覧のうち、トラック一つ分のタブの描画幅を取得する
         */
        int getTrackTabWidth();

        /**
         * @brief トラックリスト部分への、MousePress イベントを処理する
         * @param event マウスイベント
         * @return 処理された場合は true、マウスの位置が範囲該当で処理されなかった場合は false を返す
         */
        bool acceptMousePressOnTrackList( QMouseEvent *event );

        /**
         * @brief 歌手の一覧を描画する
         */
        void paintSingerList( QPainter *painter );

        /**
         * @brief 歌手変更イベントを指定された位置に描画する
         */
        void paintSinger( QPainter *painter, const VSQ_NS::Event &singerEvent, int x, int y, SingerItemState state );
    };

}

#endif
