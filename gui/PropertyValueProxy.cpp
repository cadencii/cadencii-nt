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

    void PropertyValueProxy::add(const VSQ_NS::Event *item, const VSQ_NS::Sequence *sequence) {
        VSQ_NS::Lyric lyric("a", "a");
        if (0 < item->lyricHandle.getLyricCount()) {
            lyric = item->lyricHandle.getLyricAt(0);
        }

        VSQ_NS::tick_t clock = item->clock;
        int measure, beat, tick;
        getNotelocation(item->clock, &measure, &beat, &tick, sequence);

        int vibType = 0;
        int vibLength = 0;
        if (item->vibratoHandle.getHandleType() == VSQ_NS::HandleType::VIBRATO) {
            if (item->vibratoHandle.iconId.length() == 9 && 0 < item->getLength()) {
                std::string vibTypeString = item->vibratoHandle.iconId.substr(6);
                vibType = StringUtil::parseInt<int>(vibTypeString, 16) + 1;
                vibLength = item->vibratoHandle.getLength();
            }
        }

        setValue(&lyricPhrase, lyric.phrase);
        setValue(&lyricPhoneticSymbol, lyric.getPhoneticSymbol());
        setValue(&lyricConsonantAdjustment, lyric.getConsonantAdjustment());
        setValue(&lyricProtect, lyric.isProtected ? 2 : 1);

        setValue(&noteLength, item->getLength());
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

        setNoteLength(isDefault[&noteLength] ? "" : StringUtil::toString(noteLength));
        setNoteNumber(isDefault[&noteNumber] ? "" : StringUtil::toString(noteNumber));

        setNotelocationClock(
            isDefault[&notelocationClock] ? "" : StringUtil::toString(notelocationClock));
        setNotelocationMeasure(
            isDefault[&notelocationMeasure] ? "" : StringUtil::toString(notelocationMeasure));
        setNotelocationBeat(
            isDefault[&notelocationBeat] ? "" : StringUtil::toString(notelocationBeat));
        setNotelocationTick(
            isDefault[&notelocationTick] ? "" : StringUtil::toString(notelocationTick));

        setVibratoType(isDefault[&vibratoType] ? 0 : vibratoType);
        setVibratoLength(isDefault[&vibratoLength] ? "" : StringUtil::toString(vibratoLength));
    }
}
