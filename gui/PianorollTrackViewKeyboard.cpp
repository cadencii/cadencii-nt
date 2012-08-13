/**
 * PianorollTrackViewKeyboard.cpp
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
#include <QPainter>
#include <sstream>
#include "PianorollTrackView.h"
#include "PianorollTrackViewContent.h"
#include "PianorollTrackViewKeyboard.h"

using namespace std;

PianorollTrackViewKeyboard::PianorollTrackViewKeyboard( QWidget *parent ) :
    QWidget( parent )
{
    // キーボードのキーの音名を作成
    keyNames = new QString[PianorollTrackViewContent::NOTE_MAX - PianorollTrackViewContent::NOTE_MIN + 1];
    char *names[] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B" };
    for( int noteNumber = PianorollTrackViewContent::NOTE_MIN; noteNumber <= PianorollTrackViewContent::NOTE_MAX; noteNumber++ ){
        int modura = PianorollTrackViewContent::getNoteModuration( noteNumber );
        int order = PianorollTrackViewContent::getNoteOctave( noteNumber );
        char *name = names[modura];
        ostringstream oss;
        oss << name << order;
        keyNames[noteNumber - PianorollTrackViewContent::NOTE_MIN] = QString( oss.str().c_str() );
    }

    trackHeight = PianorollTrackViewContent::DEFAULT_TRACK_HEIGHT;
}

PianorollTrackViewKeyboard::~PianorollTrackViewKeyboard()
{
    delete [] keyNames;
}

void PianorollTrackViewKeyboard::notifyVerticalScroll( int y )
{
    this->top = -y;
    this->repaint();
}

void PianorollTrackViewKeyboard::paintEvent( QPaintEvent *e )
{
    QPainter p( this );
    this->paintKeyboard( &p );
}

void PianorollTrackViewKeyboard::paintKeyboard( QPainter *g ){
    // カーソル位置でのノート番号を取得する
    QPoint cursor = QCursor::pos();
    QPoint pianoroll = mapToGlobal( QPoint( 0, 0 ) );
    int noteAtCursor = PianorollTrackViewContent::getNoteNumberFromY( cursor.y() - pianoroll.y() - top, trackHeight );

    g->fillRect( 0, 0, width(), height(),
                 QColor( 240, 240, 240 ) );
    QColor keyNameColor = QColor( 72, 77, 98 );
    QColor blackKeyColor = QColor( 125, 123, 124 );
    for( int noteNumber = PianorollTrackViewContent::NOTE_MIN - 1; noteNumber <= PianorollTrackViewContent::NOTE_MAX; noteNumber++ ){
        int y = PianorollTrackViewContent::getYFromNoteNumber( noteNumber, trackHeight ) + top;
        int modura = PianorollTrackViewContent::getNoteModuration( noteNumber );

        // C4 などの表示を描画
        if( modura == 0 || noteAtCursor == noteNumber ){
            g->setPen( keyNameColor );
            g->drawText( 0, y, width() - 2, trackHeight,
                         Qt::AlignRight | Qt::AlignVCenter, keyNames[noteNumber - PianorollTrackViewContent::NOTE_MIN] );
        }

        // 鍵盤ごとの横線
        g->setPen( QColor( 212, 212, 212 ) );
        g->drawLine( 0, y, width(), y );

        // 黒鍵を描く
        if( modura == 1 || modura == 3 || modura == 6 || modura == 8 || modura == 10 ){
            g->fillRect( 0, y, 34, trackHeight + 1, blackKeyColor );
        }
    }

    // 鍵盤とピアノロール本体との境界線
    g->setPen( QColor( 212, 212, 212 ) );
    g->drawLine( width() - 1, 0, width() - 1, height() );
}

void PianorollTrackViewKeyboard::setPianoroll( PianorollTrackView *pianoroll )
{
    this->pianoroll = pianoroll;
}

void PianorollTrackViewKeyboard::setTrackHeight( int trackHeight )
{
    this->trackHeight = trackHeight;
}
