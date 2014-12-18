#include "Util.hpp"
#include "../gui/PropertyValueProxy.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace cadencii;
using namespace vsq;

class PropertyValueProxyStub : public PropertyValueProxy {
public:
    string _lyricPhrase;
    string _lyricPhoneticSymbol;
    string _lyricConsonantAdjustment;
    int  _lyricProtect;
    string _noteLength;
    string _noteNumber;
    string _notelocationClock;
    string _notelocationMeasure;
    string _notelocationBeat;
    string _notelocationTick;
    int _vibratoType;
    string _vibratoLength;

public:
    string getLyricPhrase() { return _lyricPhrase; }
    string getLyricPhoneticSymbol() { return _lyricPhoneticSymbol; }
    string getLyricConsonantAdjustment() { return _lyricConsonantAdjustment; }
    int getLyricProtect() { return _lyricProtect; }
    string getNoteLength() { return _noteLength; }
    string getNoteNumber() { return _noteNumber; }
    string getNotelocationClock() { return _notelocationClock; }
    string getNotelocationMeasure() { return _notelocationMeasure; }
    string getNotelocationBeat() { return _notelocationBeat; }
    string getNotelocationTick() { return _notelocationTick; }
    int getVibratoType() { return _vibratoType; }
    string getVibratoLength() { return _vibratoLength; }
    void setLyricPhrase(const string &lyricPhrase) { _lyricPhrase = lyricPhrase; }
    void setLyricPhoneticSymbol(const string &lyricPhoneticSymbol) { _lyricPhoneticSymbol = lyricPhoneticSymbol; }
    void setLyricConsonantAdjustment(const string &lyricConsonantAdjustment) { _lyricConsonantAdjustment = lyricConsonantAdjustment; }
    void setLyricProtect(int lyricProtect) { _lyricProtect = lyricProtect; }

    void setNoteLength(const string &noteLength) { _noteLength = noteLength; }
    void setNoteNumber(const string &noteNumber) { _noteNumber = noteNumber; }

    void setNotelocationClock(const string &notelocationClock) { _notelocationClock = notelocationClock; }
    void setNotelocationMeasure(const string &notelocationMeasure) { _notelocationMeasure = notelocationMeasure; }
    void setNotelocationBeat(const string &notelocationBeat) { _notelocationBeat = notelocationBeat; }
    void setNotelocationTick(const string &notelocationTick) { _notelocationTick = notelocationTick; }

    void setVibratoType(int vibratoType) { _vibratoType = vibratoType; }
    void setVibratoLength(const string &vibratoLength) { _vibratoLength = vibratoLength; }
};

class PropertyValueProxyTest : public testing::Test
{
protected:
    void SetUp() override
    {
        sequence = Sequence("hoge", 1, 4, 4, 500000);
        note = Event(480, EventType::NOTE);
        note.lyricHandle.set(0, Lyric("ra", "4 a"));
        note.length(480);
        note.note = 60;

        note1 = Event(2281, EventType::NOTE);
        Lyric l("i", "i");
        l.isProtected = true;
        note1.lyricHandle.set(0, l);
        note1.length(481);
        note1.note = 61;
        note1.vibratoHandle = Handle(HandleType::VIBRATO);
        note1.vibratoHandle.iconId = "$04040003";
        note1.vibratoHandle.length(100);
    }

    void assertEqualToDefault(PropertyValueProxyStub *stub, void * except) {
        vector<void *> exceptList;
        exceptList.push_back(except);
        assertEqualToDefault(stub, exceptList);
    }

