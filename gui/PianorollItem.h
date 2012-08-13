/**
 * PianorollItem.h
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
#ifndef PIANOROLLITEM_H
#define PIANOROLLITEM_H

#include <string>

class PianorollItem
{
public:
    /**
     * ノート番号
     */
    int noteNumber;

    /**
     * tick 単位の長さ
     */
    int length;

    /**
     * 歌詞
     */
    std::string phrase;

    /**
     * 発音記号
     */
    std::string symbols;

public:
    PianorollItem();
};

#endif // PIANOROLLITEM_H
