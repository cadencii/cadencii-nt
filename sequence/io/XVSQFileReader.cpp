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

    XVSQFileReader::XVSQFileReader() {
        insertEnumValueMap(dynamicsModeValueMap, VSQ_NS::DynamicsMode::STANDARD);
        insertEnumValueMap(dynamicsModeValueMap, VSQ_NS::DynamicsMode::EXPERT);

        insertEnumValueMap(playModeValueMap, VSQ_NS::PlayMode::OFF);
        insertEnumValueMap(playModeValueMap, VSQ_NS::PlayMode::PLAY_AFTER_SYNTH);
        insertEnumValueMap(playModeValueMap, VSQ_NS::PlayMode::PLAY_WITH_SYNTH);
    }

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
        if ("VsqTrack" == name) {
            trackCount++;
            currentTrack = VSQ_NS::Track("", "");
        }
        // TODO(kbinani):
        tagNameStack.push(name);
    }

    void XVSQFileReader::endElement(const std::string &name) {
        if ("VsqTrack" == name) {
            if (1 < trackCount) {
                // first track is a 'Master' track. so, just skip it.
                sequence->tracks()->push_back(currentTrack);
            }
        }
        // TODO(kbinani):
        tagNameStack.pop();
    }

    void XVSQFileReader::characters(const std::string &ch) {
        std::stack<std::string> stack = tagNameStack;

        std::string tagName = stack.top();
        stack.pop();
        std::string parentTagName = stack.top();
        if ("Common" == parentTagName) {
            charactersCommon(ch, tagName);
        }
        // TODO(kbinani):
    }

    void XVSQFileReader::charactersCommon(const std::string &ch, const std::string &tagName) {
        if ("Version" == tagName) {
            currentTrack.common()->version = ch;
        } else if ("Name" == tagName) {
            currentTrack.common()->name = ch;
        } else if ("Color" == tagName) {
            currentTrack.common()->color = ch;
        } else if ("DynamicsMode" == tagName) {
            if (dynamicsModeValueMap.find(ch) != dynamicsModeValueMap.end()) {
                currentTrack.common()->dynamicsMode = dynamicsModeValueMap.at(ch);
            }
        } else if ("PlayMode" == tagName) {
            if (playModeValueMap.find(ch) != playModeValueMap.end()) {
                currentTrack.common()->setPlayMode(currentTrack.common()->lastPlayMode());
                currentTrack.common()->setPlayMode(playModeValueMap.at(ch));
            }
        } else if ("LastPlayMode" == tagName) {
            if (playModeValueMap.find(ch) != playModeValueMap.end()) {
                VSQ_NS::PlayMode::PlayModeEnum current = currentTrack.common()->playMode();
                currentTrack.common()->setPlayMode(playModeValueMap.at(ch));
                currentTrack.common()->setPlayMode(current);
            }
        }
    }

    template<class T>
    void XVSQFileReader::insertEnumValueMap(std::map<std::string, T> &result, const T &enumValue) {
        result.insert(std::make_pair(StringUtil::toString((int)enumValue), enumValue));
    }
}