    void assertEqualToDefault(PropertyValueProxyStub *stub, const vector<void *> &exceptList) {
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhrase) == exceptList.end()) EXPECT_EQ(string(""), stub->_lyricPhrase);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhoneticSymbol) == exceptList.end()) EXPECT_EQ(string(""), stub->_lyricPhoneticSymbol);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricConsonantAdjustment) == exceptList.end()) EXPECT_EQ(string(""), stub->_lyricConsonantAdjustment);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricProtect) == exceptList.end()) EXPECT_EQ(0, stub->_lyricProtect);

        if (find(exceptList.begin(), exceptList.end(), &stub->_noteLength) == exceptList.end()) EXPECT_EQ(string(""), stub->_noteLength);
        if (find(exceptList.begin(), exceptList.end(), &stub->_noteNumber) == exceptList.end()) EXPECT_EQ(string(""), stub->_noteNumber);

        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationClock) == exceptList.end()) EXPECT_EQ(string(""), stub->_notelocationClock);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationMeasure) == exceptList.end()) EXPECT_EQ(string(""), stub->_notelocationMeasure);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationBeat) == exceptList.end()) EXPECT_EQ(string(""), stub->_notelocationBeat);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationTick) == exceptList.end()) EXPECT_EQ(string(""), stub->_notelocationTick);

        if (find(exceptList.begin(), exceptList.end(), &stub->_vibratoType) == exceptList.end()) EXPECT_EQ(0, stub->_vibratoType);
        if (find(exceptList.begin(), exceptList.end(), &stub->_vibratoLength) == exceptList.end()) EXPECT_EQ(string(""), stub->_vibratoLength);
    }

    void assertEqualToNoteExcept(PropertyValueProxyStub *stub, void *except) {
        vector<void *> exceptList;
        exceptList.push_back(except);
        assertEqualToNoteExcept(stub, exceptList);
    }

    void assertEqualToNoteExcept(PropertyValueProxyStub *stub, const vector<void *> &exceptList) {
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhrase) == exceptList.end()) EXPECT_EQ(string("ra"), stub->_lyricPhrase);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhoneticSymbol) == exceptList.end()) EXPECT_EQ(string("4 a"), stub->_lyricPhoneticSymbol);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricConsonantAdjustment) == exceptList.end()) EXPECT_EQ(string("64,0"), stub->_lyricConsonantAdjustment);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricProtect) == exceptList.end()) EXPECT_EQ(1, stub->_lyricProtect);

        if (find(exceptList.begin(), exceptList.end(), &stub->_noteLength) == exceptList.end()) EXPECT_EQ(string("480"), stub->_noteLength);
        if (find(exceptList.begin(), exceptList.end(), &stub->_noteNumber) == exceptList.end()) EXPECT_EQ(string("60"), stub->_noteNumber);

        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationClock) == exceptList.end()) EXPECT_EQ(string("480"), stub->_notelocationClock);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationMeasure) == exceptList.end()) EXPECT_EQ(string("0"), stub->_notelocationMeasure);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationBeat) == exceptList.end()) EXPECT_EQ(string("2"), stub->_notelocationBeat);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationTick) == exceptList.end()) EXPECT_EQ(string("0"), stub->_notelocationTick);

        if (find(exceptList.begin(), exceptList.end(), &stub->_vibratoType) == exceptList.end()) EXPECT_EQ(0, stub->_vibratoType);
        if (find(exceptList.begin(), exceptList.end(), &stub->_vibratoLength) == exceptList.end()) EXPECT_EQ(string("0"), stub->_vibratoLength);
    }

protected:
    Sequence sequence;
    Event note;
    Event note1;
    PropertyValueProxyStub stub;
};

TEST_F(PropertyValueProxyTest, testCommitNoNote)
{
    stub.begin();
    stub.commit();

    string stringPropertyDefault = "";
    int enumPropertyDefault = 0;

    EXPECT_EQ(stringPropertyDefault, stub._lyricPhrase);
    EXPECT_EQ(stringPropertyDefault, stub._lyricPhoneticSymbol);
    EXPECT_EQ(stringPropertyDefault, stub._lyricConsonantAdjustment);
    EXPECT_EQ(enumPropertyDefault, stub._lyricProtect);

    EXPECT_EQ(stringPropertyDefault, stub._noteLength);
    EXPECT_EQ(stringPropertyDefault, stub._noteNumber);

    EXPECT_EQ(stringPropertyDefault, stub._notelocationClock);
    EXPECT_EQ(stringPropertyDefault, stub._notelocationMeasure);
    EXPECT_EQ(stringPropertyDefault, stub._notelocationBeat);
    EXPECT_EQ(stringPropertyDefault, stub._notelocationTick);

    EXPECT_EQ(enumPropertyDefault, stub._vibratoType);
    EXPECT_EQ(stringPropertyDefault, stub._vibratoLength);
}

