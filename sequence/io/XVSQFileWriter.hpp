/**
 * XVSQFileWriter.hpp
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
#ifndef CADENCII_SEQUENCE_IO_XVSQFILEWRITER_HPP_
#define CADENCII_SEQUENCE_IO_XVSQFILEWRITER_HPP_

#include <vector>
#include <string>
#include <libvsq/libvsq.h>
#include "XVSQSpec.hpp"

namespace cadencii {

    /**
     * @brief A writer for XVSQ file.
     * @todo  Write <AttachedCurves> element.
     * @todo  Write <BgmFiles> element.
     * @todo  Write <cacheDir> element.
     * @todo  Write <config> element.
     */
    class XVSQFileWriter {
    private:
        vsq::StreamWriter *writer;

    public:
        explicit XVSQFileWriter() : writer(0) {
        }

        /**
         * @brief Output a sequence into stream.
         * @param  sequence  A sequence to be output.
         * @param  stream    A stream.
         */
        void write(const vsq::Sequence *sequence, vsq::OutputStream *stream) {
            writer = new vsq::StreamWriter(stream);
            writeHeader(writer);
            writeTracks(sequence, writer);
            writeTempoTable(sequence->tempoList, writer);
            writeTimesigTable(sequence->timesigList, writer);
            writeTotalClocks(sequence, writer);
            writeMaster(sequence->master, writer);
            writeMixer(sequence->mixer, writer);
            writeFooter(writer);
            writer->close();
        }

    protected:
        void writeMixer(const vsq::Mixer &mixer, vsq::StreamWriter *writer) {
            writer->writeLine("<Mixer>");
            writer->writeLine("<MasterFeder>"
                              + vsq::StringUtil::toString(mixer.masterFeder)
                              + "</MasterFeder>");
            writer->writeLine("<MasterPanpot>"
                              + vsq::StringUtil::toString(mixer.masterPanpot)
                              + "</MasterPanpot>");
            writer->writeLine("<MasterMute>"
                              + vsq::StringUtil::toString(mixer.masterMute)
                              + "</MasterMute>");
            writer->writeLine("<OutputMode>"
                              + vsq::StringUtil::toString(mixer.outputMode)
                              + "</OutputMode>");
            writer->writeLine("<Slave>");
            for (int i = 0; i < mixer.slave.size(); i++) {
                const vsq::MixerItem item = mixer.slave[i];
                writer->writeLine("<VsqMixerEntry>");
                writer->writeLine("<Feder>" + vsq::StringUtil::toString(item.feder) + "</Feder>");
                writer->writeLine("<Panpot>" + vsq::StringUtil::toString(item.panpot) + "</Panpot>");
                writer->writeLine("<Mute>" + vsq::StringUtil::toString(item.mute) + "</Mute>");
                writer->writeLine("<Solo>" + vsq::StringUtil::toString(item.solo) + "</Solo>");
                writer->writeLine("</VsqMixerEntry>");
            }
            writer->writeLine("</Slave>");
            writer->writeLine("</Mixer>");
        }

        void writeMaster(const vsq::Master &master, vsq::StreamWriter *writer) {
            writer->writeLine("<Master>");
            writer->writeLine("<PreMeasure>"
                              + vsq::StringUtil::toString(master.preMeasure)
                              + "</PreMeasure>");
            writer->writeLine("</Master>");
        }

        void writeTimesigTable(const vsq::TimesigList &timesigList,
                               vsq::StreamWriter *writer) {
            writer->writeLine("<TimesigTable>");
            for (int i = 0; i < timesigList.size(); i++) {
                vsq::Timesig timesig = timesigList.get(i);
                writer->writeLine("<TimeSigTableEntry>");
                writer->writeLine("<Clock>"
                                  + vsq::StringUtil::toString(timesig.tick())
                                  + "</Clock>");
                writer->writeLine("<Numerator>"
                                  + vsq::StringUtil::toString(timesig.numerator)
                                  + "</Numerator>");
                writer->writeLine("<Denominator>"
                                  + vsq::StringUtil::toString(timesig.denominator)
                                  + "</Denominator>");
                writer->writeLine("<BarCount>"
                                  + vsq::StringUtil::toString(timesig.barCount)
                                  + "</BarCount>");
                writer->writeLine("</TimeSigTableEntry>");
            }
            writer->writeLine("</TimesigTable>");
        }

        void writeTempoTable(const vsq::TempoList &tempoList, vsq::StreamWriter *writer) {
            vsq::TempoList copy = tempoList;
            copy.updateTempoInfo();
            writer->writeLine("<TempoTable>");
            for (int i = 0; i < copy.size(); i++) {
                vsq::Tempo tempo = copy.get(i);
                writer->writeLine("<TempoTableEntry>");
                writer->writeLine("<Clock>" + vsq::StringUtil::toString(tempo.tick) + "</Clock>");
                writer->writeLine("<Tempo>" + vsq::StringUtil::toString(tempo.tempo) + "</Tempo>");
                writer->writeLine("<Time>"
                                  + vsq::StringUtil::toString(tempo.time(), "%g")
                                  + "</Time>");
                writer->writeLine("</TempoTableEntry>");
            }
            writer->writeLine("</TempoTable>");
        }

        void writeBPList(const vsq::BPList *list, vsq::StreamWriter *writer) {
            XVSQSpec spec;
            std::string elementName = spec.getElementName(list->name());
            if (elementName.empty()) return;

            writer->writeLine("<" + elementName + ">");
            writer->writeLine("<Default>"
                              + vsq::StringUtil::toString(list->defaultValue())
                              + "</Default>");
            writer->writeLine("<Name>" + list->name() + "</Name>");
            writer->writeLine("<Data>" + list->data() + "</Data>");
            writer->writeLine("<Maximum>"
                              + vsq::StringUtil::toString(list->maximum())
                              + "</Maximum>");
            writer->writeLine("<Minimum>"
                              + vsq::StringUtil::toString(list->minimum())
                              + "</Minimum>");
            writer->writeLine("</" + elementName + ">");
        }

        void writeCommon(vsq::Common const& common, vsq::StreamWriter *writer) {
            writer->writeLine("<Version>" + common.version + "</Version>");
            writer->writeLine("<Name>" + common.name + "</Name>");
            writer->writeLine("<Color>" + common.color + "</Color>");
            writer->writeLine("<DynamicsMode>"
                              + vsq::StringUtil::toString(static_cast<int>(common.dynamicsMode))
                              + "</DynamicsMode>");
            writer->writeLine("<PlayMode>"
                              + vsq::StringUtil::toString(static_cast<int>(common.playMode()))
                              + "</PlayMode>");
            writer->writeLine("<LastPlayMode>"
                              + vsq::StringUtil::toString(static_cast<int>(common.lastPlayMode()))
                              + "</LastPlayMode>");
        }

        void writeVibratoHandle(const vsq::Handle &handle, vsq::StreamWriter *writer) {
            writer->writeLine("<VibratoHandle>");
            writer->writeLine("<Index>" + vsq::StringUtil::toString(handle.index) + "</Index>");
            writer->writeLine("<IconID>" + handle.iconId + "</IconID>");
            writer->writeLine("<IDS>" + handle.ids + "</IDS>");
            writer->writeLine("<Original>" + vsq::StringUtil::toString(handle.original) + "</Original>");
            writer->writeLine("<Caption>" + handle.caption + "</Caption>");
            writer->writeLine("<RateBP>");
            writer->writeLine("<Data>" + handle.rateBP.data() + "</Data>");
            writer->writeLine("</RateBP>");
            writer->writeLine("<StartRate>"
                              + vsq::StringUtil::toString(handle.startRate)
                              + "</StartRate>");
            writer->writeLine("<DepthBP>");
            writer->writeLine("<Data>" + handle.depthBP.data() + "</Data>");
            writer->writeLine("</DepthBP>");
            writer->writeLine("<StartDepth>"
                              + vsq::StringUtil::toString(handle.startDepth)
                              + "</StartDepth>");
            writer->writeLine("<Length>"
                              + vsq::StringUtil::toString(handle.length())
                              + "</Length>");
            writer->writeLine("</VibratoHandle>");
        }

        void writeLyric(const vsq::Lyric &lyric, vsq::StreamWriter *writer) {
            writer->writeLine("<Phrase>" + lyric.phrase + "</Phrase>");
            writer->writeLine("<UnknownFloat>"
                              + vsq::StringUtil::toString(lyric.lengthRatio, "%g")
                              + "</UnknownFloat>");
            writer->writeLine("<PhoneticSymbolProtected>"
                              + (lyric.isProtected ? std::string("true") : std::string("false"))
                              + "</PhoneticSymbolProtected>");
            writer->writeLine("<ConsonantAdjustment>"
                              + vsq::StringUtil::replace(lyric.consonantAdjustment(), ",", " ")
                              + "</ConsonantAdjustment>");
            writer->writeLine("<PhoneticSymbol>" + lyric.phoneticSymbol() + "</PhoneticSymbol>");
        }

        void writeLyricHandle(const vsq::Handle &handle, vsq::StreamWriter *writer) {
            writer->writeLine("<LyricHandle>");
            writer->writeLine("<L0>");
            {
                writeLyric(handle.get(0), writer);
            }
            writer->writeLine("</L0>");
            writer->writeLine("<Index>" + vsq::StringUtil::toString(handle.index) + "</Index>");
            if (handle.size() > 1) {
                writer->writeLine("<Trailing>");
                for (int i = 1; i < handle.size(); i++) {
                    writer->writeLine("<Lyric>");
                    writeLyric(handle.get(i), writer);
                    writer->writeLine("</Lyric>");
                }
                writer->writeLine("</Trailing>");
            } else {
                writer->writeLine("<Trailing/>");
            }
            writer->writeLine("</LyricHandle>");
        }

        void writeSingerHandle(const vsq::Handle &handle, vsq::StreamWriter *writer) {
            writer->writeLine("<IconHandle>");
            writer->writeLine("<Caption>" + handle.caption + "</Caption>");
            writer->writeLine("<IconID>" + handle.iconId + "</IconID>");
            writer->writeLine("<IDS>" + handle.ids + "</IDS>");
            writer->writeLine("<Index>" + vsq::StringUtil::toString(handle.index) + "</Index>");
            writer->writeLine("<Length>" + vsq::StringUtil::toString(handle.length()) + "</Length>");
            writer->writeLine("<Original>" + vsq::StringUtil::toString(handle.original) + "</Original>");
            writer->writeLine("<Program>" + vsq::StringUtil::toString(handle.program) + "</Program>");
            writer->writeLine("<Language>" + vsq::StringUtil::toString(handle.language) + "</Language>");
            writer->writeLine("</IconHandle>");
        }

    private:
        void writeTotalClocks(const vsq::Sequence *sequence, vsq::StreamWriter *writer) {
            writer->writeLine("<TotalClocks>"
                              + vsq::StringUtil::toString(sequence->totalTicks())
                              + "</TotalClocks>");
        }

        /**
         * @brief Write tracks into xml.
         * @param  sequence  A sequence.
         * @param  writer    A writer, used for io.
         */
        void writeTracks(const vsq::Sequence *sequence, vsq::StreamWriter *writer) {
            writer->writeLine("<Track>");

            vsq::Track emptyTrack;
            writeVsqTrack(emptyTrack, writer);

            for (int i = 0; i < sequence->tracks().size(); i++) {
                writeVsqTrack(sequence->track(i), writer);
            }

            writer->writeLine("</Track>");
        }

        /**
         * @brief Write a track into xml.
         * @param  track   An instance of track to be output.
         * @param  writer  A writer.
         */
        void writeVsqTrack(vsq::Track const& track, vsq::StreamWriter *writer) {
            writer->writeLine("<VsqTrack>");
            writer->writeLine("<MetaText>");
            writer->writeLine("<Common>");
            {
                writeCommon(track.common(), writer);
            }
            writer->writeLine("</Common>");
            writer->writeLine("<Events>");
            {
                writer->writeLine("<Events>");
                int size = track.events().size();
                for (int i = 0; i < size; i++) {
                    vsq::Event event = track.events().get(i)->clone();
                    if (event.vibratoHandle.type() == vsq::HandleType::VIBRATO) {
                        // length property is treated as percentage.
                        int percent = event.vibratoHandle.length();
                        int clockLength = event.length() * percent / 100;
                        event.vibratoHandle.length(clockLength);
                        event.vibratoDelay = event.length() - clockLength;
                    }
                    writer->writeLine("<VsqEvent>");
                    writeEvent(&event, writer);
                    writer->writeLine("</VsqEvent>");
                }
                writer->writeLine("</Events>");
            }
            writer->writeLine("</Events>");
            std::vector<std::string> const* curveNameList = track.curveNameList();
            std::vector<std::string>::const_iterator i = curveNameList->begin();
            for (; i != curveNameList->end(); ++i) {
                std::string curveName = (*i);
                writeBPList(track.curve(curveName), writer);
            }
            writer->writeLine("</MetaText>");
            writer->writeLine("</VsqTrack>");
        }

        void writeEvent(const vsq::Event *event, vsq::StreamWriter *writer) {
            writer->writeLine("<InternalID>" + vsq::StringUtil::toString(event->id) + "</InternalID>");
            writer->writeLine("<Clock>" + vsq::StringUtil::toString(event->tick) + "</Clock>");
            writer->writeLine("<ID>");
            {
                writer->writeLine("<type>" + vsq::EventTypeUtil::toString(event->type()) + "</type>");
                if (event->singerHandle.type() == vsq::HandleType::SINGER) {
                    writeSingerHandle(event->singerHandle, writer);
                }

                writer->writeLine("<Note>" + vsq::StringUtil::toString(event->note) + "</Note>");
                writer->writeLine("<Dynamics>"
                                  + vsq::StringUtil::toString(event->dynamics)
                                  + "</Dynamics>");
                writer->writeLine("<PMBendDepth>" + vsq::StringUtil::toString(event->pmBendDepth)
                                  + "</PMBendDepth>");
                writer->writeLine("<PMBendLength>" + vsq::StringUtil::toString(event->pmBendLength)
                                  + "</PMBendLength>");
                writer->writeLine("<PMbPortamentoUse>"
                                  + vsq::StringUtil::toString(event->pmbPortamentoUse)
                                  + "</PMbPortamentoUse>");
                writer->writeLine("<DEMdecGainRate>" + vsq::StringUtil::toString(event->demDecGainRate)
                                  + "</DEMdecGainRate>");
                writer->writeLine("<DEMaccent>" + vsq::StringUtil::toString(event->demAccent)
                                  + "</DEMaccent>");

                if (event->lyricHandle.type() == vsq::HandleType::LYRIC) {
                    writeLyricHandle(event->lyricHandle, writer);
                }

                if (event->vibratoHandle.type() == vsq::HandleType::VIBRATO) {
                    writeVibratoHandle(event->vibratoHandle, writer);
                }

                writer->writeLine("<VibratoDelay>"
                                  + vsq::StringUtil::toString(event->vibratoDelay)
                                  + "</VibratoDelay>");
                writer->writeLine("<pMeanOnsetFirstNote>"
                                  + vsq::StringUtil::toString(event->pMeanOnsetFirstNote)
                                  + "</pMeanOnsetFirstNote>");
                writer->writeLine("<vMeanNoteTransition>"
                                  + vsq::StringUtil::toString(event->vMeanNoteTransition)
                                  + "</vMeanNoteTransition>");
                writer->writeLine("<d4mean>"
                                  + vsq::StringUtil::toString(event->d4mean)
                                  + "</d4mean>");
                writer->writeLine("<pMeanEndingNote>"
                                  + vsq::StringUtil::toString(event->pMeanEndingNote)
                                  + "</pMeanEndingNote>");
                writer->writeLine("<Length>"
                                  + vsq::StringUtil::toString(event->length())
                                  + "</Length>");
            }
            writer->writeLine("</ID>");
        }

        /**
         * @brief Write xml header and root element.
         * @param  writer  A writer.
         */
        void writeHeader(vsq::StreamWriter *writer) {
            writer->writeLine("<?xml version=\"1.0\"?>");
            writer->writeLine("<VsqFileEx xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                              "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">");
        }

        /**
         * @brief Write closing element of root.
         * @param  writer  A writer.
         */
        void writeFooter(vsq::StreamWriter *writer) {
            writer->writeLine("</VsqFileEx>");
        }
    };
}

#endif
