/**
 * XVSQFileReader.cpp
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
#include "XVSQFileReader.hpp"
#include "SAXAdapter.hpp"

namespace cadencii {

    /**
     * @brief Read XVSQ from specified SAX(Simple API for XML) adapter.
     */
    void XVSQFileReader::read(VSQ_NS::Sequence *sequence, SAXAdapter *adapter) {
        adapter->setReader(this);
        this->sequence = sequence;
        this->sequence->tracks()->clear();
        trackCount = 0;
        adapter->start();
    }

    void XVSQFileReader::startElement(const std::string &name) {
        if (name == "VsqTrack") {
            trackCount++;
            currentTrack = VSQ_NS::Track("", "");
        }
        // TODO(kbinani):
    }

    void XVSQFileReader::endElement(const std::string &name) {
        if (name == "VsqTrack") {
            if (1 < trackCount) {
                // first track is a 'Master' track. so, just skip it.
                sequence->tracks()->push_back(currentTrack);
            }
        }
        // TODO(kbinani):
    }

    void XVSQFileReader::characters(const string &ch) {
        // TODO(kbinani):
    }
}
