#include "../../Util.hpp"
#include "../../../sequence/io/XVSQFileWriter.hpp"
#include "../../../vsq/FileOutputStream.hpp"
#include "../../../vsq/TempoList.hpp"
#include "../../../vsq/ByteArrayOutputStream.hpp"

using namespace std;
using namespace cadencii;
using namespace VSQ_NS;

class XVSQFileWriterStub : public XVSQFileWriter {
public:
    void writeCommon(const Common *common, StreamWriter *writer) {
        XVSQFileWriter::writeCommon(common, writer);
    }

    void writeVibratoHandle(const VSQ_NS::Handle &handle, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeVibratoHandle(handle, writer);
    }

    void writeLyric(const VSQ_NS::Lyric &lyric, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeLyric(lyric, writer);
    }

    void writeLyricHandle(const VSQ_NS::Handle &handle, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeLyricHandle(handle, writer);
    }

    void writeSingerHandle(const VSQ_NS::Handle &handle, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeSingerHandle(handle, writer);
    }

    void writeBPList(const VSQ_NS::BPList *list, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeBPList(list, writer);
    }

    void writeTempoTable(const VSQ_NS::TempoList &tempoList, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeTempoTable(tempoList, writer);
    }

    void writeTimesigTable(const VSQ_NS::TimesigList &timesigList, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeTimesigTable(timesigList, writer);
    }

    void writeMaster(const VSQ_NS::Master &master, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeMaster(master, writer);
    }

    void writeMixer(const VSQ_NS::Mixer &mixer, VSQ_NS::StreamWriter *writer) {
        XVSQFileWriter::writeMixer(mixer, writer);
    }
};

