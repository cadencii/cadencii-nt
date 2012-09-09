/**
 * PianorollTrackView.h
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
#ifndef __PianorollTrackView_h__
#define __PianorollTrackView_h__

#include "EditorWidgetBase.h"
#include "../TrackView.hpp"
#include <QGraphicsScene>

namespace cadencii{

    class PianorollTrackView : public EditorWidgetBase, public TrackView{
    public:
        int trackHeight;

    private:
        VSQ_NS::Sequence *sequence;
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

    public:
        PianorollTrackView( QWidget *parent = 0 );

        ~PianorollTrackView();

        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void setSequence( VSQ_NS::Sequence *sequence );

        void *getWidget();

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void paintMainContent( QPainter *painter, const QRect &rect );

        void paintSubContent( QPainter *painter, const QRect &rect );

        void *getScrollEventSender();

        QSizeF getPreferedSceneSize();

        void ensureNoteVisible( VSQ_NS::tick_t tick, VSQ_NS::tick_t length, int noteNumber );

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
