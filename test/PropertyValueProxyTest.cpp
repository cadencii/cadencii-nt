#include "Util.hpp"
#include "../gui/PropertyValueProxy.hpp"

using namespace std;
using namespace cadencii;
using namespace VSQ_NS;

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

class PropertyValueProxyTest : public CppUnit::TestCase {
private:
    Sequence sequence;
    Event note;
    PropertyValueProxyStub stub;

public:
    void setUp() {
        sequence = Sequence("hoge", 1, 4, 4, 500000);
        note = Event(480, EventType::NOTE);
        note.lyricHandle.setLyricAt(0, Lyric("ra", "4 a"));
        note.setLength(480);
        note.note = 60;
    }

    void testCommitNoNote() {
        stub.begin();
        stub.commit();

        string stringPropertyDefault = "";
        int enumPropertyDefault = 0;

        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._lyricPhrase);
        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._lyricPhoneticSymbol);
        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._lyricConsonantAdjustment);
        CPPUNIT_ASSERT_EQUAL(enumPropertyDefault, stub._lyricProtect);

        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._noteLength);
        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._noteNumber);

        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._notelocationClock);
        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._notelocationMeasure);
        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._notelocationBeat);
        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._notelocationTick);

        CPPUNIT_ASSERT_EQUAL(enumPropertyDefault, stub._vibratoType);
        CPPUNIT_ASSERT_EQUAL(stringPropertyDefault, stub._vibratoLength);
    }

    void testCommitOneNote() {
        Event note = this->note;
        stub.begin();
        stub.add(&note, &sequence);
        stub.commit();

        assertDefaultsExcept(&stub, 0);
    }

    void testCommitTwoNotesWithSameProperties() {
        Event noteA = note;
        Event noteB = note;
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertDefaultsExcept(&stub, 0);
    }

    void testCommitTwoNotesLyricPhraseDiffers() {
        Event noteA = note;
        Event noteB = note;
        noteB.lyricHandle.setLyricAt(0, Lyric("wa", "4 a"));
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertDefaultsExcept(&stub, &stub._lyricPhrase);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._lyricPhrase);
    }

    void testCommitTwoNotesLyricPhoneticSymbolDiffers() {
        Event noteA = note;
        Event noteB = note;
        noteB.lyricHandle.setLyricAt(0, Lyric("ra", "a"));
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        vector<void *> exceptList;
        exceptList.push_back(&stub._lyricPhoneticSymbol);
        exceptList.push_back(&stub._lyricConsonantAdjustment);
        assertDefaultsExcept(&stub, exceptList);

        CPPUNIT_ASSERT_EQUAL(string(""), stub._lyricPhoneticSymbol);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._lyricConsonantAdjustment);
    }

    void testCommitTwoNotesLyricProtectDiffers() {
        Event noteA = note;
        Event noteB = note;
        Lyric lyric("ra", "4 a");
        lyric.isProtected = true;
        noteB.lyricHandle.setLyricAt(0, lyric);
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertDefaultsExcept(&stub, &stub._lyricProtect);
        CPPUNIT_ASSERT_EQUAL(0, stub._lyricProtect);
    }

    void testCommitTwoNotesNoteLengthDiffers() {
        Event noteA = note;
        Event noteB = note;
        noteB.setLength(noteA.getLength() + 1);
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertDefaultsExcept(&stub, &stub._noteLength);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._noteLength);
    }

    void testCommitTwoNotesNoteNumberDiffers() {
        Event noteA = note;
        Event noteB = note;
        noteB.note = noteA.note + 1;
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertDefaultsExcept(&stub, &stub._noteNumber);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._noteNumber);
    }

    void testCommitTwoNotesNotelocationClockDiffers() {
        Event noteA = note;
        Event noteB = note;
        noteB.clock = 3361;
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        vector<void *> exceptList;
        exceptList.push_back(&stub._notelocationClock);
        exceptList.push_back(&stub._notelocationMeasure);
        exceptList.push_back(&stub._notelocationBeat);
        exceptList.push_back(&stub._notelocationTick);
        assertDefaultsExcept(&stub, exceptList);

        CPPUNIT_ASSERT_EQUAL(string(""), stub._notelocationClock);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._notelocationMeasure);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._notelocationBeat);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._notelocationTick);
    }

    void testCommitTwoNotesVibratoTypeDifferers() {
        Event noteA = note;
        Event noteB = note;
        noteB.vibratoHandle = Handle(HandleType::VIBRATO);
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertDefaultsExcept(&stub, &stub._vibratoType);
        CPPUNIT_ASSERT_EQUAL(0, stub._vibratoType);
    }

    void testCommitTwoNotesVibratoLengthDiffers() {
        Event noteA = note;
        Event noteB = note;
        noteB.vibratoHandle = Handle(HandleType::VIBRATO);
        noteB.vibratoHandle.setLength(120);
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        vector<void *> exceptList;
        exceptList.push_back(&stub._vibratoType);
        exceptList.push_back(&stub._vibratoLength);
        assertDefaultsExcept(&stub, exceptList);
        CPPUNIT_ASSERT_EQUAL(0, stub._vibratoType);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._vibratoLength);
    }

    CPPUNIT_TEST_SUITE(PropertyValueProxyTest);
    CPPUNIT_TEST(testCommitNoNote);
    CPPUNIT_TEST(testCommitOneNote);
    CPPUNIT_TEST(testCommitTwoNotesWithSameProperties);
    CPPUNIT_TEST(testCommitTwoNotesLyricPhraseDiffers);
    CPPUNIT_TEST(testCommitTwoNotesLyricPhoneticSymbolDiffers);
    CPPUNIT_TEST(testCommitTwoNotesLyricProtectDiffers);
    CPPUNIT_TEST(testCommitTwoNotesNoteLengthDiffers);
    CPPUNIT_TEST(testCommitTwoNotesNoteNumberDiffers);
    CPPUNIT_TEST(testCommitTwoNotesNotelocationClockDiffers);
    CPPUNIT_TEST(testCommitTwoNotesVibratoTypeDifferers);
    CPPUNIT_TEST(testCommitTwoNotesVibratoLengthDiffers);
    CPPUNIT_TEST_SUITE_END();

private:
    void assertDefaultsExcept(PropertyValueProxyStub *stub, void *except) {
        vector<void *> exceptList;
        exceptList.push_back(except);
        assertDefaultsExcept(stub, exceptList);
    }

    void assertDefaultsExcept(PropertyValueProxyStub *stub, const vector<void *> &exceptList) {
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhrase) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("ra"), stub->_lyricPhrase);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhoneticSymbol) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("4 a"), stub->_lyricPhoneticSymbol);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricConsonantAdjustment) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("64 0"), stub->_lyricConsonantAdjustment);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricProtect) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(1, stub->_lyricProtect);

        if (find(exceptList.begin(), exceptList.end(), &stub->_noteLength) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("480"), stub->_noteLength);
        if (find(exceptList.begin(), exceptList.end(), &stub->_noteNumber) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("60"), stub->_noteNumber);

        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationClock) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("480"), stub->_notelocationClock);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationMeasure) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("0"), stub->_notelocationMeasure);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationBeat) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("2"), stub->_notelocationBeat);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationTick) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("0"), stub->_notelocationTick);

        if (find(exceptList.begin(), exceptList.end(), &stub->_vibratoType) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(0, stub->_vibratoType);
        if (find(exceptList.begin(), exceptList.end(), &stub->_vibratoLength) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("0"), stub->_vibratoLength);
    }
};

REGISTER_TEST_SUITE(PropertyValueProxyTest);