class XVSQFileWriterTest : public CppUnit::TestCase {
public:
    void writeMixer() {
        XVSQFileWriterStub writer;
        {
            Sequence sequence;
            Event note(1920, EventType::NOTE);
            note.note = 60;
            sequence.track(0)->events()->add(note);
            FileOutputStream fos("/tmp/hoge.xvsq");
            writer.write(&sequence, &fos);
        }
        Mixer mixer;
        mixer.masterFeder = 3;
        mixer.masterPanpot = 2;
        mixer.masterMute = 1;
        mixer.outputMode = 0;
        mixer.slave.push_back(MixerItem(2, 5, 1, 0));

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeMixer(mixer, &stream);
        string actual = byteStream.toString();
        string expected =
            "<Mixer>\n"
            "<MasterFeder>3</MasterFeder>\n"
            "<MasterPanpot>2</MasterPanpot>\n"
            "<MasterMute>1</MasterMute>\n"
            "<OutputMode>0</OutputMode>\n"
            "<Slave>\n"
            "<VsqMixerEntry>\n"
            "<Feder>2</Feder>\n"
            "<Panpot>5</Panpot>\n"
            "<Mute>1</Mute>\n"
            "<Solo>0</Solo>\n"
            "</VsqMixerEntry>\n"
            "</Slave>\n"
            "</Mixer>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeMaster() {
        XVSQFileWriterStub writer;
        Master master;
        master.preMeasure = 1;

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeMaster(master, &stream);
        string actual = byteStream.toString();
        string expected =
            "<Master>\n"
            "<PreMeasure>1</PreMeasure>\n"
            "</Master>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeTimesigTable() {
        XVSQFileWriterStub writer;
        TimesigList list;
        list.clear();
        list.push(Timesig(4, 4, 0));
        list.push(Timesig(3, 4, 1));

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeTimesigTable(list, &stream);
        string actual = byteStream.toString();
        string expected = "<TimesigTable>\n"
            "<TimeSigTableEntry>\n"
            "<Clock>0</Clock>\n"
            "<Numerator>4</Numerator>\n"
            "<Denominator>4</Denominator>\n"
            "<BarCount>0</BarCount>\n"
            "</TimeSigTableEntry>\n"
            "<TimeSigTableEntry>\n"
            "<Clock>1920</Clock>\n"
            "<Numerator>3</Numerator>\n"
            "<Denominator>4</Denominator>\n"
            "<BarCount>1</BarCount>\n"
            "</TimeSigTableEntry>\n"
            "</TimesigTable>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeTempoTable() {
        XVSQFileWriterStub writer;
        TempoList list;
        list.clear();
        list.push(Tempo(0, 500000));
        list.push(Tempo(1920, 250000));

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeTempoTable(list, &stream);
        string actual = byteStream.toString();
        string expected =
            "<TempoTable>\n"
            "<TempoTableEntry>\n"
            "<Clock>0</Clock>\n"
            "<Tempo>500000</Tempo>\n"
            "<Time>0</Time>\n"
            "</TempoTableEntry>\n"
            "<TempoTableEntry>\n"
            "<Clock>1920</Clock>\n"
            "<Tempo>250000</Tempo>\n"
            "<Time>2</Time>\n"
            "</TempoTableEntry>\n"
            "</TempoTable>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeBPList() {
        XVSQFileWriterStub writer;
        BPList list("pit", 0, -8192, 8191);
        list.add(0, 0);
        list.add(1920, 1);

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeBPList(&list, &stream);
        string actual = byteStream.toString();
        string expected =
            "<PIT>\n"
            "<Default>0</Default>\n"
            "<Name>pit</Name>\n"
            "<Data>0=0,1920=1</Data>\n"
            "<Maximum>8191</Maximum>\n"
            "<Minimum>-8192</Minimum>\n"
            "</PIT>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeSingerHandle() {
        XVSQFileWriterStub writer;
        Handle handle(HandleType::SINGER);
        handle.caption = "hoge";
        handle.iconId = "$01010000";
        handle.ids = "foo";
        handle.index = 11;
        handle.setLength(1);
        handle.original = 3;
        handle.program = 5;
        handle.language = 1;

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeSingerHandle(handle, &stream);
        string actual = byteStream.toString();
        string expected =
            "<IconHandle>\n"
            "<Caption>hoge</Caption>\n"
            "<IconID>$01010000</IconID>\n"
            "<IDS>foo</IDS>\n"
            "<Index>11</Index>\n"
            "<Length>1</Length>\n"
            "<Original>3</Original>\n"
            "<Program>5</Program>\n"
            "<Language>1</Language>\n"
            "</IconHandle>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeLyricHandleWithoutTrailingLyric() {
        XVSQFileWriterStub writer;
        Handle handle(HandleType::LYRIC);
        handle.index = 10;
        Lyric lyric0("ha", "h a");
        lyric0.isProtected = true;
        lyric0.lengthRatio = 0.5;
        lyric0.setConsonantAdjustment("64,0");
        handle.setLyricAt(0, lyric0);

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeLyricHandle(handle, &stream);
        string actual = byteStream.toString();
        string expected =
            "<LyricHandle>\n"
            "<L0>\n"
                "<Phrase>ha</Phrase>\n"
                "<UnknownFloat>0.5</UnknownFloat>\n"
                "<PhoneticSymbolProtected>true</PhoneticSymbolProtected>\n"
                "<ConsonantAdjustment>64 0</ConsonantAdjustment>\n"
                "<PhoneticSymbol>h a</PhoneticSymbol>\n"
            "</L0>\n"
            "<Index>10</Index>\n"
            "<Trailing/>\n"
            "</LyricHandle>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeLyricHandleWithTrailingLyric() {
        XVSQFileWriterStub writer;
        Handle handle(HandleType::LYRIC);
        handle.index = 10;
        Lyric lyric0("ha", "h a");
        lyric0.isProtected = true;
        lyric0.lengthRatio = 0.5;
        lyric0.setConsonantAdjustment("64,0");
        handle.setLyricAt(0, lyric0);
        Lyric lyric1("ge", "g e");
        lyric1.isProtected = false;
        lyric1.lengthRatio = 0.5;
        lyric1.setConsonantAdjustment("63,0");
        handle.addLyric(lyric1);

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeLyricHandle(handle, &stream);
        string actual = byteStream.toString();
        string expected =
            "<LyricHandle>\n"
            "<L0>\n"
                "<Phrase>ha</Phrase>\n"
                "<UnknownFloat>0.5</UnknownFloat>\n"
                "<PhoneticSymbolProtected>true</PhoneticSymbolProtected>\n"
                "<ConsonantAdjustment>64 0</ConsonantAdjustment>\n"
                "<PhoneticSymbol>h a</PhoneticSymbol>\n"
            "</L0>\n"
            "<Index>10</Index>\n"
            "<Trailing>\n"
            "<Lyric>\n"
                "<Phrase>ge</Phrase>\n"
                "<UnknownFloat>0.5</UnknownFloat>\n"
                "<PhoneticSymbolProtected>false</PhoneticSymbolProtected>\n"
                "<ConsonantAdjustment>63 0</ConsonantAdjustment>\n"
                "<PhoneticSymbol>g e</PhoneticSymbol>\n"
            "</Lyric>\n"
            "</Trailing>\n"
            "</LyricHandle>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeLyric() {
        XVSQFileWriterStub writer;
        Lyric lyric("ha", "h a");
        lyric.lengthRatio = 1.0;
        lyric.isProtected = true;
        lyric.setConsonantAdjustment("64,0");

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeLyric(lyric, &stream);
        string actual = byteStream.toString();
        string expected =
            "<Phrase>ha</Phrase>\n"
            "<UnknownFloat>1</UnknownFloat>\n"
            "<PhoneticSymbolProtected>true</PhoneticSymbolProtected>\n"
            "<ConsonantAdjustment>64 0</ConsonantAdjustment>\n"
            "<PhoneticSymbol>h a</PhoneticSymbol>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeCommon() {
        XVSQFileWriterStub writer;
        Common common;
        common.version = "hoge";
        common.name = "foo";
        common.color = "123,124,125";
        common.dynamicsMode = DynamicsMode::EXPERT;
        common.setPlayMode(PlayMode::PLAY_WITH_SYNTH);
        common.setPlayMode(PlayMode::PLAY_WITH_SYNTH);

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeCommon(&common, &stream);
        string actual = byteStream.toString();
        string expected =
            "<Version>hoge</Version>\n"
            "<Name>foo</Name>\n"
            "<Color>123,124,125</Color>\n"
            "<DynamicsMode>1</DynamicsMode>\n"
            "<PlayMode>1</PlayMode>\n"
            "<LastPlayMode>1</LastPlayMode>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void writeVibratoHandle() {
        XVSQFileWriterStub writer;
        Handle handle(HandleType::VIBRATO);
        handle.index = 10;
        handle.iconId = "$04040000";
        handle.ids = "hoge";
        handle.original = 1;
        handle.caption = "foo";
        handle.rateBP.setData("0=1,0.5=2,0.8=3");
        handle.startRate = 1;
        handle.depthBP.setData("0=4,0.6=5,0.9=6");
        handle.startDepth = 4;
        handle.setLength(11);

        ByteArrayOutputStream byteStream;
        StreamWriter stream(&byteStream);
        writer.writeVibratoHandle(handle, &stream);
        string actual = byteStream.toString();
        string expected =
            "<VibratoHandle>\n"
            "<Index>10</Index>\n"
            "<IconID>$04040000</IconID>\n"
            "<IDS>hoge</IDS>\n"
            "<Original>1</Original>\n"
            "<Caption>foo</Caption>\n"
            "<RateBP>\n"
            "<Data>0=1,0.5=2,0.8=3</Data>\n"
            "</RateBP>\n"
            "<StartRate>1</StartRate>\n"
            "<DepthBP>\n"
            "<Data>0=4,0.6=5,0.9=6</Data>\n"
            "</DepthBP>\n"
            "<StartDepth>4</StartDepth>\n"
            "<Length>11</Length>\n"
            "</VibratoHandle>\n";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    CPPUNIT_TEST_SUITE(XVSQFileWriterTest);
    CPPUNIT_TEST(writeMixer);
    CPPUNIT_TEST(writeMaster);
    CPPUNIT_TEST(writeTimesigTable);
    CPPUNIT_TEST(writeTempoTable);
    CPPUNIT_TEST(writeBPList);
    CPPUNIT_TEST(writeSingerHandle);
    CPPUNIT_TEST(writeLyricHandleWithoutTrailingLyric);
    CPPUNIT_TEST(writeLyricHandleWithTrailingLyric);
    CPPUNIT_TEST(writeLyric);
    CPPUNIT_TEST(writeCommon);
    CPPUNIT_TEST(writeVibratoHandle);
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(XVSQFileWriterTest);
