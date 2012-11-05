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
#include "pianoroll_track_view/LyricEditWidget.hpp"
#include <QGraphicsScene>
#include <QLineEdit>

namespace cadencii{

    class PianorollTrackView : public EditorWidgetBase, public TrackView{
        Q_OBJECT

    private:
        /**
         * @brief マウスの状態
         */
        class MouseStatus{
        public:
            enum MouseStatusEnum{
                /**
                 * @brief 何もしてない
                 */
                NONE,
                /**
                 * @brief 左ボタンで範囲選択するモード
                 */
                LEFTBUTTON_SELECT_ITEM,
                /**
                 * @brief 左ボタンで音符イベントを移動するモード
                 */
                LEFTBUTTON_MOVE_ITEM,
                /**
                 * @brief Add note item mode.
                 */
                LEFTBUTTON_ADD_ITEM,
                /**
                 * @brief 中ボタンでスクロールするモード
                 */
                MIDDLEBUTTON_SCROLL
            };

        public:
            /**
             * @brief モード
             */
            MouseStatusEnum mode;
            /**
             * @brief 最初にマウスがおりた位置。QGraphicsScene 基準の座標
             */
            QPoint startPosition;
            /**
             * @brief マウスが Up となった位置。QGraphicsScene 基準の座標
             */
            QPoint endPosition;
            /**
             * @brief 最初にマウスが降りた位置。グローバルなスクリーン座標
             */
            QPoint globalStartPosition;
            /**
             * @brief 水平スクロールバーの、マウスが降りた時点での値
             */
            int horizontalScrollStartValue;
            /**
             * @brief 垂直スクロールバーの、マウスが降りた時点での値
             */
            int verticalScrollStartValue;
            /**
             * @brief マウスが動いたかどうか
             */
            bool isMouseMoved;
            /**
             * @brief 範囲選択開始直前の、アイテム選択状態
             */
            ItemSelectionManager itemSelectionStatusAtFirst;
            /**
             * @brief Note item status, adding by mouse dragging.
             */
            VSQ_NS::Event addingNoteItem;
            /**
             * @brief Note item at mouse pressed position.
             */
            const VSQ_NS::Event *noteOnMouse;

        public:
            explicit MouseStatus();

            /**
             * @brief startPosition と endPosition が成す矩形を取得する
             */
            inline QRect rect()const;

            /**
             * @brief ステータスをクリアする
             */
            void clear();
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
        /**
         * @brief A component for editing lyric.
         */
        LyricEditWidget *lyricEdit;

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

        void onMouseDoubleClickSlot( QMouseEvent *event );

        /**
         * @brief Called when graphics scene was scrolled.
         */
        void onContentScroll( int value );

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
         * @brief Handle MousePress event by left button with PENCIL tool or LINE tool.
         */
        void handleMouseLeftButtonPressByPencil( QMouseEvent *event );

        /**
         * @brief 中ボタンでの MousePress イベントを処理する
         */
        void handleMouseMiddleButtonPress( QMouseEvent *event );

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
         * @brief Update 'mouseStatus' field.
         * @param status A kind of mouse status.
         * @param event A mouse event.
         * @param noteOnMouse Note item at mouse pressed position.
         */
        void initMouseStatus( MouseStatus::MouseStatusEnum status, const QMouseEvent *event, const VSQ_NS::Event *noteOnMouse );

        /**
         * @brief Draw a note item.
         */
        inline void paintItem( QPainter *g, const VSQ_NS::Event *item, const QRect &itemRect, const QColor &color, const QColor &borderColor );

        /**
         * @brief Quantize specified tick.
         */
        inline VSQ_NS::tick_t quantize( VSQ_NS::tick_t tick );

        /**
         * @brief Update 'lyricEdit' component position.
         */
        inline void updateLyricEditComponentPosition();

        /**
         * @brief Get component position for specified note event.
         * @param noteEvent A note event.
         * @return Component position.
         */
        inline QPoint getLyricEditPosition( const VSQ_NS::Event *noteEvent );

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
