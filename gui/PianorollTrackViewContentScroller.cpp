/**
 * PianorollTrackViewContentScroller.cpp
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
#include "PianorollTrackViewContentScroller.h"
#include "PianorollTrackView.h"
#include <sstream>

PianorollTrackViewContentScroller::PianorollTrackViewContentScroller( QWidget *parent ) :
    QScrollArea( parent )
{
}

void PianorollTrackViewContentScroller::scrollContentsBy( int dx, int dy )
{
    QScrollArea::scrollContentsBy( dx, dy );
    if( dy ){
        this->pianoroll->notifyVerticalScroll();
    }
}

void PianorollTrackViewContentScroller::setPianoroll( PianorollTrackView *pianoroll )
{
    this->pianoroll = pianoroll;
}
