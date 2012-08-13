/**
 * PianorollTrackViewContentScroller.h
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
#ifndef __PianorollTrackViewContentScroller_h__
#define __PianorollTrackViewContentScroller_h__

#include <QScrollArea>

namespace cadencii{

    class PianorollTrackView;

    class PianorollTrackViewContentScroller : public QScrollArea{
        Q_OBJECT

    private:
        PianorollTrackView *pianoroll;

    public:
        explicit PianorollTrackViewContentScroller( QWidget *parent = 0 );

        /**
         * @brief このインスタンスを持っているピアノロールを設定する
         * @param pianoroll ピアノロール
         */
        void setPianoroll( PianorollTrackView *pianoroll );

        void scrollContentsBy( int dx, int dy );
    };

}
#endif
