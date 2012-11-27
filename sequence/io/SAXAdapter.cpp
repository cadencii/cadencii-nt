/**
 * SAXAdapter.cpp
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
#include "SAXAdapter.hpp"
#include "XVSQFileReader.hpp"

namespace cadencii {

    SAXAdapter::SAXAdapter(VSQ_NS::InputStream *stream) :
        stream(stream), reader(0) {
    }

    SAXAdapter::SAXAdapter() {
    }

    SAXAdapter::~SAXAdapter() {
    }

    void SAXAdapter::onStartElement(const std::string &name) {
        reader->startElement(name);
    }

    void SAXAdapter::onEndElement(const string &name) {
        reader->endElement(name);
    }

    void SAXAdapter::onCharacters(const std::string &ch) {
        reader->characters(ch);
    }

    void SAXAdapter::setReader(XVSQFileReader *reader) {
        this->reader = reader;
    }

    void SAXAdapter::start() {
    }
}
