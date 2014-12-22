/**
 * ConcretePropertyValueProxy.hpp
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
#ifndef CADENCII_QT_GUI_CONCRETE_PROPERTY_VIEW_CONCRETEPROPERTYVALUEPROXY_HPP_
#define CADENCII_QT_GUI_CONCRETE_PROPERTY_VIEW_CONCRETEPROPERTYVALUEPROXY_HPP_

#include <string>
#include "../../gui/PropertyValueProxy.hpp"

namespace cadencii {

    class ConcretePropertyView;

    class ConcretePropertyValueProxy : public PropertyValueProxy {
    private:
        ConcretePropertyView *parent;

    public:
        ConcretePropertyValueProxy();

        void setParent(ConcretePropertyView *parent);

        std::string getLyricPhrase() override;

        std::string getLyricPhoneticSymbol() override;

        std::string getLyricConsonantAdjustment() override;

        int getLyricProtect() override;

        std::string getNoteNumber() override;

        std::string getNoteLength() override;

        std::string getNotelocationClock() override;

        std::string getNotelocationMeasure() override;

        std::string getNotelocationBeat() override;

        std::string getNotelocationTick() override;

        int getVibratoType() override;

        std::string getVibratoLength() override;

        void setLyricPhrase(const std::string &lyricPhrase) override;

        void setLyricPhoneticSymbol(const std::string &lyricPhoneticSymbol) override;

        void setLyricConsonantAdjustment(const std::string &lyricConsonantAdjustment) override;

        void setLyricProtect(int lyricProtect) override;

        void setNoteLength(const std::string &noteLength) override;

        void setNoteNumber(const std::string &noteNumber) override;

        void setNotelocationClock(const std::string &notelocationClock) override;

        void setNotelocationMeasure(const std::string &notelocationMeasure) override;

        void setNotelocationBeat(const std::string &notelocationBeat) override;

        void setNotelocationTick(const std::string &notelocationTick) override;

        void setVibratoType(int vibratoType) override;

        void setVibratoLength(const std::string &vibratoLength) override;
    };
}

#endif
