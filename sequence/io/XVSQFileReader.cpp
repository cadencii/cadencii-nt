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

    XVSQFileReader::XVSQFileReader()
        : currentLyric(vsq::Lyric("", ""))
        , currentMixerItem(vsq::MixerItem(0, 0, 0, 0))
        , currentEvent(vsq::Event::eos())
        , currentBPList(vsq::BPList("", 0, 0, 127))
    {
        insertIntegerEnumValueMap(dynamicsModeValueMap, vsq::DynamicsMode::STANDARD);
        insertIntegerEnumValueMap(dynamicsModeValueMap, vsq::DynamicsMode::EXPERT);

        insertIntegerEnumValueMap(playModeValueMap, vsq::PlayMode::OFF);
        insertIntegerEnumValueMap(playModeValueMap, vsq::PlayMode::PLAY_AFTER_SYNTH);
        insertIntegerEnumValueMap(playModeValueMap, vsq::PlayMode::PLAY_WITH_SYNTH);

        insertStringEnumValueMap(eventTypeValueMap, vsq::EventType::ICON);
        insertStringEnumValueMap(eventTypeValueMap, vsq::EventType::NOTE);
        insertStringEnumValueMap(eventTypeValueMap, vsq::EventType::SINGER);

        boolValueMap.insert(std::make_pair("true", true));
        boolValueMap.insert(std::make_pair("false", false));

        tagNameMap.insert(std::make_pair("PIT", "PIT"));
        tagNameMap.insert(std::make_pair("PBS", "PBS"));
        tagNameMap.insert(std::make_pair("DYN", "DYN"));
        tagNameMap.insert(std::make_pair("BRE", "BRE"));
        tagNameMap.insert(std::make_pair("BRI", "BRI"));
        tagNameMap.insert(std::make_pair("CLE", "CLE"));
        tagNameMap.insert(std::make_pair("GEN", "GEN"));
        tagNameMap.insert(std::make_pair("POR", "POR"));
        tagNameMap.insert(std::make_pair("OPE", "OPE"));
        tagNameMap.insert(std::make_pair("harmonics", "harmonics"));
        tagNameMap.insert(std::make_pair("fx2depth", "fx2depth"));
        tagNameMap.insert(std::make_pair("reso1FreqBPList", "reso1freq"));
        tagNameMap.insert(std::make_pair("reso2FreqBPList", "reso2freq"));
        tagNameMap.insert(std::make_pair("reso3FreqBPList", "reso3freq"));
        tagNameMap.insert(std::make_pair("reso4FreqBPList", "reso4freq"));
        tagNameMap.insert(std::make_pair("reso1BWBPList", "reso1bw"));
        tagNameMap.insert(std::make_pair("reso2BWBPList", "reso2bw"));
        tagNameMap.insert(std::make_pair("reso3BWBPList", "reso3bw"));
        tagNameMap.insert(std::make_pair("reso4BWBPList", "reso4bw"));
        tagNameMap.insert(std::make_pair("reso1AmpBPList", "reso1amp"));
        tagNameMap.insert(std::make_pair("reso2AmpBPList", "reso2amp"));
        tagNameMap.insert(std::make_pair("reso3AmpBPList", "reso3amp"));
        tagNameMap.insert(std::make_pair("reso4AmpBPList", "reso4amp"));
    }

    /**
     * @brief Read XVSQ from specified SAX(Simple API for XML) adapter.
     */
    void XVSQFileReader::read(vsq::Sequence *sequence, SAXAdapter *adapter) {
        adapter->setReader(this);
        this->sequence = sequence;
        this->sequence->tracks().clear();
        trackCount = 0;
        adapter->start();

        validate();
    }

    void XVSQFileReader::startElement(const std::string &name) {
        tagNameStack.push(name);
        if ("VsqTrack" == name) {
            trackCount++;
            currentTrack = vsq::Track("", "");
            currentTrack.events().clear();
        } else if ("VsqEvent" == name) {
            currentEvent = vsq::Event::eos();
        } else if ("IconHandle" == name) {
            currentHandle = vsq::Handle(vsq::HandleType::SINGER);
        } else if ("LyricHandle" == name) {
            currentHandle = vsq::Handle(vsq::HandleType::LYRIC);
        } else if ("L0" == name || ("Trailing" == getParentTag() && "Lyric" == name)) {
            currentLyric = vsq::Lyric("", "");
        } else if ("VibratoHandle" == name) {
            currentHandle = vsq::Handle(vsq::HandleType::VIBRATO);
        } else if ("NoteHeadHandle" == name) {
            currentHandle = vsq::Handle(vsq::HandleType::NOTE_HEAD);
        } else if ("IconDynamicsHandle" == name) {
            currentHandle = vsq::Handle(vsq::HandleType::DYNAMICS);
        } else if (isControlCurveTagName(name)) {
            std::string curveName = getCurveNameFrom(name);
            currentBPList = *defaultTrack.curve(curveName);
        } else if ("TempoTable" == name) {
            sequence->tempoList.clear();
        } else if ("TempoTableEntry" == name) {
            currentTempo = vsq::Tempo();
        } else if ("TimesigTable" == name) {
            sequence->timesigList.clear();
        } else if ("TimeSigTableEntry" == name) {
            currentTimesig = vsq::Timesig();
        } else if ("Slave" == name) {
            sequence->mixer.slave.clear();
        } else if ("VsqMixerEntry" == name) {
            currentMixerItem = vsq::MixerItem(0, 0, 0, 0);
        }
    }

    void XVSQFileReader::endElement(const std::string &name) {
        if ("VsqTrack" == name) {
            if (1 < trackCount) {
                // first track is a 'Master' track. so, just skip it.
                sequence->tracks().push_back(currentTrack);
            }
        } else if ("VsqEvent" == name) {
            if (currentEvent.vibratoHandle.type() == vsq::HandleType::VIBRATO) {
                int length = currentEvent.length();
                int vibratoLength = length - currentEvent.vibratoDelay;
                currentEvent.vibratoHandle.length(vibratoLength * 100 / length);
            }
            currentTrack.events().add(currentEvent, currentEvent.id);
        } else if ("L0" == name || ("Trailing" == getParentTag() && "Lyric" == name)) {
            currentHandle.add(currentLyric);
        } else if ("IconHandle" == name) {
            currentEvent.singerHandle = currentHandle;
        } else if ("LyricHandle" == name) {
            currentEvent.lyricHandle = currentHandle;
        } else if ("VibratoHandle" == name) {
            currentEvent.vibratoHandle = currentHandle;
        } else if ("NoteHeadHandle" == name) {
            currentEvent.noteHeadHandle = currentHandle;
        } else if ("IconDynamicsHandle" == name) {
            currentEvent.iconDynamicsHandle = currentHandle;
        } else if (isControlCurveTagName(name)) {
            std::string curveName = getCurveNameFrom(name);
            *currentTrack.curve(curveName) = currentBPList;
        } else if ("TempoTable" == name) {
            sequence->tempoList.updateTempoInfo();
        } else if ("TempoTableEntry" == name) {
            sequence->tempoList.push(currentTempo);
        } else if ("TimeSigTableEntry" == name) {
            sequence->timesigList.push(currentTimesig);
        } else if ("VsqMixerEntry" == name) {
            sequence->mixer.slave.push_back(currentMixerItem);
        }
        tagNameStack.pop();
    }

    void XVSQFileReader::characters(const std::string &ch) {
        std::string tagName = tagNameStack.top();
        std::string parentTagName = getParentTag();
        std::string grandParentTag = getParentTag(1);

        if ("Common" == parentTagName) {
            charactersCommon(ch, tagName);
        } else if ("VsqEvent" == parentTagName) {
            charactersVsqEvent(ch, tagName);
        } else if ("ID" == parentTagName) {
            charactersID(ch, tagName);
        } else if ("IconHandle" == parentTagName
                   || "VibratoHandle" == parentTagName
                   || "NoteHeadHandle" == parentTagName
                   || "IconDynamicsHandle" == parentTagName) {
            charactersHandle(ch, tagName);
        } else if ("L0" == parentTagName
                   || ("Lyric" == parentTagName && "Trailing" == grandParentTag)) {
            charactersLyric(ch, tagName);
        } else if ("Data" == tagName) {
            if ("RateBP" == parentTagName) {
                currentHandle.rateBP.data(ch);
            } else if ("DepthBP" == parentTagName) {
                currentHandle.depthBP.data(ch);
            } else if ("DynBP" == parentTagName) {
                currentHandle.dynBP.data(ch);
            } else if (isControlCurveTagName(parentTagName)) {
                currentBPList.data(ch);
            }
        } else if (isControlCurveTagName(parentTagName)) {
            charactersBPList(ch, tagName);
        } else if ("TempoTableEntry" == parentTagName) {
            charactersTempo(ch, tagName);
        } else if ("TimeSigTableEntry" == parentTagName) {
            charactersTimesig(ch, tagName);
        } else if ("Master" == parentTagName) {
            charactersMaster(ch, tagName);
        } else if ("Mixer" == parentTagName) {
            charactersMixer(ch, tagName);
        } else if ("VsqMixerEntry" == parentTagName) {
            charactersMixerItem(ch, tagName);
        }
    }

    void XVSQFileReader::charactersMixerItem(std::string const& ch, std::string const& tagName) {
        if ("Solo" == tagName) {
            currentMixerItem.solo = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Feder" == tagName) {
            currentMixerItem.feder = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Mute" == tagName) {
            currentMixerItem.mute = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Panpot" == tagName) {
            currentMixerItem.panpot = vsq::StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersMixer(const std::string &ch, const std::string &tagName) {
        if ("MasterFeder" == tagName) {
            sequence->mixer.masterFeder = vsq::StringUtil::parseInt<int>(ch);
        } else if ("MasterPanpot" == tagName) {
            sequence->mixer.masterPanpot = vsq::StringUtil::parseInt<int>(ch);
        } else if ("MasterMute" == tagName) {
            sequence->mixer.masterMute = vsq::StringUtil::parseInt<int>(ch);
        } else if ("OutputMode" == tagName) {
            sequence->mixer.outputMode = vsq::StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersMaster(const std::string &ch, const std::string &tagName) {
        if ("PreMeasure" == tagName) {
            sequence->master.preMeasure = vsq::StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersTimesig(const std::string &ch, const std::string &tagName) {
        if ("Numerator" == tagName) {
            currentTimesig.numerator = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Denominator" == tagName) {
            currentTimesig.denominator = vsq::StringUtil::parseInt<int>(ch);
        } else if ("BarCount" == tagName) {
            currentTimesig.barCount = vsq::StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersTempo(const std::string &ch, const std::string &tagName) {
        if ("Clock" == tagName) {
            currentTempo.tick = vsq::StringUtil::parseInt<vsq::tick_t>(ch);
        } else if ("Tempo" == tagName) {
            currentTempo.tempo = vsq::StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersBPList(const std::string &ch, const std::string &tagName) {
        if ("Default" == tagName) {
            currentBPList.defaultValue(vsq::StringUtil::parseInt<int>(ch));
        } else if ("Name" == tagName) {
            currentBPList.name(ch);
        } else if ("Maximum" == tagName) {
            currentBPList.maximum(vsq::StringUtil::parseInt<int>(ch));
        } else if ("Minimum" == tagName) {
            currentBPList.minimum(vsq::StringUtil::parseInt<int>(ch));
        }
    }

    void XVSQFileReader::charactersHandle(const std::string &ch, const std::string &tagName) {
        if ("IconID" == tagName) {
            currentHandle.iconId = ch;
        } else if ("IDS" == tagName) {
            currentHandle.ids = ch;
        } else if ("Original" == tagName) {
            currentHandle.original = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Caption" == tagName) {
            currentHandle.caption = ch;
        } else if ("Length" == tagName) {
            currentHandle.length(vsq::StringUtil::parseInt<vsq::tick_t>(ch));
        } else if ("StartDyn" == tagName) {
            currentHandle.startDyn = vsq::StringUtil::parseInt<int>(ch);
        } else if ("EndDyn" == tagName) {
            currentHandle.endDyn = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Depth" == tagName) {
            currentHandle.depth = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Duration" == tagName) {
            currentHandle.duration = vsq::StringUtil::parseInt<int>(ch);
        } else if ("StartRate" == tagName) {
            currentHandle.startRate = vsq::StringUtil::parseInt<int>(ch);
        } else if ("StartDepth" == tagName) {
            currentHandle.startDepth = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Index" == tagName) {
            currentHandle.index = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Program" == tagName) {
            currentHandle.program = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Language" == tagName) {
            currentHandle.language = vsq::StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersLyric(const std::string &ch, const std::string &tagName) {
        if ("Phrase" == tagName) {
            currentLyric.phrase = ch;
        } else if ("UnknownFloat" == tagName) {
            currentLyric.lengthRatio = vsq::StringUtil::parseFloat<double>(ch);
        } else if ("PhoneticSymbolProtected" == tagName) {
            if (boolValueMap.find(ch) != boolValueMap.end()) {
                currentLyric.isProtected = boolValueMap.at(ch);
            }
        } else if ("ConsonantAdjustment" == tagName) {
            currentLyric.consonantAdjustment(vsq::StringUtil::replace(ch, " ", ","));
        } else if ("PhoneticSymbol" == tagName) {
            currentLyric.phoneticSymbol(ch);
        }
    }

    void XVSQFileReader::charactersID(const std::string &ch, const std::string &tagName) {
        if ("type" == tagName) {
            if (eventTypeValueMap.find(ch) != eventTypeValueMap.end()) {
                currentEvent.type(eventTypeValueMap.at(ch));
            }
        } else if ("Note" == tagName) {
            currentEvent.note = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Dynamics" == tagName) {
            currentEvent.dynamics = vsq::StringUtil::parseInt<int>(ch);
        } else if ("PMBendDepth" == tagName) {
            currentEvent.pmBendDepth = vsq::StringUtil::parseInt<int>(ch);
        } else if ("PMBendLength" == tagName) {
            currentEvent.pmBendLength = vsq::StringUtil::parseInt<int>(ch);
        } else if ("PMbPortamentoUse" == tagName) {
            currentEvent.pmbPortamentoUse = vsq::StringUtil::parseInt<int>(ch);
        } else if ("DEMdecGainRate" == tagName) {
            currentEvent.demDecGainRate = vsq::StringUtil::parseInt<int>(ch);
        } else if ("DEMaccent" == tagName) {
            currentEvent.demAccent = vsq::StringUtil::parseInt<int>(ch);
        } else if ("VibratoDelay" == tagName) {
            currentEvent.vibratoDelay = vsq::StringUtil::parseInt<int>(ch);
        } else if ("pMeanOnsetFirstNote" == tagName) {
            currentEvent.pMeanOnsetFirstNote = vsq::StringUtil::parseInt<int>(ch);
        } else if ("vMeanNoteTransition" == tagName) {
            currentEvent.vMeanNoteTransition = vsq::StringUtil::parseInt<int>(ch);
        } else if ("d4mean" == tagName) {
            currentEvent.d4mean = vsq::StringUtil::parseInt<int>(ch);
        } else if ("pMeanEndingNote" == tagName) {
            currentEvent.pMeanEndingNote = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Length" == tagName) {
            currentEvent.length(vsq::StringUtil::parseInt<vsq::tick_t>(ch));
        }
    }

    void XVSQFileReader::charactersVsqEvent(const std::string &ch, const std::string &tagName) {
        if ("InternalID" == tagName) {
            currentEvent.id = vsq::StringUtil::parseInt<int>(ch);
        } else if ("Clock" == tagName) {
            currentEvent.tick = vsq::StringUtil::parseInt<vsq::tick_t>(ch);
        }
    }

    void XVSQFileReader::charactersCommon(const std::string &ch, const std::string &tagName) {
        if ("Version" == tagName) {
            currentTrack.common().version = ch;
        } else if ("Name" == tagName) {
            currentTrack.common().name = ch;
        } else if ("Color" == tagName) {
            currentTrack.common().color = ch;
        } else if ("DynamicsMode" == tagName) {
            if (dynamicsModeValueMap.find(ch) != dynamicsModeValueMap.end()) {
                currentTrack.common().dynamicsMode = dynamicsModeValueMap.at(ch);
            }
        } else if ("PlayMode" == tagName) {
            if (playModeValueMap.find(ch) != playModeValueMap.end()) {
                currentTrack.common().playMode(currentTrack.common().lastPlayMode());
                currentTrack.common().playMode(playModeValueMap.at(ch));
            }
        } else if ("LastPlayMode" == tagName) {
            if (playModeValueMap.find(ch) != playModeValueMap.end()) {
                vsq::PlayMode current = currentTrack.common().playMode();
                currentTrack.common().playMode(playModeValueMap.at(ch));
                currentTrack.common().playMode(current);
            }
        }
    }

    void XVSQFileReader::validate() {
        if (sequence->tracks().size() < 1) {
            throw XVSQFileReader::ParseException();
        }
    }

    template<class T>
    void XVSQFileReader::insertIntegerEnumValueMap(
        std::map<std::string, T> &result, const T &enumValue
    ) {
        result.insert(std::make_pair(vsq::StringUtil::toString(static_cast<int>(enumValue)), enumValue));
    }

    void XVSQFileReader::insertStringEnumValueMap(std::map<std::string, vsq::EventType> &result, vsq::EventType type) {
        result.insert(std::make_pair(vsq::EventTypeUtil::toString(type), type));
    }
}
