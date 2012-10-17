/**
 * PianorollTrackView.hpp
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
#ifndef __PianorollTrackView_hpp__
#define __PianorollTrackView_hpp__

#include "EditorWidgetBase.hpp"
#include "../../gui/TrackView.hpp"
#include <QGraphicsScene>

namespace cadencii{

    class PianorollTrackView : public EditorWidgetBase, public TrackView{
        Q_OBJECT

    private:
        /**
         * @brief マウスの状態
         */
        class MouseStatus{
        public:
            /**
             * @brief マウスがおりているかどうか
             */
            bool isDown;
            /**
             * @brief 最初にマウスがおりた位置。QGraphicsScene 基準の座標
             */
            QPoint startPosition;
            /**
             * @brief マウスが Up となった位置。QGraphicsScene 基準の座標
             */
            QPoint endPosition;

        public:
            explicit MouseStatus();

            /**
             * @brief 指定したマウス位置を始点とし、マウスが降りたことにする
             * @param mousePosition QGraphicsScene 基準の座標
             */
            void start( const QPoint &mousePosition );

            /**
             * @brief startPosition と endPosition が成す矩形を取得する
             */
            inline QRect rect()const;
        };

    public:
        int trackHeight;

    private:
        /**
         * ノート描画高さのデフォルト値(ピクセル単位)
         */
        static const int DEFAULT_TRACK_HEIGHT = 14;
        static const int NOTE_MAX = 127;
        static const int NOTE_MIN = 0;
        /**
         * @brief 描画アイテムのリストをロックするための Mutex
         */
        QMutex *mutex;
        /**
         * @brief 鍵盤の音の名前(C4など)
         */
        QString *keyNames;
        int trackIndex;
        MouseStatus mouseStatus;

    public:
        PianorollTrackView( QWidget *parent = 0 );

        ~PianorollTrackView();

        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void *getWidget();

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void paintMainContent( QPainter *painter, const QRect &rect );

        void paintSubContent( QPainter *painter, const QRect &rect );

        void *getScrollEventSender();

        QSize getPreferedSceneSize();

        void ensureNoteVisible( VSQ_NS::tick_t tick, VSQ_NS::tick_t length, int noteNumber = -1 );

        void setTrackIndex( int index );

        void updateWidget();

        int getTrackViewWidth();

        /**
         * @brief ピアノロールのレーン1本の高さ(ピクセル単位)を設定する
         * @param trackHeight レーンの高さ(ピクセル単位)
         */
        void setTrackHeight( int trackHeight );

        /**
         * @brief ミューテックスを設定する
         * @param mutex ミューテックス
         */
        void setMutex( QMutex *mutex );
    private slots:
        void onMousePressSlot( QMouseEvent *event );

        void onMouseMoveSlot( QMouseEvent *event );

        void onMouseReleaseSlot( QMouseEvent *event );

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
         * @brief POINTER ツールによる、左ボタンでの MousePress イベントを処理する
         */
        void handleMouseLeftButtonPressByPointer( QMouseEvent *event );

        /**
         * @brief ERASER ツールによる、左ボタンでの MousePress イベントを処理する
         */
        void handleMouseLeftButtonPressByEraser( QMouseEvent *event );

        /**
         * @brief このウィジェットの指定された位置における音符イベントを探す
         * @param mousePosition このウィジェットローカル座標基準の位置
         * @return 音符イベント。見つからなければ null を返す
         */
        const VSQ_NS::Event *findNoteEventAt( const QPoint &mousePosition );

        /**
         * @brief 指定された音符イベントの、描画の際の形状を取得する。
         * @param 形状を取得する音符イベント
         * @return 形状。座標は、QGraphicsScene の座標を用いる
         */
        QRect getNoteItemRect( const VSQ_NS::Event *item );

        /**
         * @brief ui->scrollArea 基準の座標を、scene の座標に変換する
         */
        inline QPoint mapToScene( const QPoint &mousePos );

        /**
         * @brief 矩形選択の四角形内に入っているアイテムを選択し直す
         */
        void updateSelectedItem();

        /**
         * @brief y 座標からノート番号を取得する
         * @param y 座標
         * @return ノート番号
         */
        static int getNoteNumberFromY( int y, int trackHeight );

        /**
         * @brief ノート番号から、描画時の y 座標を取得する
         * @param noteNumber ノート番号
         * @param trackHeight ノートの描画高さ
         */
        static int getYFromNoteNumber( int noteNumber, int trackHeight );

        /**
         * @brief ノート番号から、音名を表す番号を取得する。Cであれば0, C#であれば1など
         * @param noteNumber ノート番号
         * @return 音名を表す番号
         */
        static int getNoteModuration( int noteNumber );

        /**
         * @brief ノート番号から、その音高が何オクターブめかを取得する。
         * @param noteNumber ノート番号
         * @return 何オクターブめかを表す番号
         */
        static int getNoteOctave( int noteNumber );
    };

}

#endif
