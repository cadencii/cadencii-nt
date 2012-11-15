/**
 * ConcretePropertyValueProxy.cpp
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
#include <string>
#include "ConcretePropertyValueProxy.hpp"
#include "ConcretePropertyView.hpp"

namespace cadencii {

    ConcretePropertyValueProxy::ConcretePropertyValueProxy() :
        PropertyValueProxy(), parent(0) {
    }

    void ConcretePropertyValueProxy::setParent(ConcretePropertyView *parent) {
        this->parent = parent;
    }

    void ConcretePropertyValueProxy::setLyricPhrase(const std::string &lyricPhrase) {
        parent->stringPropertyManager.setValue(
            parent->lyricPhrase, lyricPhrase.c_str());
    }

    void ConcretePropertyValueProxy::setLyricPhoneticSymbol(
            const std::string &lyricPhoneticSymbol) {
        parent->stringPropertyManager.setValue(
            parent->lyricPhoneticSymbol, lyricPhoneticSymbol.c_str());
    }

    void ConcretePropertyValueProxy::setLyricConsonantAdjustment(
            const std::string &lyricConsonantAdjustment) {
        parent->stringPropertyManager.setValue(
            parent->lyricConsonantAdjustment, lyricConsonantAdjustment.c_str());
    }

    void ConcretePropertyValueProxy::setLyricProtect(int lyricProtect) {
        parent->enumPropertyManager.setValue(parent->lyricProtect, lyricProtect);
    }

    void ConcretePropertyValueProxy::setNoteLength(const std::string &noteLength) {
        parent->stringPropertyManager.setValue(
            parent->noteLength, noteLength.c_str());
    }

    void ConcretePropertyValueProxy::setNoteNumber(const std::string &noteNumber) {
        parent->stringPropertyManager.setValue(
            parent->noteNumber, noteNumber.c_str());
    }

    void ConcretePropertyValueProxy::setNotelocationClock(const std::string &notelocationClock) {
        parent->stringPropertyManager.setValue(
            parent->notelocationClock, notelocationClock.c_str());
    }

    void ConcretePropertyValueProxy::setNotelocationMeasure(
            const std::string &notelocationMeasure) {
        parent->stringPropertyManager.setValue(
            parent->notelocationMeasure, notelocationMeasure.c_str());
    }

    void ConcretePropertyValueProxy::setNotelocationBeat(const std::string &notelocationBeat) {
        parent->stringPropertyManager.setValue(
            parent->notelocationBeat, notelocationBeat.c_str());
    }

    void ConcretePropertyValueProxy::setNotelocationTick(const std::string &notelocationTick) {
        parent->stringPropertyManager.setValue(
            parent->notelocationTick, notelocationTick.c_str());
    }

    void ConcretePropertyValueProxy::setVibratoType(int vibratoType) {
        parent->enumPropertyManager.setValue(parent->vibratoType, vibratoType);
    }

    void ConcretePropertyValueProxy::setVibratoLength(const std::string &vibratoLength) {
        parent->stringPropertyManager.setValue(
            parent->vibratoLength, vibratoLength.c_str());
    }
}
