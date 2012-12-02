#include "../../Util.hpp"
#include "../../../sequence/io/XVSQFileReader.hpp"
#include "../../../sequence/io/SAXAdapter.hpp"
#include "../../../vsq/FileInputStream.hpp"
#include <cstdio>

using namespace VSQ_NS;
using namespace cadencii;
using namespace std;

/**
 * @brief A stub for SAXAdapter.
 *    This stub reads a text file like:
 *    <pre>
 *        startElement    VsqFileEx
 *        endElement      VsqFileEx
 *    </pre>
 *    and invokes SAX callback method.
 */
class SAXAdapterStub : public SAXAdapter {
private:
    string fixturePath;

public:
    explicit SAXAdapterStub(const string &fixturePath) :
        SAXAdapter(0), fixturePath(fixturePath) {
    }

    void start() {
        FILE *handle = fopen(fixturePath.c_str(), "r");
        if (!handle) return;

        const size_t BUFFER_LENGTH = 4096;
        char *line = new char[BUFFER_LENGTH];
        memset((void *)line, 0, BUFFER_LENGTH);
        while (fgets(line, sizeof(char) * BUFFER_LENGTH, handle)) {
            string parameter(line);
            string::size_type index = parameter.find('\t');
            if (index == string::npos) continue;
            string name = parameter.substr(0, index);
            string message = parameter.substr(index + 1);
            if (message.find_last_of('\r') != string::npos) {
                message = message.erase(message.find_last_of('\r'));
            }
            if (message.find_last_of('\n') != string::npos) {
                message = message.erase(message.find_last_of('\n'));
            }

            if (string("startElement") == name) {
                onStartElement(message);
            } else if (string("endElement") == name) {
                onEndElement(message);
            } else if (string("characters") == name) {
                onCharacters(message);
            }
        }
        fclose(handle);
        delete [] line;
    }
};

