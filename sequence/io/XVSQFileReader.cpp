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
#include <map>
#include <stack>
#include "XVSQFileReader.hpp"
#include "SAXAdapter.hpp"

namespace cadencii {

    XVSQFileReader::XVSQFileReader() {
        insertIntegerEnumValueMap(dynamicsModeValueMap, VSQ_NS::DynamicsMode::STANDARD);
        insertIntegerEnumValueMap(dynamicsModeValueMap, VSQ_NS::DynamicsMode::EXPERT);

        insertIntegerEnumValueMap(playModeValueMap, VSQ_NS::PlayMode::OFF);
        insertIntegerEnumValueMap(playModeValueMap, VSQ_NS::PlayMode::PLAY_AFTER_SYNTH);
        insertIntegerEnumValueMap(playModeValueMap, VSQ_NS::PlayMode::PLAY_WITH_SYNTH);

        insertStringEnumValueMap<VSQ_NS::EventType, VSQ_NS::EventType::EventTypeEnum>(
                    eventTypeValueMap, VSQ_NS::EventType::ICON);
        insertStringEnumValueMap<VSQ_NS::EventType, VSQ_NS::EventType::EventTypeEnum>(
                    eventTypeValueMap, VSQ_NS::EventType::NOTE);
        insertStringEnumValueMap<VSQ_NS::EventType, VSQ_NS::EventType::EventTypeEnum>(
                    eventTypeValueMap, VSQ_NS::EventType::SINGER);
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
            currentTrack.events()->clear();
        } else if ("VsqEvent" == name) {
            currentEvent = VSQ_NS::Event();
        } else if ("IconHandle" == name) {
            currentEvent.singerHandle = VSQ_NS::Handle(VSQ_NS::HandleType::SINGER);
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
        } else if ("VsqEvent" == name) {
            currentTrack.events()->add(currentEvent, currentEvent.id);
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
        } else if ("VsqEvent" == parentTagName) {
            charactersVsqEvent(ch, tagName);
        } else if ("ID" == parentTagName) {
            charactersID(ch, tagName);
        } else if ("IconHandle" == parentTagName) {
            charactersIconHandle(ch, tagName);
        }
        // TODO(kbinani):
    }

    void XVSQFileReader::charactersIconHandle(const std::string &ch, const std::string &tagName) {
        if ("Caption" == tagName) {
            currentEvent.singerHandle.caption = ch;
        } else if ("IconID" == tagName) {
            currentEvent.singerHandle.iconId = ch;
        } else if ("IDS" == tagName) {
            currentEvent.singerHandle.ids = ch;
        } else if ("Index" == tagName) {
            currentEvent.singerHandle.index = StringUtil::parseInt<int>(ch);
        } else if ("Length" == tagName) {
            currentEvent.singerHandle.setLength(StringUtil::parseInt<VSQ_NS::tick_t>(ch));
        } else if ("Original" == tagName) {
            currentEvent.singerHandle.original = StringUtil::parseInt<int>(ch);
        } else if ("Program" == tagName) {
            currentEvent.singerHandle.program = StringUtil::parseInt<int>(ch);
        } else if ("Language" == tagName) {
            currentEvent.singerHandle.language = StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersID(const std::string &ch, const std::string &tagName) {
        if ("type" == tagName) {
            if (eventTypeValueMap.find(ch) != eventTypeValueMap.end()) {
                currentEvent.type = eventTypeValueMap.at(ch);
            }
        } else if ("Note" == tagName) {
            currentEvent.note = StringUtil::parseInt<int>(ch);
        } else if ("Dynamics" == tagName) {
            currentEvent.dynamics = StringUtil::parseInt<int>(ch);
        } else if ("PMBendDepth" == tagName) {
            currentEvent.pmBendDepth = StringUtil::parseInt<int>(ch);
        } else if ("PMBendLength" == tagName) {
            currentEvent.pmBendLength = StringUtil::parseInt<int>(ch);
        } else if ("PMbPortamentoUse" == tagName) {
            currentEvent.pmbPortamentoUse = StringUtil::parseInt<int>(ch);
        } else if ("DEMdecGainRate" == tagName) {
            currentEvent.demDecGainRate = StringUtil::parseInt<int>(ch);
        } else if ("DEMaccent" == tagName) {
            currentEvent.demAccent = StringUtil::parseInt<int>(ch);
        } else if ("VibratoDelay" == tagName) {
            currentEvent.vibratoDelay = StringUtil::parseInt<int>(ch);
        } else if ("pMeanOnsetFirstNote" == tagName) {
            currentEvent.pMeanOnsetFirstNote = StringUtil::parseInt<int>(ch);
        } else if ("vMeanNoteTransition" == tagName) {
            currentEvent.vMeanNoteTransition = StringUtil::parseInt<int>(ch);
        } else if ("d4mean" == tagName) {
            currentEvent.d4mean = StringUtil::parseInt<int>(ch);
        } else if ("pMeanEndingNote" == tagName) {
            currentEvent.pMeanEndingNote = StringUtil::parseInt<int>(ch);
        } else if ("Length" == tagName) {
            currentEvent.setLength(StringUtil::parseInt<VSQ_NS::tick_t>(ch));
        }
    }

    void XVSQFileReader::charactersVsqEvent(const std::string &ch, const std::string &tagName) {
        if ("InternalID" == tagName) {
            currentEvent.id = StringUtil::parseInt<int>(ch);
        } else if ("Clock" == tagName) {
            currentEvent.clock = StringUtil::parseInt<VSQ_NS::tick_t>(ch);
        }
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
    void XVSQFileReader::insertIntegerEnumValueMap(
        std::map<std::string, T> &result, const T &enumValue
    ) {
        result.insert(std::make_pair(StringUtil::toString(static_cast<int>(enumValue)), enumValue));
    }

    template<class classT, class enumT>
    void XVSQFileReader::insertStringEnumValueMap(
        std::map<std::string, enumT> &result, const enumT &enumValue
    ) {
        result.insert(std::make_pair(classT::toString(enumValue), enumValue));
    }
}
