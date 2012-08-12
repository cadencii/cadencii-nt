/**
 * PianorollContentScroller.h
 * Copyright © 2012 kbinani
 *
 * This file is part of `MIDIInput UG Job Plugin'.
 *
 * `MIDIInput UG Job Plugin' is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * `MIDIInput UG Job Plugin' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __PianorollContentScroller_h__
#define __PianorollContentScroller_h__

#include <QScrollArea>

class Pianoroll;

class PianorollContentScroller : public QScrollArea
{
    Q_OBJECT

private:
    Pianoroll *pianoroll;

public:
    explicit PianorollContentScroller( QWidget *parent = 0 );

    /**
     * @brief このインスタンスを持っているピアノロールを設定する
     * @param pianoroll ピアノロール
     */
    void setPianoroll( Pianoroll *pianoroll );

    void scrollContentsBy( int dx, int dy );
};

#endif
