/**
 * PropertyValueProxy.cpp
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2.0 as published by the Free Software Foundation.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <map>
#include <string>
#include "PropertyValueProxy.hpp"

namespace cadencii {

    PropertyValueProxy::PropertyValueProxy() {
        begin();
        setupDefaultKey(&lyricPhrase);
        setupDefaultKey(&lyricPhoneticSymbol);
        setupDefaultKey(&lyricConsonantAdjustment);
        setupDefaultKey(&lyricProtect);

        setupDefaultKey(&noteLength);
        setupDefaultKey(&noteNumber);

        setupDefaultKey(&notelocationClock);
        setupDefaultKey(&notelocationMeasure);
        setupDefaultKey(&notelocationBeat);
        setupDefaultKey(&notelocationTick);

        setupDefaultKey(&vibratoType);
        setupDefaultKey(&vibratoLength);
    }

    PropertyValueProxy::~PropertyValueProxy() {
    }

    void PropertyValueProxy::begin() {
        std::map<void *, bool>::iterator i = isDefault.begin();
        for (; i != isDefault.end(); ++i) {
            void *property = i->first;
            isDefault[property] = true;
        }
        isFirst = true;
    }

    void PropertyValueProxy::add(const vsq::Event *item, const vsq::Sequence *sequence) {
        vsq::Lyric lyric("a", "a");
        if (0 < item->lyricHandle.size()) {
            lyric = item->lyricHandle.get(0);
        }

        vsq::tick_t clock = item->tick;
        int measure, beat, tick;
        getNotelocation(item->tick, &measure, &beat, &tick, sequence);

        int vibType = 0;
        int vibLength = 0;
        if (item->vibratoHandle.type() == vsq::HandleType::VIBRATO) {
            if (item->vibratoHandle.iconId.length() == 9 && 0 < item->length()) {
                std::string vibTypeString = item->vibratoHandle.iconId.substr(6);
                vibType = vsq::StringUtil::parseInt<int>(vibTypeString, 16) + 1;
                vibLength = item->vibratoHandle.length();
            }
        }

        setValue(&lyricPhrase, lyric.phrase);
        setValue(&lyricPhoneticSymbol, lyric.phoneticSymbol());
        setValue(&lyricConsonantAdjustment, lyric.consonantAdjustment());
        setValue(&lyricProtect, lyric.isProtected ? 2 : 1);

        setValue(&noteLength, item->length());
        setValue(&noteNumber, item->note);

        setValue(&notelocationClock, clock);
        setValue(&notelocationMeasure, measure);
        setValue(&notelocationBeat, beat);
        setValue(&notelocationTick, tick);

        setValue(&vibratoType, vibType);
        setValue(&vibratoLength, vibLength);

        isFirst = false;
    }

    void PropertyValueProxy::commit() {
        setLyricPhrase(isDefault[&lyricPhrase] ? "" : lyricPhrase);
        setLyricPhoneticSymbol(isDefault[&lyricPhoneticSymbol] ? "" : lyricPhoneticSymbol);
        setLyricConsonantAdjustment(
            isDefault[&lyricConsonantAdjustment] ? "" : lyricConsonantAdjustment);
        setLyricProtect(isDefault[&lyricProtect] ? 0 : lyricProtect);

        setNoteLength(isDefault[&noteLength] ? "" : vsq::StringUtil::toString(noteLength));
        setNoteNumber(isDefault[&noteNumber] ? "" : vsq::StringUtil::toString(noteNumber));

        setNotelocationClock(
            isDefault[&notelocationClock] ? "" : vsq::StringUtil::toString(notelocationClock));
        setNotelocationMeasure(
            isDefault[&notelocationMeasure] ? "" : vsq::StringUtil::toString(notelocationMeasure));
        setNotelocationBeat(
            isDefault[&notelocationBeat] ? "" : vsq::StringUtil::toString(notelocationBeat));
        setNotelocationTick(
            isDefault[&notelocationTick] ? "" : vsq::StringUtil::toString(notelocationTick));

        setVibratoType(isDefault[&vibratoType] ? 0 : vibratoType);
        setVibratoLength(isDefault[&vibratoLength] ? "" : vsq::StringUtil::toString(vibratoLength));
    }
}
