/**
 * PropertyValueProxy.hpp
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
#ifndef CADENCII_GUI_PROPERTYVALUEPROXY_HPP_
#define CADENCII_GUI_PROPERTYVALUEPROXY_HPP_

#include <map>
#include <string>
#include "../vsq/Sequence.hpp"

namespace cadencii {

    /**
     * @brief Proxy class to (get/)set aggregated property of VSQ_NS::Event items.
     */
    class PropertyValueProxy {
    private:
        std::map<void *, bool> isDefault;
        bool isFirst;

        std::string lyricPhrase;
        std::string lyricPhoneticSymbol;
        std::string lyricConsonantAdjustment;
        /**
         * @brief Whether to protect phonetic symbols.
         *     0: undefined
         *     1: Off(not protected)
         *     2: On(protected)
         */
        int lyricProtect;
        VSQ_NS::tick_t noteLength;
        int noteNumber;
        VSQ_NS::tick_t notelocationClock;
        int notelocationMeasure;
        int notelocationBeat;
        int notelocationTick;
        int vibratoType;
        int vibratoLength;

    public:
        PropertyValueProxy() {
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

        /**
         * @brief Start setting up property tree.
         */
        void begin() {
            std::map<void *, bool>::iterator i = isDefault.begin();
            for (; i != isDefault.end(); ++i) {
                void *property = i->first;
                isDefault[property] = true;
            }
            isFirst = true;
        }

        /**
         * @brief Add property value of \a item into property tree.
         */
        void add(const VSQ_NS::Event *item, const VSQ_NS::Sequence *sequence) {
            VSQ_NS::Lyric lyric("a", "a");
            if (0 < item->lyricHandle.getLyricCount()) {
                lyric = item->lyricHandle.getLyricAt(0);
            }

            VSQ_NS::tick_t clock = item->clock;
            int premeasure = sequence->getPreMeasure();
            int measure = sequence->timesigList.getBarCountFromClock(clock) - premeasure + 1;
            int clock_bartop = sequence->timesigList.getClockFromBarCount(measure + premeasure - 1);
            VSQ_NS::Timesig timesig = sequence->timesigList.getTimesigAt(clock);
            int den = timesig.denominator;
            int dif = clock - clock_bartop;
            int step = 480 * 4 / den;
            int beat = dif / step + 1;
            int tick = dif - (beat - 1) * step;

            int vibType = 0;
            int vibLength = 0;
            if (item->vibratoHandle.getHandleType() == VSQ_NS::HandleType::VIBRATO) {
                if (item->vibratoHandle.iconId.length() == 9 && 0 < item->getLength()) {
                    std::string vibTypeString = item->vibratoHandle.iconId.substr(6);
                    vibType = StringUtil::parseInt<int>(vibTypeString, 16);
                    vibLength = item->vibratoHandle.getLength() * 100 / item->getLength();
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

        /**
         * @brief Finish setting up property tree.
         */
        void commit() {
            setLyricPhrase(isDefault[&lyricPhrase] ? "" : lyricPhrase);
            setLyricPhoneticSymbol(isDefault[&lyricPhoneticSymbol] ? "" : lyricPhoneticSymbol);
            setLyricConsonantAdjustment(isDefault[&lyricConsonantAdjustment] ? "" : lyricConsonantAdjustment);
            setLyricProtect(isDefault[&lyricProtect] ? 0 : lyricProtect);

            setNoteLength(isDefault[&noteLength] ? "" : StringUtil::toString(noteLength));
            setNoteNumber(isDefault[&noteNumber] ? "" : StringUtil::toString(noteNumber));

            setNotelocationClock(isDefault[&notelocationClock] ? "" : StringUtil::toString(notelocationClock));
            setNotelocationMeasure(isDefault[&notelocationMeasure] ? "" : StringUtil::toString(notelocationMeasure));
            setNotelocationBeat(isDefault[&notelocationBeat] ? "" : StringUtil::toString(notelocationBeat));
            setNotelocationTick(isDefault[&notelocationTick] ? "" : StringUtil::toString(notelocationTick));

            setVibratoType(isDefault[&vibratoType] ? 0 : vibratoType);
            setVibratoLength(isDefault[&vibratoLength] ? "" : StringUtil::toString(vibratoLength));
        }

    protected:
        /**
         * @brief Set phrase value to property view component.
         */
        virtual void setLyricPhrase(const std::string &lyricPhrase) = 0;

        /**
         * @brief Set phonetic symbol value to property view component.
         */
        virtual void setLyricPhoneticSymbol(const std::string &lyricPhoneticSymbol) = 0;

        /**
         * @brief Set consonant adjustment value to property view component.
         */
        virtual void setLyricConsonantAdjustment(const std::string &lyricConsonantAdjustment) = 0;

        /**
         * @brief Set lyric protect value to property view component.
         */
        virtual void setLyricProtect(int lyricProtect) = 0;

        /**
         * @brief Set note length value to property view component.
         */
        virtual void setNoteLength(const std::string &noteLength) = 0;

        /**
         * @brief Set note number value to property view component.
         */
        virtual void setNoteNumber(const std::string &noteNumber) = 0;

        /**
         * @brief Set note location clock value to property view component.
         */
        virtual void setNotelocationClock(const std::string &notelocationClock) = 0;

        /**
         * @brief Set note location measure value to property view component.
         */
        virtual void setNotelocationMeasure(const std::string &notelocationMeasure) = 0;

        /**
         * @brief Set note location beat value to property view component.
         */
        virtual void setNotelocationBeat(const std::string &notelocationBeat) = 0;

        /**
         * @brief Set note location tick value to property view component.
         */
        virtual void setNotelocationTick(const std::string &notelocationTick) = 0;

        /**
         * @brief Set vibrato type value to property view component.
         */
        virtual void setVibratoType(int vibratoType) = 0;

        /**
         * @brief Set vibrato length value to property view component.
         */
        virtual void setVibratoLength(const std::string &vibratoLength) = 0;

    private:
        template<typename T>
        inline void setupDefaultKey(T *key) {
            isDefault.insert(std::make_pair(static_cast<void *>(key), false));
        }

        template<typename T>
        inline void setValue(T *property, const T &value) {
            if (isFirst) {
                *property = value;
                isDefault[property] = false;
            } else if (!isDefault[property]) {
                if (*property != value) {
                    isDefault[property] = true;
                }
            }
        }
    };
}

#endif
