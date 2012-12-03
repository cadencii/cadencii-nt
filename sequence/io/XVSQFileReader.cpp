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

    XVSQFileReader::XVSQFileReader() :
        currentLyric(VSQ_NS::Lyric("", "")),
        currentMixerItem(VSQ_NS::MixerItem(0, 0, 0, 0)) {
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

        boolValueMap.insert(std::make_pair("true", true));
        boolValueMap.insert(std::make_pair("false", false));

        tagNameMap.insert(make_pair("PIT", "PIT"));
        tagNameMap.insert(make_pair("PBS", "PBS"));
        tagNameMap.insert(make_pair("DYN", "DYN"));
        tagNameMap.insert(make_pair("BRE", "BRE"));
        tagNameMap.insert(make_pair("BRI", "BRI"));
        tagNameMap.insert(make_pair("CLE", "CLE"));
        tagNameMap.insert(make_pair("GEN", "GEN"));
        tagNameMap.insert(make_pair("POR", "POR"));
        tagNameMap.insert(make_pair("OPE", "OPE"));
        tagNameMap.insert(make_pair("harmonics", "harmonics"));
        tagNameMap.insert(make_pair("fx2depth", "fx2depth"));
        tagNameMap.insert(make_pair("reso1FreqBPList", "reso1freq"));
        tagNameMap.insert(make_pair("reso2FreqBPList", "reso2freq"));
        tagNameMap.insert(make_pair("reso3FreqBPList", "reso3freq"));
        tagNameMap.insert(make_pair("reso4FreqBPList", "reso4freq"));
        tagNameMap.insert(make_pair("reso1BWBPList", "reso1bw"));
        tagNameMap.insert(make_pair("reso2BWBPList", "reso2bw"));
        tagNameMap.insert(make_pair("reso3BWBPList", "reso3bw"));
        tagNameMap.insert(make_pair("reso4BWBPList", "reso4bw"));
        tagNameMap.insert(make_pair("reso1AmpBPList", "reso1amp"));
        tagNameMap.insert(make_pair("reso2AmpBPList", "reso2amp"));
        tagNameMap.insert(make_pair("reso3AmpBPList", "reso3amp"));
        tagNameMap.insert(make_pair("reso4AmpBPList", "reso4amp"));
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
        tagNameStack.push(name);
        if ("VsqTrack" == name) {
            trackCount++;
            currentTrack = VSQ_NS::Track("", "");
            currentTrack.events()->clear();
        } else if ("VsqEvent" == name) {
            currentEvent = VSQ_NS::Event();
        } else if ("IconHandle" == name) {
            currentHandle = VSQ_NS::Handle(VSQ_NS::HandleType::SINGER);
        } else if ("LyricHandle" == name) {
            currentHandle = VSQ_NS::Handle(VSQ_NS::HandleType::LYRIC);
        } else if ("L0" == name || ("Trailing" == getParentTag() && "Lyric" == name)) {
            currentLyric = VSQ_NS::Lyric("", "");
        } else if ("VibratoHandle" == name) {
            currentHandle = VSQ_NS::Handle(VSQ_NS::HandleType::VIBRATO);
        } else if ("NoteHeadHandle" == name) {
            currentHandle = VSQ_NS::Handle(VSQ_NS::HandleType::NOTE_HEAD);
        } else if ("IconDynamicsHandle" == name) {
            currentHandle = VSQ_NS::Handle(VSQ_NS::HandleType::DYNAMICS);
        } else if (isControlCurveTagName(name)) {
            std::string curveName = getCurveNameFrom(name);
            currentBPList = *defaultTrack.curve(curveName);
        } else if ("TempoTable" == name) {
            sequence->tempoList.clear();
        } else if ("TempoTableEntry" == name) {
            currentTempo = VSQ_NS::Tempo();
        } else if ("TimesigTable" == name) {
            sequence->timesigList.clear();
        } else if ("TimeSigTableEntry" == name) {
            currentTimesig = VSQ_NS::Timesig();
        } else if ("Slave" == name) {
            sequence->mixer.slave.clear();
        } else if ("VsqMixerEntry" == name) {
            currentMixerItem = VSQ_NS::MixerItem(0, 0, 0, 0);
        }
        // TODO(kbinani):
    }

    void XVSQFileReader::endElement(const std::string &name) {
        if ("VsqTrack" == name) {
            if (1 < trackCount) {
                // first track is a 'Master' track. so, just skip it.
                sequence->tracks()->push_back(currentTrack);
            }
        } else if ("VsqEvent" == name) {
            if (currentEvent.vibratoHandle.getHandleType() == VSQ_NS::HandleType::VIBRATO) {
                int length = currentEvent.getLength();
                int vibratoLength = length - currentEvent.vibratoDelay;
                currentEvent.vibratoHandle.setLength(vibratoLength * 100 / length);
            }
            currentTrack.events()->add(currentEvent, currentEvent.id);
        } else if ("L0" == name || ("Trailing" == getParentTag() && "Lyric" == name)) {
            currentHandle.addLyric(currentLyric);
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
        // TODO(kbinani):
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
                currentHandle.rateBP.setData(ch);
            } else if ("DepthBP" == parentTagName) {
                currentHandle.depthBP.setData(ch);
            } else if ("DynBP" == parentTagName) {
                currentHandle.dynBP.setData(ch);
            } else if (isControlCurveTagName(parentTagName)) {
                currentBPList.setData(ch);
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
        // TODO(kbinani):
    }

    void XVSQFileReader::charactersMixerItem(const string &ch, const string &tagName) {
        if ("Solo" == tagName) {
            currentMixerItem.solo = StringUtil::parseInt<int>(ch);
        } else if ("Feder" == tagName) {
            currentMixerItem.feder = StringUtil::parseInt<int>(ch);
        } else if ("Mute" == tagName) {
            currentMixerItem.mute = StringUtil::parseInt<int>(ch);
        } else if ("Panpot" == tagName) {
            currentMixerItem.panpot = StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersMixer(const string &ch, const string &tagName) {
        if ("MasterFeder" == tagName) {
            sequence->mixer.masterFeder = StringUtil::parseInt<int>(ch);
        } else if ("MasterPanpot" == tagName) {
            sequence->mixer.masterPanpot = StringUtil::parseInt<int>(ch);
        } else if ("MasterMute" == tagName) {
            sequence->mixer.masterMute = StringUtil::parseInt<int>(ch);
        } else if ("OutputMode" == tagName) {
            sequence->mixer.outputMode = StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersMaster(const string &ch, const string &tagName) {
        if ("PreMeasure" == tagName) {
            sequence->master.preMeasure = StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersTimesig(const string &ch, const string &tagName) {
        if ("Numerator" == tagName) {
            currentTimesig.numerator = StringUtil::parseInt<int>(ch);
        } else if ("Denominator" == tagName) {
            currentTimesig.denominator = StringUtil::parseInt<int>(ch);
        } else if ("BarCount" == tagName) {
            currentTimesig.barCount = StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersTempo(const string &ch, const string &tagName) {
        if ("Clock" == tagName) {
            currentTempo.clock = StringUtil::parseInt<VSQ_NS::tick_t>(ch);
        } else if ("Tempo" == tagName) {
            currentTempo.tempo = StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersBPList(const string &ch, const string &tagName) {
        if ("Default" == tagName) {
            currentBPList.setDefault(StringUtil::parseInt<int>(ch));
        } else if ("Name" == tagName) {
            currentBPList.setName(ch);
        } else if ("Maximum" == tagName) {
            currentBPList.setMaximum(StringUtil::parseInt<int>(ch));
        } else if ("Minimum" == tagName) {
            currentBPList.setMinimum(StringUtil::parseInt<int>(ch));
        }
    }

    void XVSQFileReader::charactersHandle(const string &ch, const string &tagName) {
        if ("IconID" == tagName) {
            currentHandle.iconId = ch;
        } else if ("IDS" == tagName) {
            currentHandle.ids = ch;
        } else if ("Original" == tagName) {
            currentHandle.original = StringUtil::parseInt<int>(ch);
        } else if ("Caption" == tagName) {
            currentHandle.caption = ch;
        } else if ("Length" == tagName) {
            currentHandle.setLength(StringUtil::parseInt<VSQ_NS::tick_t>(ch));
        } else if ("StartDyn" == tagName) {
            currentHandle.startDyn = StringUtil::parseInt<int>(ch);
        } else if ("EndDyn" == tagName) {
            currentHandle.endDyn = StringUtil::parseInt<int>(ch);
        } else if ("Depth" == tagName) {
            currentHandle.depth = StringUtil::parseInt<int>(ch);
        } else if ("Duration" == tagName) {
            currentHandle.duration = StringUtil::parseInt<int>(ch);
        } else if ("StartRate" == tagName) {
            currentHandle.startRate = StringUtil::parseInt<int>(ch);
        } else if ("StartDepth" == tagName) {
            currentHandle.startDepth = StringUtil::parseInt<int>(ch);
        } else if ("Index" == tagName) {
            currentHandle.index = StringUtil::parseInt<int>(ch);
        } else if ("Program" == tagName) {
            currentHandle.program = StringUtil::parseInt<int>(ch);
        } else if ("Language" == tagName) {
            currentHandle.language = StringUtil::parseInt<int>(ch);
        }
    }

    void XVSQFileReader::charactersLyric(const string &ch, const string &tagName) {
        if ("Phrase" == tagName) {
            currentLyric.phrase = ch;
        } else if ("UnknownFloat" == tagName) {
            currentLyric.lengthRatio = StringUtil::parseFloat<double>(ch);
        } else if ("PhoneticSymbolProtected" == tagName) {
            if (boolValueMap.find(ch) != boolValueMap.end()) {
                currentLyric.isProtected = boolValueMap.at(ch);
            }
        } else if ("ConsonantAdjustment" == tagName) {
            currentLyric.setConsonantAdjustment(StringUtil::replace(ch, " ", ","));
        } else if ("PhoneticSymbol" == tagName) {
            currentLyric.setPhoneticSymbol(ch);
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
