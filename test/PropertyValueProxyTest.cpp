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
    Event note1;
    PropertyValueProxyStub stub;

public:
    void setUp() {
        sequence = Sequence("hoge", 1, 4, 4, 500000);
        note = Event(480, EventType::NOTE);
        note.lyricHandle.setLyricAt(0, Lyric("ra", "4 a"));
        note.setLength(480);
        note.note = 60;

        note1 = Event(2281, EventType::NOTE);
        Lyric l("i", "i");
        l.isProtected = true;
        note1.lyricHandle.setLyricAt(0, l);
        note1.setLength(481);
        note1.note = 61;
        note1.vibratoHandle = Handle(HandleType::VIBRATO);
        note1.vibratoHandle.iconId = "$04040003";
        note1.vibratoHandle.setLength(100);
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

        assertEqualToNoteExcept(&stub, 0);
    }

    void testCommitTwoNotesWithSameProperties() {
        Event noteA = note;
        Event noteB = note;
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertEqualToNoteExcept(&stub, 0);
    }

    void testCommitTwoNotesLyricPhraseDiffers() {
        Event noteA = note;
        Event noteB = note;
        noteB.lyricHandle.setLyricAt(0, Lyric("wa", "4 a"));
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertEqualToNoteExcept(&stub, &stub._lyricPhrase);
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
        assertEqualToNoteExcept(&stub, exceptList);

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

        assertEqualToNoteExcept(&stub, &stub._lyricProtect);
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

        assertEqualToNoteExcept(&stub, &stub._noteLength);
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

        assertEqualToNoteExcept(&stub, &stub._noteNumber);
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
        assertEqualToNoteExcept(&stub, exceptList);

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

        assertEqualToNoteExcept(&stub, &stub._vibratoType);
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
        assertEqualToNoteExcept(&stub, exceptList);
        CPPUNIT_ASSERT_EQUAL(0, stub._vibratoType);
        CPPUNIT_ASSERT_EQUAL(string(""), stub._vibratoLength);
    }

    void testCommitAllPropertyDiffers() {
        Event noteA = note;
        Event noteB = note1;
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        assertEqualToDefault(&stub, 0);
    }

    void testFetchPropertyAll() {
        Event noteA = note;
        Lyric l = noteA.lyricHandle.getLyricAt(0);
        l.isProtected = true;
        noteA.lyricHandle.setLyricAt(0, l);
        stub.begin();
        stub.add(&noteA, &sequence);
        stub.commit();

        Event actual(0, EventType::NOTE);
        stub.fetchProperty(&actual, &sequence);

        Lyric actualLyric = actual.lyricHandle.getLyricAt(0);
        CPPUNIT_ASSERT_EQUAL(string("ra"), actualLyric.phrase);
        CPPUNIT_ASSERT_EQUAL(string("4 a"), actualLyric.getPhoneticSymbol());
        CPPUNIT_ASSERT_EQUAL(string("64,0"), actualLyric.getConsonantAdjustment());
        CPPUNIT_ASSERT_EQUAL(true, actualLyric.isProtected);
    }

    void testFetchPropertyPhrase() {
        Event noteA = note;
        Event noteB = note1;
        Lyric l("wa", "w a");
        l.isProtected = true;
        noteA.lyricHandle.setLyricAt(0, l);
        noteB.lyricHandle.setLyricAt(0, l);

        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        Event actual(0, EventType::NOTE);
        stub.fetchProperty(&actual, &sequence);

        CPPUNIT_ASSERT_EQUAL(string("wa"), actual.lyricHandle.getLyricAt(0).phrase);
        CPPUNIT_ASSERT_EQUAL(string("w a"), actual.lyricHandle.getLyricAt(0).getPhoneticSymbol());
        CPPUNIT_ASSERT_EQUAL(string("64,0"), actual.lyricHandle.getLyricAt(0).getConsonantAdjustment());
        CPPUNIT_ASSERT_EQUAL(true, actual.lyricHandle.getLyricAt(0).isProtected);
    }

    void testFetchPropertyNote() {
        Event noteA = note;
        Event noteB = note1;

        noteA.note = 67;
        noteB.note = 67;
        noteA.setLength(2);
        noteB.setLength(2);

        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        Event actual(0, EventType::NOTE);
        stub.fetchProperty(&actual, &sequence);

        CPPUNIT_ASSERT_EQUAL(67, actual.note);
        CPPUNIT_ASSERT_EQUAL((tick_t)2, actual.getLength());
    }

    void testFetchPropertyModifiedNotelocation() {
        Event noteA = note;
        Event noteB = note1;

        noteA.clock = 0;  // clock = 0, measure = 0, beat = 1, tick = 0
        noteB.clock = 1;  // clock = 1, measure = 0, beat = 1, tick = 1
                          // clock = -, measure = 0, beat = 1, tick = -

        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        Event actual(0, EventType::NOTE);
        actual.clock = 2402;  // clock = 2402, measure = 1, beat = 2, tick = 2
                              //                      -> 0,     -> 1,        2
        stub.fetchProperty(&actual, &sequence);

        CPPUNIT_ASSERT_EQUAL((tick_t)2, actual.clock);
    }

    void testFetchPropertyNotModifiedNotelocation() {
        Event noteA = note;
        Event noteB = note1;

        noteA.clock = 13;
        noteB.clock = 13;

        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        Event actual(0, EventType::NOTE);
        actual.clock = 2402;
        stub.fetchProperty(&actual, &sequence);

        CPPUNIT_ASSERT_EQUAL((tick_t)13, actual.clock);
    }

    void testFetchPropertyVibrato() {
        Event noteA = note;
        Event noteB = note1;

        Handle vibrato = noteB.vibratoHandle;
        noteA.vibratoHandle = vibrato;

        stub.begin();
        stub.add(&noteA, &sequence);
        stub.add(&noteB, &sequence);
        stub.commit();

        Event actual(0, EventType::NOTE);
        stub.fetchProperty(&actual, &sequence);

        CPPUNIT_ASSERT_EQUAL(HandleType::VIBRATO, actual.vibratoHandle.getHandleType());
        CPPUNIT_ASSERT_EQUAL(string("$04040003"), actual.vibratoHandle.iconId);
        CPPUNIT_ASSERT_EQUAL((tick_t)100, actual.vibratoHandle.getLength());
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
    CPPUNIT_TEST(testCommitAllPropertyDiffers);
    CPPUNIT_TEST(testFetchPropertyAll);
    CPPUNIT_TEST(testFetchPropertyPhrase);
    CPPUNIT_TEST(testFetchPropertyNote);
    CPPUNIT_TEST(testFetchPropertyModifiedNotelocation);
    CPPUNIT_TEST(testFetchPropertyNotModifiedNotelocation);
    CPPUNIT_TEST(testFetchPropertyVibrato);
    CPPUNIT_TEST_SUITE_END();

private:
    void assertEqualToDefault(PropertyValueProxyStub *stub, void * except) {
        vector<void *> exceptList;
        exceptList.push_back(except);
        assertEqualToDefault(stub, exceptList);
    }

    void assertEqualToDefault(PropertyValueProxyStub *stub, const vector<void *> &exceptList) {
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhrase) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_lyricPhrase);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhoneticSymbol) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_lyricPhoneticSymbol);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricConsonantAdjustment) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_lyricConsonantAdjustment);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricProtect) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(0, stub->_lyricProtect);

        if (find(exceptList.begin(), exceptList.end(), &stub->_noteLength) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_noteLength);
        if (find(exceptList.begin(), exceptList.end(), &stub->_noteNumber) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_noteNumber);

        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationClock) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_notelocationClock);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationMeasure) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_notelocationMeasure);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationBeat) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_notelocationBeat);
        if (find(exceptList.begin(), exceptList.end(), &stub->_notelocationTick) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_notelocationTick);

        if (find(exceptList.begin(), exceptList.end(), &stub->_vibratoType) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(0, stub->_vibratoType);
        if (find(exceptList.begin(), exceptList.end(), &stub->_vibratoLength) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string(""), stub->_vibratoLength);
    }

    void assertEqualToNoteExcept(PropertyValueProxyStub *stub, void *except) {
        vector<void *> exceptList;
        exceptList.push_back(except);
        assertEqualToNoteExcept(stub, exceptList);
    }

    void assertEqualToNoteExcept(PropertyValueProxyStub *stub, const vector<void *> &exceptList) {
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhrase) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("ra"), stub->_lyricPhrase);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricPhoneticSymbol) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("4 a"), stub->_lyricPhoneticSymbol);
        if (find(exceptList.begin(), exceptList.end(), &stub->_lyricConsonantAdjustment) == exceptList.end()) CPPUNIT_ASSERT_EQUAL(string("64,0"), stub->_lyricConsonantAdjustment);
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
