/**
 * PianorollKeyboard.cpp
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
#include "Pianoroll.h"
#include "PianorollContent.h"
#include "PianorollKeyboard.h"

using namespace std;

PianorollKeyboard::PianorollKeyboard( QWidget *parent ) :
    QWidget( parent )
{
    // キーボードのキーの音名を作成
    keyNames = new QString[PianorollContent::NOTE_MAX - PianorollContent::NOTE_MIN + 1];
    char *names[] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B" };
    for( int noteNumber = PianorollContent::NOTE_MIN; noteNumber <= PianorollContent::NOTE_MAX; noteNumber++ ){
        int modura = PianorollContent::getNoteModuration( noteNumber );
        int order = PianorollContent::getNoteOctave( noteNumber );
        char *name = names[modura];
        ostringstream oss;
        oss << name << order;
        keyNames[noteNumber - PianorollContent::NOTE_MIN] = QString( oss.str().c_str() );
    }

    trackHeight = PianorollContent::DEFAULT_TRACK_HEIGHT;
}

PianorollKeyboard::~PianorollKeyboard()
{
    delete [] keyNames;
}

void PianorollKeyboard::notifyVerticalScroll( int y )
{
    this->top = -y;
    this->repaint();
}

void PianorollKeyboard::paintEvent( QPaintEvent *e )
{
    QPainter p( this );
    this->paintKeyboard( &p );
}

void PianorollKeyboard::paintKeyboard( QPainter *g ){
    // カーソル位置でのノート番号を取得する
    QPoint cursor = QCursor::pos();
    QPoint pianoroll = mapToGlobal( QPoint( 0, 0 ) );
    int noteAtCursor = PianorollContent::getNoteNumberFromY( cursor.y() - pianoroll.y() - top, trackHeight );

    g->fillRect( 0, 0, width(), height(),
                 QColor( 240, 240, 240 ) );
    QColor keyNameColor = QColor( 72, 77, 98 );
    QColor blackKeyColor = QColor( 125, 123, 124 );
    for( int noteNumber = PianorollContent::NOTE_MIN - 1; noteNumber <= PianorollContent::NOTE_MAX; noteNumber++ ){
        int y = PianorollContent::getYFromNoteNumber( noteNumber, trackHeight ) + top;
        int modura = PianorollContent::getNoteModuration( noteNumber );

        // C4 などの表示を描画
        if( modura == 0 || noteAtCursor == noteNumber ){
            g->setPen( keyNameColor );
            g->drawText( 0, y, width() - 2, trackHeight,
                         Qt::AlignRight | Qt::AlignVCenter, keyNames[noteNumber - PianorollContent::NOTE_MIN] );
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

void PianorollKeyboard::setPianoroll( Pianoroll *pianoroll )
{
    this->pianoroll = pianoroll;
}

void PianorollKeyboard::setTrackHeight( int trackHeight )
{
    this->trackHeight = trackHeight;
}
