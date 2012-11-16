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
     *    Note: The vibrato length, Handle::getLength() is treated as in 'percentage' unit in Cadencii.
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
        PropertyValueProxy();

        virtual ~PropertyValueProxy();

        /**
         * @brief Start setting up property tree.
         */
        void begin();

        /**
         * @brief Add property value of \a item into property tree.
         */
        void add(const VSQ_NS::Event *item, const VSQ_NS::Sequence *sequence);

        /**
         * @brief Finish setting up property tree.
         */
        void commit();

        void getNotelocation(
                VSQ_NS::tick_t clock, int *measure, int *beat, int *tick,
                const VSQ_NS::Sequence *sequence) {
            int premeasure = sequence->getPreMeasure();
            *measure = sequence->timesigList.getBarCountFromClock(clock) - premeasure + 1;
            int clock_bartop
                = sequence->timesigList.getClockFromBarCount(*measure + premeasure - 1);
            VSQ_NS::Timesig timesig = sequence->timesigList.getTimesigAt(clock);
            int den = timesig.denominator;
            int dif = clock - clock_bartop;
            int step = 480 * 4 / den;
            *beat = dif / step + 1;
            *tick = dif - (*beat - 1) * step;
        }

    protected:
        /**
         * @brief Get phrase value from property view component.
         */
        virtual std::string getLyricPhrase() = 0;

        /**
         * @brief Get phonetic symbol value from property view component.
         */
        virtual std::string getLyricPhoneticSymbol() = 0;

        /**
         * @brief Get consonant adjustment value from property view component.
         */
        virtual std::string getLyricConsonantAdjustment() = 0;

        /**
         * @brief Get lyric protect value from property view component.
         */
        virtual int getLyricProtect() = 0;

        /**
         * @brief Get note length value from property view component.
         */
        virtual std::string getNoteLength() = 0;

        /**
         * @brief Get note number value from property view component.
         */
        virtual std::string getNoteNumber() = 0;

        /**
         * @brief Get note location clock value from property view component.
         */
        virtual std::string getNotelocationClock() = 0;

        /**
         * @brief Get note location measure value from property view component.
         */
        virtual std::string getNotelocationMeasure() = 0;

        /**
         * @brief Get note location beat value from property view component.
         */
        virtual std::string getNotelocationBeat() = 0;

        /**
         * @brief Get note location tick value from property view component.
         */
        virtual std::string getNotelocationTick() = 0;

        /**
         * @brief Get vibrato type value from property view component.
         */
        virtual int getVibratoType() = 0;

        /**
         * @brief Get vibrato length value from property view component.
         */
        virtual std::string getVibratoLength() = 0;

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