class XVSQFileReaderTest : public CppUnit::TestCase {
public:
    void read() {
        XVSQFileReader reader;
        Sequence sequence;
        SAXAdapterStub adapter(TestUtil::getFixtureRootPath() + "/fixture/sample.xvsq.sax.log");
        reader.read(&sequence, &adapter);

        CPPUNIT_ASSERT_EQUAL((size_t)2, sequence.tracks()->size());

        {
            // 1st track
            const Track *track = sequence.track(0);
            {
                const Common *common = track->common();
                CPPUNIT_ASSERT_EQUAL(string("DSB301"), common->version);
                CPPUNIT_ASSERT_EQUAL(string("Voice1"), common->name);
                CPPUNIT_ASSERT_EQUAL(string("179,181,123"), common->color);
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::DynamicsMode::EXPERT, common->dynamicsMode);
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::PlayMode::PLAY_WITH_SYNTH, common->playMode());
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::PlayMode::PLAY_WITH_SYNTH, common->lastPlayMode());
            }
            CPPUNIT_ASSERT_EQUAL(2, track->events()->size());
            {
                const Event *item = track->events()->get(0);
                CPPUNIT_ASSERT_EQUAL(0, item->id);
                CPPUNIT_ASSERT_EQUAL((tick_t)0, item->clock);
                CPPUNIT_ASSERT_EQUAL(EventType::SINGER, item->type);
                CPPUNIT_ASSERT_EQUAL(0, item->note);
                CPPUNIT_ASSERT_EQUAL(0, item->dynamics);
                CPPUNIT_ASSERT_EQUAL(0, item->pmBendDepth);
                CPPUNIT_ASSERT_EQUAL(0, item->pmBendLength);
                CPPUNIT_ASSERT_EQUAL(0, item->pmbPortamentoUse);
                CPPUNIT_ASSERT_EQUAL(0, item->demDecGainRate);
                CPPUNIT_ASSERT_EQUAL(0, item->demAccent);
                CPPUNIT_ASSERT_EQUAL(0, item->vibratoDelay);
                CPPUNIT_ASSERT_EQUAL(10, item->pMeanOnsetFirstNote);
                CPPUNIT_ASSERT_EQUAL(12, item->vMeanNoteTransition);
                CPPUNIT_ASSERT_EQUAL(24, item->d4mean);
                CPPUNIT_ASSERT_EQUAL(12, item->pMeanEndingNote);
                CPPUNIT_ASSERT_EQUAL((tick_t)0, item->getLength());
                {
                    const Handle *handle = &item->singerHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::SINGER, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(string("hoge"), handle->caption);
                    CPPUNIT_ASSERT_EQUAL(string("$07010000"), handle->iconId);
                    CPPUNIT_ASSERT_EQUAL(string("Miku"), handle->ids);
                    CPPUNIT_ASSERT_EQUAL(0, handle->index);
                    CPPUNIT_ASSERT_EQUAL((tick_t)1, handle->getLength());
                    CPPUNIT_ASSERT_EQUAL(0, handle->original);
                    CPPUNIT_ASSERT_EQUAL(0, handle->program);
                    CPPUNIT_ASSERT_EQUAL(0, handle->language);
                }
                {
                    const Handle *handle = &item->lyricHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->vibratoHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
            }
            {
                const Event *item = track->events()->get(1);
                CPPUNIT_ASSERT_EQUAL(1, item->id);
                CPPUNIT_ASSERT_EQUAL((tick_t)1920, item->clock);
                CPPUNIT_ASSERT_EQUAL(EventType::NOTE, item->type);
                CPPUNIT_ASSERT_EQUAL(72, item->note);
                CPPUNIT_ASSERT_EQUAL(64, item->dynamics);
                CPPUNIT_ASSERT_EQUAL(8, item->pmBendDepth);
                CPPUNIT_ASSERT_EQUAL(0, item->pmBendLength);
                CPPUNIT_ASSERT_EQUAL(3, item->pmbPortamentoUse);
                CPPUNIT_ASSERT_EQUAL(50, item->demDecGainRate);
                CPPUNIT_ASSERT_EQUAL(50, item->demAccent);
                CPPUNIT_ASSERT_EQUAL(320, item->vibratoDelay);
                CPPUNIT_ASSERT_EQUAL(10, item->pMeanOnsetFirstNote);
                CPPUNIT_ASSERT_EQUAL(12, item->vMeanNoteTransition);
                CPPUNIT_ASSERT_EQUAL(24, item->d4mean);
                CPPUNIT_ASSERT_EQUAL(12, item->pMeanEndingNote);
                CPPUNIT_ASSERT_EQUAL((tick_t)960, item->getLength());
                {
                    const Handle *handle = &item->singerHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->lyricHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::LYRIC, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(1, handle->getLyricCount());
                    const Lyric lyric = item->lyricHandle.getLyricAt(0);
                    CPPUNIT_ASSERT_EQUAL(string("あ"), lyric.phrase);
                    CPPUNIT_ASSERT_EQUAL(1.0, lyric.lengthRatio);
                    CPPUNIT_ASSERT_EQUAL(false, lyric.isProtected);
                    CPPUNIT_ASSERT_EQUAL(string("0"), lyric.getConsonantAdjustment());
                    CPPUNIT_ASSERT_EQUAL(string("a"), lyric.getPhoneticSymbol());
                }
                {
                    const Handle *handle = &item->vibratoHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::VIBRATO, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(string("$04040001"), handle->iconId);
                    CPPUNIT_ASSERT_EQUAL(string(""), handle->ids);
                    CPPUNIT_ASSERT_EQUAL(1, handle->original);
                    CPPUNIT_ASSERT_EQUAL(string("[Normal] Type 1"), handle->caption);
                    CPPUNIT_ASSERT_EQUAL(50, handle->startRate);
                    CPPUNIT_ASSERT_EQUAL(64, handle->startDepth);
                    CPPUNIT_ASSERT_EQUAL((tick_t)66, handle->getLength());
                    {
                        const VibratoBPList list = handle->rateBP;
                        CPPUNIT_ASSERT_EQUAL(21, list.size());
                        CPPUNIT_ASSERT_EQUAL(0.0, list.get(0).x); CPPUNIT_ASSERT_EQUAL(104, list.get(0).y);
                        CPPUNIT_ASSERT_EQUAL(0.0421875, list.get(1).x); CPPUNIT_ASSERT_EQUAL(101, list.get(1).y);
                        CPPUNIT_ASSERT_EQUAL(0.0984375, list.get(2).x); CPPUNIT_ASSERT_EQUAL(99, list.get(2).y);
                        CPPUNIT_ASSERT_EQUAL(0.1453125, list.get(3).x); CPPUNIT_ASSERT_EQUAL(97, list.get(3).y);
                        CPPUNIT_ASSERT_EQUAL(0.1921875, list.get(4).x); CPPUNIT_ASSERT_EQUAL(95, list.get(4).y);
                        CPPUNIT_ASSERT_EQUAL(0.2484375, list.get(5).x); CPPUNIT_ASSERT_EQUAL(92, list.get(5).y);
                        CPPUNIT_ASSERT_EQUAL(0.2953125, list.get(6).x); CPPUNIT_ASSERT_EQUAL(89, list.get(6).y);
                        CPPUNIT_ASSERT_EQUAL(0.3421875, list.get(7).x); CPPUNIT_ASSERT_EQUAL(87, list.get(7).y);
                        CPPUNIT_ASSERT_EQUAL(0.3984375, list.get(8).x); CPPUNIT_ASSERT_EQUAL(83, list.get(8).y);
                        CPPUNIT_ASSERT_EQUAL(0.4453125, list.get(9).x); CPPUNIT_ASSERT_EQUAL(80, list.get(9).y);
                        CPPUNIT_ASSERT_EQUAL(0.4921875, list.get(10).x); CPPUNIT_ASSERT_EQUAL(77, list.get(10).y);
                        CPPUNIT_ASSERT_EQUAL(0.5484375, list.get(11).x); CPPUNIT_ASSERT_EQUAL(75, list.get(11).y);
                        CPPUNIT_ASSERT_EQUAL(0.5953125, list.get(12).x); CPPUNIT_ASSERT_EQUAL(73, list.get(12).y);
                        CPPUNIT_ASSERT_EQUAL(0.6421875, list.get(13).x); CPPUNIT_ASSERT_EQUAL(70, list.get(13).y);
                        CPPUNIT_ASSERT_EQUAL(0.6984375, list.get(14).x); CPPUNIT_ASSERT_EQUAL(68, list.get(14).y);
                        CPPUNIT_ASSERT_EQUAL(0.7453125, list.get(15).x); CPPUNIT_ASSERT_EQUAL(65, list.get(15).y);
                        CPPUNIT_ASSERT_EQUAL(0.7921875, list.get(16).x); CPPUNIT_ASSERT_EQUAL(61, list.get(16).y);
                        CPPUNIT_ASSERT_EQUAL(0.8484375, list.get(17).x); CPPUNIT_ASSERT_EQUAL(58, list.get(17).y);
                        CPPUNIT_ASSERT_EQUAL(0.8953125, list.get(18).x); CPPUNIT_ASSERT_EQUAL(55, list.get(18).y);
                        CPPUNIT_ASSERT_EQUAL(0.9421875, list.get(19).x); CPPUNIT_ASSERT_EQUAL(51, list.get(19).y);
                        CPPUNIT_ASSERT_EQUAL(0.9984375, list.get(20).x); CPPUNIT_ASSERT_EQUAL(46, list.get(20).y);
                    }
                    {
                        const VibratoBPList list = handle->depthBP;
                        CPPUNIT_ASSERT_EQUAL(21, list.size());
                        CPPUNIT_ASSERT_EQUAL(0.0000000, list.get(0).x); CPPUNIT_ASSERT_EQUAL(25, list.get(0).y);
                        CPPUNIT_ASSERT_EQUAL(0.0421875, list.get(1).x); CPPUNIT_ASSERT_EQUAL(27, list.get(1).y);
                        CPPUNIT_ASSERT_EQUAL(0.0984375, list.get(2).x); CPPUNIT_ASSERT_EQUAL(29, list.get(2).y);
                        CPPUNIT_ASSERT_EQUAL(0.1453125, list.get(3).x); CPPUNIT_ASSERT_EQUAL(32, list.get(3).y);
                        CPPUNIT_ASSERT_EQUAL(0.1921875, list.get(4).x); CPPUNIT_ASSERT_EQUAL(34, list.get(4).y);
                        CPPUNIT_ASSERT_EQUAL(0.2484375, list.get(5).x); CPPUNIT_ASSERT_EQUAL(36, list.get(5).y);
                        CPPUNIT_ASSERT_EQUAL(0.2953125, list.get(6).x); CPPUNIT_ASSERT_EQUAL(38, list.get(6).y);
                        CPPUNIT_ASSERT_EQUAL(0.3421875, list.get(7).x); CPPUNIT_ASSERT_EQUAL(41, list.get(7).y);
                        CPPUNIT_ASSERT_EQUAL(0.3984375, list.get(8).x); CPPUNIT_ASSERT_EQUAL(45, list.get(8).y);
                        CPPUNIT_ASSERT_EQUAL(0.4453125, list.get(9).x); CPPUNIT_ASSERT_EQUAL(47, list.get(9).y);
                        CPPUNIT_ASSERT_EQUAL(0.4921875, list.get(10).x); CPPUNIT_ASSERT_EQUAL(50, list.get(10).y);
                        CPPUNIT_ASSERT_EQUAL(0.5484375, list.get(11).x); CPPUNIT_ASSERT_EQUAL(52, list.get(11).y);
                        CPPUNIT_ASSERT_EQUAL(0.5953125, list.get(12).x); CPPUNIT_ASSERT_EQUAL(55, list.get(12).y);
                        CPPUNIT_ASSERT_EQUAL(0.6421875, list.get(13).x); CPPUNIT_ASSERT_EQUAL(58, list.get(13).y);
                        CPPUNIT_ASSERT_EQUAL(0.6984375, list.get(14).x); CPPUNIT_ASSERT_EQUAL(61, list.get(14).y);
                        CPPUNIT_ASSERT_EQUAL(0.7453125, list.get(15).x); CPPUNIT_ASSERT_EQUAL(64, list.get(15).y);
                        CPPUNIT_ASSERT_EQUAL(0.7921875, list.get(16).x); CPPUNIT_ASSERT_EQUAL(68, list.get(16).y);
                        CPPUNIT_ASSERT_EQUAL(0.8484375, list.get(17).x); CPPUNIT_ASSERT_EQUAL(71, list.get(17).y);
                        CPPUNIT_ASSERT_EQUAL(0.8953125, list.get(18).x); CPPUNIT_ASSERT_EQUAL(73, list.get(18).y);
                        CPPUNIT_ASSERT_EQUAL(0.9421875, list.get(19).x); CPPUNIT_ASSERT_EQUAL(76, list.get(19).y);
                        CPPUNIT_ASSERT_EQUAL(0.9984375, list.get(20).x); CPPUNIT_ASSERT_EQUAL(78, list.get(20).y);
                    }
                }
            }
        }

        {
            // 2nd track
            const Track *track = sequence.track(1);
            {
                const Common *common = track->common();
                CPPUNIT_ASSERT_EQUAL(string("DSB300"), common->version);
                CPPUNIT_ASSERT_EQUAL(string("Voice2"), common->name);
                CPPUNIT_ASSERT_EQUAL(string("179,181,123"), common->color);
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::DynamicsMode::STANDARD, common->dynamicsMode);
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::PlayMode::OFF, common->playMode());
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::PlayMode::PLAY_AFTER_SYNTH, common->lastPlayMode());
            }
            CPPUNIT_ASSERT_EQUAL(2, track->events()->size());
            {
                const Event *item = track->events()->get(0);
                CPPUNIT_ASSERT_EQUAL(2, item->id);
                CPPUNIT_ASSERT_EQUAL((tick_t)0, item->clock);
                CPPUNIT_ASSERT_EQUAL(EventType::SINGER, item->type);
                CPPUNIT_ASSERT_EQUAL(0, item->note);
                CPPUNIT_ASSERT_EQUAL(0, item->dynamics);
                CPPUNIT_ASSERT_EQUAL(0, item->pmBendDepth);
                CPPUNIT_ASSERT_EQUAL(1, item->pmBendLength);
                CPPUNIT_ASSERT_EQUAL(0, item->pmbPortamentoUse);
                CPPUNIT_ASSERT_EQUAL(0, item->demDecGainRate);
                CPPUNIT_ASSERT_EQUAL(0, item->demAccent);
                CPPUNIT_ASSERT_EQUAL(0, item->vibratoDelay);
                CPPUNIT_ASSERT_EQUAL(11, item->pMeanOnsetFirstNote);
                CPPUNIT_ASSERT_EQUAL(13, item->vMeanNoteTransition);
                CPPUNIT_ASSERT_EQUAL(23, item->d4mean);
                CPPUNIT_ASSERT_EQUAL(11, item->pMeanEndingNote);
                CPPUNIT_ASSERT_EQUAL((tick_t)0, item->getLength());
                {
                    const Handle *handle = &item->singerHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::SINGER, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(string(""), handle->caption);
                    CPPUNIT_ASSERT_EQUAL(string("$07010000"), handle->iconId);
                    CPPUNIT_ASSERT_EQUAL(string("Mike"), handle->ids);
                    CPPUNIT_ASSERT_EQUAL(1, handle->index);
                    CPPUNIT_ASSERT_EQUAL((tick_t)1, handle->getLength());
                    CPPUNIT_ASSERT_EQUAL(1, handle->original);
                    CPPUNIT_ASSERT_EQUAL(2, handle->program);
                    CPPUNIT_ASSERT_EQUAL(1, handle->language);
                }
                {
                    const Handle *handle = &item->lyricHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->vibratoHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
            }
            {
                const Event *item = track->events()->get(1);
                CPPUNIT_ASSERT_EQUAL(3, item->id);
                CPPUNIT_ASSERT_EQUAL((tick_t)1921, item->clock);
                CPPUNIT_ASSERT_EQUAL(EventType::NOTE, item->type);
                CPPUNIT_ASSERT_EQUAL(76, item->note);
                CPPUNIT_ASSERT_EQUAL(64, item->dynamics);
                CPPUNIT_ASSERT_EQUAL(8, item->pmBendDepth);
                CPPUNIT_ASSERT_EQUAL(2, item->pmBendLength);
                CPPUNIT_ASSERT_EQUAL(3, item->pmbPortamentoUse);
                CPPUNIT_ASSERT_EQUAL(50, item->demDecGainRate);
                CPPUNIT_ASSERT_EQUAL(50, item->demAccent);
                CPPUNIT_ASSERT_EQUAL(320, item->vibratoDelay);
                CPPUNIT_ASSERT_EQUAL(12, item->pMeanOnsetFirstNote);
                CPPUNIT_ASSERT_EQUAL(14, item->vMeanNoteTransition);
                CPPUNIT_ASSERT_EQUAL(22, item->d4mean);
                CPPUNIT_ASSERT_EQUAL(10, item->pMeanEndingNote);
                CPPUNIT_ASSERT_EQUAL((tick_t)960, item->getLength());
                {
                    const Handle *handle = &item->singerHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->lyricHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::LYRIC, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(2, handle->getLyricCount());
                    {
                        const Lyric lyric = item->lyricHandle.getLyricAt(0);
                        CPPUNIT_ASSERT_EQUAL(string("は"), lyric.phrase);
                        CPPUNIT_ASSERT_EQUAL(0.5, lyric.lengthRatio);
                        CPPUNIT_ASSERT_EQUAL(true, lyric.isProtected);
                        CPPUNIT_ASSERT_EQUAL(string("64,0"), lyric.getConsonantAdjustment());
                        CPPUNIT_ASSERT_EQUAL(string("h a"), lyric.getPhoneticSymbol());
                    }
                    {
                        const Lyric lyric = item->lyricHandle.getLyricAt(1);
                        CPPUNIT_ASSERT_EQUAL(string("ほ"), lyric.phrase);
                        CPPUNIT_ASSERT_EQUAL(0.5, lyric.lengthRatio);
                        CPPUNIT_ASSERT_EQUAL(true, lyric.isProtected);
                        CPPUNIT_ASSERT_EQUAL(string("64,0"), lyric.getConsonantAdjustment());
                        CPPUNIT_ASSERT_EQUAL(string("h o"), lyric.getPhoneticSymbol());
                    }
                }
                {
                    const Handle *handle = &item->vibratoHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::VIBRATO, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(string("$04040001"), handle->iconId);
                    CPPUNIT_ASSERT_EQUAL(string("a"), handle->ids);
                    CPPUNIT_ASSERT_EQUAL(0, handle->original);
                    CPPUNIT_ASSERT_EQUAL(string("[Normal] Type 1"), handle->caption);
                    CPPUNIT_ASSERT_EQUAL(0, handle->rateBP.size());
                    CPPUNIT_ASSERT_EQUAL(0, handle->depthBP.size());
                    CPPUNIT_ASSERT_EQUAL(50, handle->startRate);
                    CPPUNIT_ASSERT_EQUAL(63, handle->startDepth);
                    CPPUNIT_ASSERT_EQUAL((tick_t)66, handle->getLength());
                }
            }
        }

        //TODO(kbinani): more assertion
    }

    CPPUNIT_TEST_SUITE(XVSQFileReaderTest);
    CPPUNIT_TEST(read);
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(XVSQFileReaderTest);
