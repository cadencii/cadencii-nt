#include "../../Util.hpp"
#include "../../../sequence/io/XVSQFileWriter.hpp"
#include <libvsq/libvsq.h>
#include <gtest/gtest.h>

using namespace std;
using namespace cadencii;
using namespace vsq;

class XVSQFileWriterStub : public XVSQFileWriter {
    typedef XVSQFileWriter base_type;
public:
    void writeCommon(Common const& common, StreamWriter *writer)
    {
        base_type::writeCommon(common, writer);
    }

    void writeVibratoHandle(const vsq::Handle &handle, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeVibratoHandle(handle, writer);
    }

    void writeLyric(const vsq::Lyric &lyric, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeLyric(lyric, writer);
    }

    void writeLyricHandle(const vsq::Handle &handle, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeLyricHandle(handle, writer);
    }

    void writeSingerHandle(const vsq::Handle &handle, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeSingerHandle(handle, writer);
    }

    void writeBPList(const vsq::BPList *list, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeBPList(list, writer);
    }

    void writeTempoTable(const vsq::TempoList &tempoList, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeTempoTable(tempoList, writer);
    }

    void writeTimesigTable(const vsq::TimesigList &timesigList, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeTimesigTable(timesigList, writer);
    }

    void writeMaster(const vsq::Master &master, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeMaster(master, writer);
    }

    void writeMixer(const vsq::Mixer &mixer, vsq::StreamWriter *writer) {
        XVSQFileWriter::writeMixer(mixer, writer);
    }
};

TEST(XVSQFileWriterTest, writeMixer)
{
    XVSQFileWriterStub writer;
    {
        Sequence sequence;
        Event note(1920, EventType::NOTE);
        note.note = 60;
        sequence.track(0).events().add(note);
        FileOutputStream fos("/tmp/hoge.xvsq");
        writer.write(&sequence, &fos);
    }
    Mixer mixer;
    mixer.masterFeder = 3;
    mixer.masterPanpot = 2;
    mixer.masterMute = 1;
    mixer.outputMode = 0;
    mixer.slave.push_back(MixerItem(2, 5, 1, 0));

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeMixer(mixer, &stream);
    string actual = byteStream->toString();
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
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeMaster)
{
    XVSQFileWriterStub writer;
    Master master;
    master.preMeasure = 1;

    ByteArrayOutputStream *byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeMaster(master, &stream);
    string actual = byteStream->toString();
    string expected =
        "<Master>\n"
        "<PreMeasure>1</PreMeasure>\n"
        "</Master>\n";
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeTimesigTable)
{
    XVSQFileWriterStub writer;
    TimesigList list;
    list.clear();
    list.push(Timesig(4, 4, 0));
    list.push(Timesig(3, 4, 1));

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeTimesigTable(list, &stream);
    string actual = byteStream->toString();
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
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeTempoTable)
{
    XVSQFileWriterStub writer;
    TempoList list;
    list.clear();
    list.push(Tempo(0, 500000));
    list.push(Tempo(1920, 250000));

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeTempoTable(list, &stream);
    string actual = byteStream->toString();
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
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeBPList)
{
    XVSQFileWriterStub writer;
    BPList list("pit", 0, -8192, 8191);
    list.add(0, 0);
    list.add(1920, 1);

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeBPList(&list, &stream);
    string actual = byteStream->toString();
    string expected =
        "<PIT>\n"
        "<Default>0</Default>\n"
        "<Name>pit</Name>\n"
        "<Data>0=0,1920=1</Data>\n"
        "<Maximum>8191</Maximum>\n"
        "<Minimum>-8192</Minimum>\n"
        "</PIT>\n";
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeSingerHandle)
{
    XVSQFileWriterStub writer;
    Handle handle(HandleType::SINGER);
    handle.caption = "hoge";
    handle.iconId = "$01010000";
    handle.ids = "foo";
    handle.index = 11;
    handle.length(1);
    handle.original = 3;
    handle.program = 5;
    handle.language = 1;

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeSingerHandle(handle, &stream);
    string actual = byteStream->toString();
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
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeLyricHandleWithoutTrailingLyric)
{
    XVSQFileWriterStub writer;
    Handle handle(HandleType::LYRIC);
    handle.index = 10;
    Lyric lyric0("ha", "h a");
    lyric0.isProtected = true;
    lyric0.lengthRatio = 0.5;
    lyric0.consonantAdjustment("64,0");
    handle.set(0, lyric0);

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeLyricHandle(handle, &stream);
    string actual = byteStream->toString();
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
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeLyricHandleWithTrailingLyric)
{
    XVSQFileWriterStub writer;
    Handle handle(HandleType::LYRIC);
    handle.index = 10;
    Lyric lyric0("ha", "h a");
    lyric0.isProtected = true;
    lyric0.lengthRatio = 0.5;
    lyric0.consonantAdjustment("64,0");
    handle.set(0, lyric0);
    Lyric lyric1("ge", "g e");
    lyric1.isProtected = false;
    lyric1.lengthRatio = 0.5;
    lyric1.consonantAdjustment("63,0");
    handle.add(lyric1);

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeLyricHandle(handle, &stream);
    string actual = byteStream->toString();
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
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeLyric)
{
    XVSQFileWriterStub writer;
    Lyric lyric("ha", "h a");
    lyric.lengthRatio = 1.0;
    lyric.isProtected = true;
    lyric.consonantAdjustment("64,0");

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeLyric(lyric, &stream);
    string actual = byteStream->toString();
    string expected =
        "<Phrase>ha</Phrase>\n"
        "<UnknownFloat>1</UnknownFloat>\n"
        "<PhoneticSymbolProtected>true</PhoneticSymbolProtected>\n"
        "<ConsonantAdjustment>64 0</ConsonantAdjustment>\n"
        "<PhoneticSymbol>h a</PhoneticSymbol>\n";
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeCommon)
{
    XVSQFileWriterStub writer;
    Common common;
    common.version = "hoge";
    common.name = "foo";
    common.color = "123,124,125";
    common.dynamicsMode = DynamicsMode::EXPERT;
    common.playMode(PlayMode::PLAY_WITH_SYNTH);
    common.playMode(PlayMode::PLAY_WITH_SYNTH);

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeCommon(common, &stream);
    string actual = byteStream->toString();
    string expected =
        "<Version>hoge</Version>\n"
        "<Name>foo</Name>\n"
        "<Color>123,124,125</Color>\n"
        "<DynamicsMode>1</DynamicsMode>\n"
        "<PlayMode>1</PlayMode>\n"
        "<LastPlayMode>1</LastPlayMode>\n";
    EXPECT_EQ(expected, actual);
}

TEST(XVSQFileWriterTest, writeVibratoHandle)
{
    XVSQFileWriterStub writer;
    Handle handle(HandleType::VIBRATO);
    handle.index = 10;
    handle.iconId = "$04040000";
    handle.ids = "hoge";
    handle.original = 1;
    handle.caption = "foo";
    handle.rateBP.data("0=1,0.5=2,0.8=3");
    handle.startRate = 1;
    handle.depthBP.data("0=4,0.6=5,0.9=6");
    handle.startDepth = 4;
    handle.length(11);

    ByteArrayOutputStream* byteStream = new ByteArrayOutputStream();
    StreamWriter stream(byteStream);
    writer.writeVibratoHandle(handle, &stream);
    string actual = byteStream->toString();
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
    EXPECT_EQ(expected, actual);
}