TEST_F(PropertyValueProxyTest, testCommitOneNote)
{
    Event note = this->note;
    stub.begin();
    stub.add(&note, &sequence);
    stub.commit();

    assertEqualToNoteExcept(&stub, 0);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesWithSameProperties)
{
    Event noteA = note;
    Event noteB = note;
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    assertEqualToNoteExcept(&stub, 0);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesLyricPhraseDiffers)
{
    Event noteA = note;
    Event noteB = note;
    noteB.lyricHandle.set(0, Lyric("wa", "4 a"));
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    assertEqualToNoteExcept(&stub, &stub._lyricPhrase);
    EXPECT_EQ(string(""), stub._lyricPhrase);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesLyricPhoneticSymbolDiffers)
{
    Event noteA = note;
    Event noteB = note;
    noteB.lyricHandle.set(0, Lyric("ra", "a"));
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    vector<void *> exceptList;
    exceptList.push_back(&stub._lyricPhoneticSymbol);
    exceptList.push_back(&stub._lyricConsonantAdjustment);
    assertEqualToNoteExcept(&stub, exceptList);

    EXPECT_EQ(string(""), stub._lyricPhoneticSymbol);
    EXPECT_EQ(string(""), stub._lyricConsonantAdjustment);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesLyricProtectDiffers)
{
    Event noteA = note;
    Event noteB = note;
    Lyric lyric("ra", "4 a");
    lyric.isProtected = true;
    noteB.lyricHandle.set(0, lyric);
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    assertEqualToNoteExcept(&stub, &stub._lyricProtect);
    EXPECT_EQ(0, stub._lyricProtect);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesNoteLengthDiffers)
{
    Event noteA = note;
    Event noteB = note;
    noteB.length(noteA.length() + 1);
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    assertEqualToNoteExcept(&stub, &stub._noteLength);
    EXPECT_EQ(string(""), stub._noteLength);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesNoteNumberDiffers)
{
    Event noteA = note;
    Event noteB = note;
    noteB.note = noteA.note + 1;
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    assertEqualToNoteExcept(&stub, &stub._noteNumber);
    EXPECT_EQ(string(""), stub._noteNumber);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesNotelocationClockDiffers)
{
    Event noteA = note;
    Event noteB = note;
    noteB.tick = 3361;
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    vector<void *> exceptList;
    exceptList.push_back(&stub._notelocationClock);
    exceptList.push_back(&stub._notelocationMeasure);
    exceptList.push_back(&stub._notelocationBeat);
    exceptList.push_back(&stub._notelocationTick);
    assertEqualToNoteExcept(&stub, exceptList);

    EXPECT_EQ(string(""), stub._notelocationClock);
    EXPECT_EQ(string(""), stub._notelocationMeasure);
    EXPECT_EQ(string(""), stub._notelocationBeat);
    EXPECT_EQ(string(""), stub._notelocationTick);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesVibratoTypeDifferers)
{
    Event noteA = note;
    Event noteB = note;
    noteB.vibratoHandle = Handle(HandleType::VIBRATO);
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    assertEqualToNoteExcept(&stub, &stub._vibratoType);
    EXPECT_EQ(0, stub._vibratoType);
}

TEST_F(PropertyValueProxyTest, testCommitTwoNotesVibratoLengthDiffers)
{
    Event noteA = note;
    Event noteB = note;
    noteB.vibratoHandle = Handle(HandleType::VIBRATO);
    noteB.vibratoHandle.length(120);
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    vector<void *> exceptList;
    exceptList.push_back(&stub._vibratoType);
    exceptList.push_back(&stub._vibratoLength);
    assertEqualToNoteExcept(&stub, exceptList);
    EXPECT_EQ(0, stub._vibratoType);
    EXPECT_EQ(string(""), stub._vibratoLength);
}

TEST_F(PropertyValueProxyTest, testCommitAllPropertyDiffers)
{
    Event noteA = note;
    Event noteB = note1;
    stub.begin();
    stub.add(&noteA, &sequence);
    stub.add(&noteB, &sequence);
    stub.commit();

    assertEqualToDefault(&stub, 0);
}
