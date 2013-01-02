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
#include "../../vsq/Sequence.hpp"
#include "../../vsq/StreamWriter.hpp"
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
        VSQ_NS::StreamWriter *writer;

    public:
        explicit XVSQFileWriter() : writer(0) {
        }

        /**
         * @brief Output a sequence into stream.
         * @param  sequence  A sequence to be output.
         * @param  stream    A stream.
         */
        void write(const VSQ_NS::Sequence *sequence, VSQ_NS::OutputStream *stream) {
            writer = new VSQ_NS::StreamWriter(stream);
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
        void writeMixer(const VSQ_NS::Mixer &mixer, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<Mixer>");
            writer->writeLine("<MasterFeder>"
                              + StringUtil::toString(mixer.masterFeder)
                              + "</MasterFeder>");
            writer->writeLine("<MasterPanpot>"
                              + StringUtil::toString(mixer.masterPanpot)
                              + "</MasterPanpot>");
            writer->writeLine("<MasterMute>"
                              + StringUtil::toString(mixer.masterMute)
                              + "</MasterMute>");
            writer->writeLine("<OutputMode>"
                              + StringUtil::toString(mixer.outputMode)
                              + "</OutputMode>");
            writer->writeLine("<Slave>");
            for (int i = 0; i < mixer.slave.size(); i++) {
                const VSQ_NS::MixerItem item = mixer.slave[i];
                writer->writeLine("<VsqMixerEntry>");
                writer->writeLine("<Feder>" + StringUtil::toString(item.feder) + "</Feder>");
                writer->writeLine("<Panpot>" + StringUtil::toString(item.panpot) + "</Panpot>");
                writer->writeLine("<Mute>" + StringUtil::toString(item.mute) + "</Mute>");
                writer->writeLine("<Solo>" + StringUtil::toString(item.solo) + "</Solo>");
                writer->writeLine("</VsqMixerEntry>");
            }
            writer->writeLine("</Slave>");
            writer->writeLine("</Mixer>");
        }

        void writeMaster(const VSQ_NS::Master &master, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<Master>");
            writer->writeLine("<PreMeasure>"
                              + StringUtil::toString(master.preMeasure)
                              + "</PreMeasure>");
            writer->writeLine("</Master>");
        }

        void writeTimesigTable(const VSQ_NS::TimesigList &timesigList,
                               VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<TimesigTable>");
            for (int i = 0; i < timesigList.size(); i++) {
                VSQ_NS::Timesig timesig = timesigList.get(i);
                writer->writeLine("<TimeSigTableEntry>");
                writer->writeLine("<Clock>"
                                  + StringUtil::toString(timesig.getClock())
                                  + "</Clock>");
                writer->writeLine("<Numerator>"
                                  + StringUtil::toString(timesig.numerator)
                                  + "</Numerator>");
                writer->writeLine("<Denominator>"
                                  + StringUtil::toString(timesig.denominator)
                                  + "</Denominator>");
                writer->writeLine("<BarCount>"
                                  + StringUtil::toString(timesig.barCount)
                                  + "</BarCount>");
                writer->writeLine("</TimeSigTableEntry>");
            }
            writer->writeLine("</TimesigTable>");
        }

        void writeTempoTable(const VSQ_NS::TempoList &tempoList, VSQ_NS::StreamWriter *writer) {
            VSQ_NS::TempoList copy = tempoList;
            copy.updateTempoInfo();
            writer->writeLine("<TempoTable>");
            for (int i = 0; i < copy.size(); i++) {
                VSQ_NS::Tempo tempo = copy.get(i);
                writer->writeLine("<TempoTableEntry>");
                writer->writeLine("<Clock>" + StringUtil::toString(tempo.clock) + "</Clock>");
                writer->writeLine("<Tempo>" + StringUtil::toString(tempo.tempo) + "</Tempo>");
                writer->writeLine("<Time>"
                                  + StringUtil::toString(tempo.getTime(), "%g")
                                  + "</Time>");
                writer->writeLine("</TempoTableEntry>");
            }
            writer->writeLine("</TempoTable>");
        }

        void writeBPList(const VSQ_NS::BPList *list, VSQ_NS::StreamWriter *writer) {
            XVSQSpec spec;
            std::string elementName = spec.getElementName(list->getName());
            if (elementName.empty()) return;

            writer->writeLine("<" + elementName + ">");
            writer->writeLine("<Default>"
                              + StringUtil::toString(list->getDefault())
                              + "</Default>");
            writer->writeLine("<Name>" + list->getName() + "</Name>");
            writer->writeLine("<Data>" + list->getData() + "</Data>");
            writer->writeLine("<Maximum>"
                              + StringUtil::toString(list->getMaximum())
                              + "</Maximum>");
            writer->writeLine("<Minimum>"
                              + StringUtil::toString(list->getMinimum())
                              + "</Minimum>");
            writer->writeLine("</" + elementName + ">");
        }

        void writeCommon(const VSQ_NS::Common *common, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<Version>" + common->version + "</Version>");
            writer->writeLine("<Name>" + common->name + "</Name>");
            writer->writeLine("<Color>" + common->color + "</Color>");
            writer->writeLine("<DynamicsMode>"
                              + StringUtil::toString(common->dynamicsMode)
                              + "</DynamicsMode>");
            writer->writeLine("<PlayMode>"
                              + StringUtil::toString(common->playMode())
                              + "</PlayMode>");
            writer->writeLine("<LastPlayMode>"
                              + StringUtil::toString(common->lastPlayMode())
                              + "</LastPlayMode>");
        }

        void writeVibratoHandle(const VSQ_NS::Handle &handle, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<VibratoHandle>");
            writer->writeLine("<Index>" + StringUtil::toString(handle.index) + "</Index>");
            writer->writeLine("<IconID>" + handle.iconId + "</IconID>");
            writer->writeLine("<IDS>" + handle.ids + "</IDS>");
            writer->writeLine("<Original>" + StringUtil::toString(handle.original) + "</Original>");
            writer->writeLine("<Caption>" + handle.caption + "</Caption>");
            writer->writeLine("<RateBP>");
            writer->writeLine("<Data>" + handle.rateBP.getData() + "</Data>");
            writer->writeLine("</RateBP>");
            writer->writeLine("<StartRate>"
                              + StringUtil::toString(handle.startRate)
                              + "</StartRate>");
            writer->writeLine("<DepthBP>");
            writer->writeLine("<Data>" + handle.depthBP.getData() + "</Data>");
            writer->writeLine("</DepthBP>");
            writer->writeLine("<StartDepth>"
                              + StringUtil::toString(handle.startDepth)
                              + "</StartDepth>");
            writer->writeLine("<Length>"
                              + StringUtil::toString(handle.getLength())
                              + "</Length>");
            writer->writeLine("</VibratoHandle>");
        }

        void writeLyric(const VSQ_NS::Lyric &lyric, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<Phrase>" + lyric.phrase + "</Phrase>");
            writer->writeLine("<UnknownFloat>"
                              + StringUtil::toString(lyric.lengthRatio, "%g")
                              + "</UnknownFloat>");
            writer->writeLine("<PhoneticSymbolProtected>"
                              + (lyric.isProtected ? string("true") : string("false"))
                              + "</PhoneticSymbolProtected>");
            writer->writeLine("<ConsonantAdjustment>"
                              + StringUtil::replace(lyric.getConsonantAdjustment(), ",", " ")
                              + "</ConsonantAdjustment>");
            writer->writeLine("<PhoneticSymbol>" + lyric.getPhoneticSymbol() + "</PhoneticSymbol>");
        }

        void writeLyricHandle(const VSQ_NS::Handle &handle, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<LyricHandle>");
            writer->writeLine("<L0>");
            {
                writeLyric(handle.getLyricAt(0), writer);
            }
            writer->writeLine("</L0>");
            writer->writeLine("<Index>" + StringUtil::toString(handle.index) + "</Index>");
            if (handle.getLyricCount() > 1) {
                writer->writeLine("<Trailing>");
                for (int i = 1; i < handle.getLyricCount(); i++) {
                    writer->writeLine("<Lyric>");
                    writeLyric(handle.getLyricAt(i), writer);
                    writer->writeLine("</Lyric>");
                }
                writer->writeLine("</Trailing>");
            } else {
                writer->writeLine("<Trailing/>");
            }
            writer->writeLine("</LyricHandle>");
        }

        void writeSingerHandle(const VSQ_NS::Handle &handle, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<IconHandle>");
            writer->writeLine("<Caption>" + handle.caption + "</Caption>");
            writer->writeLine("<IconID>" + handle.iconId + "</IconID>");
            writer->writeLine("<IDS>" + handle.ids + "</IDS>");
            writer->writeLine("<Index>" + StringUtil::toString(handle.index) + "</Index>");
            writer->writeLine("<Length>" + StringUtil::toString(handle.getLength()) + "</Length>");
            writer->writeLine("<Original>" + StringUtil::toString(handle.original) + "</Original>");
            writer->writeLine("<Program>" + StringUtil::toString(handle.program) + "</Program>");
            writer->writeLine("<Language>" + StringUtil::toString(handle.language) + "</Language>");
            writer->writeLine("</IconHandle>");
        }

    private:
        void writeTotalClocks(const VSQ_NS::Sequence *sequence, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<TotalClocks>"
                              + StringUtil::toString(sequence->getTotalClocks())
                              + "</TotalClocks>");
        }

        /**
         * @brief Write tracks into xml.
         * @param  sequence  A sequence.
         * @param  writer    A writer, used for io.
         */
        void writeTracks(const VSQ_NS::Sequence *sequence, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<Track>");

            VSQ_NS::Track emptyTrack;
            writeVsqTrack(&emptyTrack, writer);

            for (int i = 0; i < sequence->tracks()->size(); i++) {
                writeVsqTrack(sequence->track(i), writer);
            }

            writer->writeLine("</Track>");
        }

        /**
         * @brief Write a track into xml.
         * @param  track   An instance of track to be output.
         * @param  writer  A writer.
         */
        void writeVsqTrack(const VSQ_NS::Track *track, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<VsqTrack>");
            writer->writeLine("<MetaText>");
            writer->writeLine("<Common>");
            {
                writeCommon(track->common(), writer);
            }
            writer->writeLine("</Common>");
            writer->writeLine("<Events>");
            {
                writer->writeLine("<Events>");
                int size = track->events()->size();
                for (int i = 0; i < size; i++) {
                    VSQ_NS::Event event = track->events()->get(i)->clone();
                    if (event.vibratoHandle.getHandleType() == VSQ_NS::HandleType::VIBRATO) {
                        // length property is treated as percentage.
                        int percent = event.vibratoHandle.getLength();
                        int clockLength = event.getLength() * percent / 100;
                        event.vibratoHandle.setLength(clockLength);
                        event.vibratoDelay = event.getLength() - clockLength;
                    }
                    writer->writeLine("<VsqEvent>");
                    writeEvent(&event, writer);
                    writer->writeLine("</VsqEvent>");
                }
                writer->writeLine("</Events>");
            }
            writer->writeLine("</Events>");
            const std::vector<std::string> *curveNameList = track->curveNameList();
            std::vector<std::string>::const_iterator i = curveNameList->begin();
            for (; i != curveNameList->end(); ++i) {
                std::string curveName = (*i);
                writeBPList(track->curve(curveName), writer);
            }
            writer->writeLine("</MetaText>");
            writer->writeLine("</VsqTrack>");
        }

        void writeEvent(const VSQ_NS::Event *event, VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<InternalID>" + StringUtil::toString(event->id) + "</InternalID>");
            writer->writeLine("<Clock>" + StringUtil::toString(event->clock) + "</Clock>");
            writer->writeLine("<ID>");
            {
                writer->writeLine("<type>" + VSQ_NS::EventType::toString(event->type) + "</type>");
                if (event->singerHandle.getHandleType() == VSQ_NS::HandleType::SINGER) {
                    writeSingerHandle(event->singerHandle, writer);
                }

                writer->writeLine("<Note>" + StringUtil::toString(event->note) + "</Note>");
                writer->writeLine("<Dynamics>"
                                  + StringUtil::toString(event->dynamics)
                                  + "</Dynamics>");
                writer->writeLine("<PMBendDepth>" + StringUtil::toString(event->pmBendDepth)
                                  + "</PMBendDepth>");
                writer->writeLine("<PMBendLength>" + StringUtil::toString(event->pmBendLength)
                                  + "</PMBendLength>");
                writer->writeLine("<PMbPortamentoUse>"
                                  + StringUtil::toString(event->pmbPortamentoUse)
                                  + "</PMbPortamentoUse>");
                writer->writeLine("<DEMdecGainRate>" + StringUtil::toString(event->demDecGainRate)
                                  + "</DEMdecGainRate>");
                writer->writeLine("<DEMaccent>" + StringUtil::toString(event->demAccent)
                                  + "</DEMaccent>");

                if (event->lyricHandle.getHandleType() == VSQ_NS::HandleType::LYRIC) {
                    writeLyricHandle(event->lyricHandle, writer);
                }

                if (event->vibratoHandle.getHandleType() == VSQ_NS::HandleType::VIBRATO) {
                    writeVibratoHandle(event->vibratoHandle, writer);
                }

                writer->writeLine("<VibratoDelay>"
                                  + StringUtil::toString(event->vibratoDelay)
                                  + "</VibratoDelay>");
                writer->writeLine("<pMeanOnsetFirstNote>"
                                  + StringUtil::toString(event->pMeanOnsetFirstNote)
                                  + "</pMeanOnsetFirstNote>");
                writer->writeLine("<vMeanNoteTransition>"
                                  + StringUtil::toString(event->vMeanNoteTransition)
                                  + "</vMeanNoteTransition>");
                writer->writeLine("<d4mean>"
                                  + StringUtil::toString(event->d4mean)
                                  + "</d4mean>");
                writer->writeLine("<pMeanEndingNote>"
                                  + StringUtil::toString(event->pMeanEndingNote)
                                  + "</pMeanEndingNote>");
                writer->writeLine("<Length>"
                                  + StringUtil::toString(event->getLength())
                                  + "</Length>");
            }
            writer->writeLine("</ID>");
        }

        /**
         * @brief Write xml header and root element.
         * @param  writer  A writer.
         */
        void writeHeader(VSQ_NS::StreamWriter *writer) {
            writer->writeLine("<?xml version=\"1.0\"?>");
            writer->writeLine("<VsqFileEx xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                              "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">");
        }

        /**
         * @brief Write closing element of root.
         * @param  writer  A writer.
         */
        void writeFooter(VSQ_NS::StreamWriter *writer) {
            writer->writeLine("</VsqFileEx>");
        }
    };
}

#endif
