/**
 * PianorollContentScroller.cpp
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
#include "PianorollContentScroller.h"
#include "Pianoroll.h"
#include <sstream>

PianorollContentScroller::PianorollContentScroller( QWidget *parent ) :
    QScrollArea( parent )
{
}

void PianorollContentScroller::scrollContentsBy( int dx, int dy )
{
    QScrollArea::scrollContentsBy( dx, dy );
    if( dy ){
        this->pianoroll->notifyVerticalScroll();
    }
}

void PianorollContentScroller::setPianoroll( Pianoroll *pianoroll )
{
    this->pianoroll = pianoroll;
}
