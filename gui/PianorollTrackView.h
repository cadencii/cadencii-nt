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
#include "vsq/Sequence.hpp"
#include "gui/CurveControlChangeViewContent.h"
#include "gui/PianorollTrackViewKeyboard.h"
#include "TrackView.hpp"

namespace Ui{
    class PianorollTrackView;
}

namespace cadencii{
    class PianorollTrackView : public QWidget, public TrackView{
        friend class PianorollTrackViewContentScroller;
        friend class PianorollTrackViewContent;

        Q_OBJECT

    public:
        Ui::PianorollTrackView *ui;

    public:
        explicit PianorollTrackView( QWidget *parent = 0 );

        ~PianorollTrackView();

        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void setSequence( VSQ_NS::Sequence *sequence );

        void ensureNoteVisible( VSQ_NS::tick_t tick, VSQ_NS::tick_t length, int noteNumber );

        void *getWidget();

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        /**
         * @brief ピアノロールのレーン1本の高さ(ピクセル単位)を設定する
         * @param trackHeight レーンの高さ(ピクセル単位)
         */
        void setTrackHeight( int trackHeight );

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

    private:
        /**
         * @brief スクロール領域が縦方向にスクロールしたことを PianorollContentScroller -> Pianoroll に通知する
         */
        void notifyVerticalScroll();

        /**
         * @brief スクロール領域が横方向にスクロールしたことを PianorollContentScroller -> Pianoroll に通知する
         */
        void notifyHorizontalScroll();
    };

}
#endif // __PianorollTrackView_h__
