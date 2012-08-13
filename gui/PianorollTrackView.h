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

#include <map>
#include <QScrollArea>
#include <QMutex>
#include "vsq/MeasureLineIterator.hpp"
#include "vsq/TimesigList.hpp"
#include "vsq/Track.hpp"
#include "gui/PianorollTrackViewContentScroller.h"
#include "gui/PianorollTrackViewKeyboard.h"
#include "TrackView.hpp"

namespace cadencii{
    namespace Ui{
        class PianorollTrackView;
    }

    class PianorollTrackView : public QWidget, public TrackView{
        friend class PianorollTrackViewContentScroller;

        Q_OBJECT

    public:
        Ui::PianorollTrackView *ui;

        explicit PianorollTrackView( QWidget *parent = 0 );

        void setSongPosition( VSQ_NS::tick_t songPosition, bool autoScroll );

        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void setTrack( VSQ_NS::Track *track );

        void ensureNoteVisible( VSQ_NS::tick_t tick, VSQ_NS::tick_t length, int noteNumber );

        /**
         * @brief ピアノロールのレーン1本の高さ(ピクセル単位)を設定する
         * @param trackHeight レーンの高さ(ピクセル単位)
         */
        void setTrackHeight( int trackHeight );

        /**
         * @brief ソングポジションを取得する
         * @return ソングポジション
         */
        VSQ_NS::tick_t getSongPosition();

        /**
         * @brief オーバーライドする。ピアノロール本体と、鍵盤部分を repaint する処理を追加している。
         */
        void repaint();

        /**
         * @brief ミューテックスを設定する
         * @param mutex ミューテックス
         */
        void setMutex( QMutex *mutex );

        /**
         * @brief ソングポジションの移動に伴って自動スクロールするかどうかを取得する
         * @return 自動スクロールする場合は true を返す
         */
        bool isAutoScroll();

        /**
         * @brief 曲頭から Musical Part 開始位置のオフセット
         * @param musicalPartOffset オフセット
         */
        void setMusicalPartOffset( VSQ_NS::tick_t musicalPartOffset );

    private:
        /**
         * @brief スクロール領域が縦方向にスクロールしたことを PianorollContentScroller -> Pianoroll に通知する
         */
        void notifyVerticalScroll();
    };

}
#endif // __PianorollTrackView_h__
