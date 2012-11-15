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

#include "../../gui/PropertyValueProxy.hpp"

namespace cadencii {

    class ConcretePropertyView;

    class ConcretePropertyValueProxy : public PropertyValueProxy {
    private:
        ConcretePropertyView *parent;

    public:
        ConcretePropertyValueProxy();

        void setParent(ConcretePropertyView *parent);

        void setLyricPhrase(const std::string &lyricPhrase);

        void setLyricPhoneticSymbol(const std::string &lyricPhoneticSymbol);

        void setLyricConsonantAdjustment(const std::string &lyricConsonantAdjustment);

        void setLyricProtect(int lyricProtect);

        void setNoteLength(const std::string &noteLength);

        void setNoteNumber(const std::string &noteNumber);

        void setNotelocationClock(const std::string &notelocationClock);

        void setNotelocationMeasure(const std::string &notelocationMeasure);

        void setNotelocationBeat(const std::string &notelocationBeat);

        void setNotelocationTick(const std::string &notelocationTick);

        void setVibratoType(int vibratoType);

        void setVibratoLength(const std::string &vibratoLength);
    };
}

#endif
