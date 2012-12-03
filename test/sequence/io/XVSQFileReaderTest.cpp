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

        CPPUNIT_ASSERT_EQUAL((size_t)3, sequence.tracks()->size());

        // tempo table
        {
            CPPUNIT_ASSERT_EQUAL(2, sequence.tempoList.size());
            {
                const Tempo tempo = sequence.tempoList.get(0);
                CPPUNIT_ASSERT_EQUAL((tick_t)0, tempo.clock);
                CPPUNIT_ASSERT_EQUAL(500000, tempo.tempo);
                CPPUNIT_ASSERT_EQUAL(0.0, tempo.getTime());
            }
            {
                const Tempo tempo = sequence.tempoList.get(1);
                CPPUNIT_ASSERT_EQUAL((tick_t)1920, tempo.clock);
                CPPUNIT_ASSERT_EQUAL(250000, tempo.tempo);
                CPPUNIT_ASSERT_EQUAL(2.0, tempo.getTime());
            }
        }

        // timesig table
        {
            CPPUNIT_ASSERT_EQUAL(2, sequence.timesigList.size());
            {
                const Timesig timesig = sequence.timesigList.get(0);
                CPPUNIT_ASSERT_EQUAL(4, timesig.numerator);
                CPPUNIT_ASSERT_EQUAL(4, timesig.denominator);
                CPPUNIT_ASSERT_EQUAL(0, timesig.barCount);
                CPPUNIT_ASSERT_EQUAL((tick_t)0, timesig.getClock());
            }
            {
                const Timesig timesig = sequence.timesigList.get(1);
                CPPUNIT_ASSERT_EQUAL(3, timesig.numerator);
                CPPUNIT_ASSERT_EQUAL(4, timesig.denominator);
                CPPUNIT_ASSERT_EQUAL(1, timesig.barCount);
                CPPUNIT_ASSERT_EQUAL((tick_t)1920, timesig.getClock());
            }
        }

        // 1st track
        {
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

            // 1st event of 1st track
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
                {
                    const Handle *handle = &item->noteHeadHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->iconDynamicsHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
            }

            // 2nd event of 1st track
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
                {
                    const Handle *handle = &item->noteHeadHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->iconDynamicsHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
            }

            // control curves
            {
                // pit
                {
                    const BPList *curve = track->curve("PIT");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("pit"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(8191, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(-8192, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(95, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2710=-2483,2740=-2279,2770=-2061,2800=-1829,2830=-1581,2860=-1319,2890=-1043,2920=-753,2950=-450,2980=-135,3010=190,3040=525,3070=867,3100=1213,3130=1562,3160=1909,3190=2251,3220=2584,3250=2906,3280=3212,3310=3499,3340=3763,3370=4003,3400=4215,3430=4398,3460=4550,3490=4672,3520=4763,3550=4822,3580=4851,3610=4850,3640=4820,3670=4763,3700=4678,3730=4569,3760=4435,3790=4278,3820=4100,3850=3901,3880=3682,3910=3452,3940=3233,3970=3026,4000=2829,4030=2641,4060=2462,4090=2289,4120=2123,4150=1964,4180=1809,4210=1660,4240=1516,4270=1376,4300=1240,4330=1107,4360=979,4390=854,4420=732,4450=613,4480=497,4510=383,4540=273,4570=164,4600=59,4630=-44,4660=-145,4690=-244,4720=-341,4750=-436,4780=-530,4810=-621,4840=-711,4870=-798,4900=-884,4930=-969,4960=-1052,4990=-1133,5020=-1213,5050=-1291,5080=-1368,5110=-1443,5140=-1517,5170=-1589,5200=-1660,5230=-1730,5260=-1798,5290=-1865,5320=-1930,5350=-1994,5380=-2057,5410=-2118,5440=-2178,5470=-2235,5500=-2291,5530=-2345"),
                                         curve->getData());
                }
                // pbs
                {
                    const BPList *curve = track->curve("PBS");
                    CPPUNIT_ASSERT_EQUAL(2, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("pbs"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(24, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(57, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2628=10,2658=10,2688=10,2718=11,2748=11,2778=11,2808=12,2838=13,2868=13,2898=14,2928=15,2958=16,2988=17,3018=18,3048=19,3078=20,3108=22,3138=23,3168=24,3198=24,3228=24,3258=24,3288=24,3318=24,3348=24,3378=24,3408=24,3438=24,3468=24,3498=24,3528=24,3558=24,3588=24,3618=24,3648=24,3678=24,3708=24,3738=24,3768=24,3798=24,3828=24,3858=24,3888=24,3918=24,3948=24,3978=24,4008=24,4038=24,4068=24,4098=24,4128=23,4158=22,4188=21,4218=19,4248=18,4278=17,4308=16"),
                                         curve->getData());
                }
                // dyn
                {
                    const BPList *curve = track->curve("DYN");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("dyn"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(45, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2097=41,2127=42,2157=44,2187=46,2217=48,2247=51,2277=53,2307=56,2337=58,2367=61,2397=64,2427=66,2457=69,2487=72,2517=75,2547=77,2577=79,2607=81,2637=83,2667=85,2697=86,2727=87,2757=88,2787=89,2817=89,2847=89,2877=89,2907=89,2937=89,2967=89,2997=88,3027=87,3057=87,3087=86,3117=85,3147=84,3177=83,3207=82,3237=81,3267=79,3297=78,3327=77,3357=75,3387=74,3417=73"),
                                         curve->getData());
                }
                // bre
                {
                    const BPList *curve = track->curve("BRE");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("bre"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(143, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2134=40,2164=41,2194=42,2224=43,2254=44,2284=45,2314=46,2344=47,2374=48,2404=48,2434=49,2464=50,2494=51,2524=51,2554=52,2584=52,2614=53,2644=53,2674=54,2704=54,2734=55,2764=55,2794=56,2824=56,2854=56,2884=56,2914=57,2944=57,2974=57,3004=57,3034=57,3064=57,3094=57,3124=57,3154=57,3184=57,3214=57,3244=57,3274=56,3304=56,3334=56,3364=55,3394=55,3424=54,3454=53,3484=52,3514=52,3544=51,3574=49,3604=48,3634=46,3664=45,3694=42,3724=40,3754=36,3784=33,3814=31,3844=30,3874=29,3904=29,3934=30,3964=31,3994=33,4024=35,4054=37,4084=39,4114=42,4144=45,4174=48,4204=51,4234=54,4264=57,4294=60,4324=63,4354=66,4384=69,4414=72,4444=74,4474=77,4504=79,4534=82,4564=84,4594=85,4624=87,4654=88,4684=89,4714=89,5804=45,5834=47,5864=50,5894=52,5924=55,5954=57,5984=59,6014=62,6044=64,6074=66,6104=68,6134=70,6164=72,6194=74,6224=76,6254=78,6284=79,6314=81,6344=82,6374=83,6404=84,6434=85,6464=86,6494=86,6524=87,6554=87,6584=87,6614=86,6644=86,6674=85,6704=83,6734=82,6764=79,6794=76,6824=73,6854=71,6884=68,6914=66,6944=63,6974=61,7004=59,7034=57,7064=55,7094=54,7124=52,7154=51,7184=50,7214=48,7244=47,7274=46,7304=45,7334=45,7364=44,7394=44,7424=44,7454=44"),
                                         curve->getData());
                }
                // bri
                {
                    const BPList *curve = track->curve("BRI");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("bri"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(79, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("3101=48,3131=50,3161=52,3191=54,3221=56,3251=57,3281=59,3311=61,3341=63,3371=64,3401=66,3431=68,3461=69,3491=70,3521=72,3551=73,3581=74,3611=75,3641=77,3671=78,3701=78,3731=79,3761=80,3791=81,3821=81,3851=82,3881=82,3911=82,3941=83,3971=83,4001=83,4031=82,4061=82,4091=82,4121=81,4151=80,4181=80,4211=79,4241=77,4271=76,4301=75,4331=73,4361=71,4391=70,4421=68,4451=67,4481=66,4511=65,4541=64,4571=63,4601=62,4631=61,4661=61,4691=60,4721=59,4751=58,4781=58,4811=57,4841=57,4871=56,4901=56,4931=55,4961=55,4991=54,5021=54,5051=54,5081=53,5111=53,5141=53,5171=53,5201=52,5231=52,5261=52,5291=52,5321=52,5351=52,5381=52,5411=52,5441=52"),
                                         curve->getData());
                }
                // cle
                {
                    const BPList *curve = track->curve("CLE");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("cle"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(64, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2340=50,2370=51,2400=52,2430=54,2460=56,2490=58,2520=60,2550=63,2580=65,2610=68,2640=71,2670=74,2700=76,2730=79,2760=82,2790=85,2820=88,2850=90,2880=92,2910=95,2940=97,2970=98,3000=100,3030=101,3060=101,3090=101,3120=101,3150=100,3180=99,3210=97,3240=95,3270=93,3300=91,3330=89,3360=88,3390=86,3420=85,3450=84,3480=82,3510=81,3540=80,3570=79,3600=78,3630=77,3660=76,3690=75,3720=74,3750=73,3780=72,3810=72,3840=71,3870=70,3900=70,3930=69,3960=68,3990=68,4020=68,4050=67,4080=67,4110=67,4140=67,4170=67,4200=67,4230=68"),
                                         curve->getData());
                }
                // gen
                {
                    const BPList *curve = track->curve("GEN");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("gen"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(55, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2924=43,2954=45,2984=48,3014=51,3044=54,3074=57,3104=61,3134=65,3164=69,3194=73,3224=76,3254=80,3284=83,3314=86,3344=88,3374=89,3404=90,3434=91,3464=90,3494=90,3524=89,3554=88,3584=86,3614=84,3644=82,3674=80,3704=78,3734=76,3764=74,3794=72,3824=70,3854=68,3884=66,3914=64,3944=62,3974=61,4004=59,4034=57,4064=56,4094=54,4124=52,4154=51,4184=49,4214=48,4244=47,4274=45,4304=44,4334=43,4364=42,4394=41,4424=40,4454=40,4484=40,4514=40,4544=41"),
                                         curve->getData());
                }
                // por
                {
                    const BPList *curve = track->curve("POR");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("por"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(91, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2104=95,2134=94,2164=93,2194=92,2224=91,2254=90,2284=89,2314=88,2344=87,2374=86,2404=84,2434=83,2464=82,2494=81,2524=79,2554=78,2584=77,2614=76,2644=74,2674=73,2704=72,2734=70,2764=69,2794=68,2824=67,2854=65,2884=64,2914=63,2944=61,2974=60,3004=59,3034=57,3064=56,3094=55,3124=54,3154=53,3184=52,3214=50,3244=49,3274=49,3304=48,3334=48,3364=48,3394=49,3424=50,3454=51,3484=53,3514=54,3544=55,3574=57,3604=58,3634=60,3664=61,3694=63,3724=64,3754=65,3784=67,3814=68,3844=69,3874=70,3904=72,3934=73,3964=74,3994=75,4024=76,4054=77,4084=77,4114=78,4144=79,4174=80,4204=80,4234=81,4264=81,4294=81,4324=82,4354=82,4384=82,4414=82,4444=82,4474=82,4504=82,4534=82,4564=81,4594=81,4624=80,4654=79,4684=78,4714=77,4744=76,5604=25,7037=50"),
                                         curve->getData());
                }
                // ope
                {
                    const BPList *curve = track->curve("OPE");
                    CPPUNIT_ASSERT_EQUAL(127, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("ope"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(98, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("4740=64,4770=63,4800=63,4830=63,4860=63,4890=62,4920=62,4950=61,4980=61,5010=60,5040=60,5070=59,5100=59,5130=58,5160=57,5190=57,5220=56,5250=55,5280=55,5310=54,5340=53,5370=53,5400=52,5430=51,5460=50,5490=50,5520=49,5550=48,5580=47,5610=47,5640=46,5670=45,5700=45,5730=44,5760=43,5790=42,5820=42,5850=41,5880=41,5910=40,5940=39,5970=39,6000=38,6030=38,6060=38,6090=37,6120=37,6150=37,6180=37,6210=37,6240=37,6270=37,6300=37,6330=38,6360=39,6390=40,6420=41,6450=44,6480=47,6510=51,6540=54,6570=56,6600=59,6630=61,6660=63,6690=64,6720=66,6750=68,6780=69,6810=70,6840=72,6870=73,6900=74,6930=75,6960=76,6990=77,7020=78,7050=78,7080=79,7110=80,7140=80,7170=81,7200=81,7230=82,7260=82,7290=83,7320=83,7350=83,7380=83,7410=83,7440=83,7470=83,7500=83,7530=82,7560=82,7590=81,7620=80,7650=79"),
                                         curve->getData());
                }
                // reso1Freq
                {
                    const BPList *curve = track->curve("reso1freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso2Freq
                {
                    const BPList *curve = track->curve("reso2freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso3Freq
                {
                    const BPList *curve = track->curve("reso3freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso4Freq
                {
                    const BPList *curve = track->curve("reso4freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso1bw
                {
                    const BPList *curve = track->curve("reso1bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso2bw
                {
                    const BPList *curve = track->curve("reso2bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso3bw
                {
                    const BPList *curve = track->curve("reso3bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso4bw
                {
                    const BPList *curve = track->curve("reso4bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso1amp
                {
                    const BPList *curve = track->curve("reso1amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso2amp
                {
                    const BPList *curve = track->curve("reso2amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso3amp
                {
                    const BPList *curve = track->curve("reso3amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso4amp
                {
                    const BPList *curve = track->curve("reso4amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // harmonics
                {
                    const BPList *curve = track->curve("harmonics");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("harmonics"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // fx2depth
                {
                    const BPList *curve = track->curve("fx2depth");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("fx2depth"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
            }
        }

        // 2nd track
        {
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

            // 1st event of 2nd track
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
                {
                    const Handle *handle = &item->noteHeadHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->iconDynamicsHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
            }

            // 2nd event of 2nd track
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
                {
                    const Handle *handle = &item->noteHeadHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->iconDynamicsHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
            }

            // control curves
            {
                // pit
                {
                    const BPList *curve = track->curve("PIT");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("pit"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(8191, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(-8192, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // pbs
                {
                    const BPList *curve = track->curve("PBS");
                    CPPUNIT_ASSERT_EQUAL(2, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("pbs"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(24, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // dyn
                {
                    const BPList *curve = track->curve("DYN");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("dyn"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // bre
                {
                    const BPList *curve = track->curve("BRE");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("bre"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // bri
                {
                    const BPList *curve = track->curve("BRI");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("bri"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // cle
                {
                    const BPList *curve = track->curve("CLE");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("cle"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // gen
                {
                    const BPList *curve = track->curve("GEN");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("gen"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // por
                {
                    const BPList *curve = track->curve("POR");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("por"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // ope
                {
                    const BPList *curve = track->curve("OPE");
                    CPPUNIT_ASSERT_EQUAL(127, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("ope"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso1Freq
                {
                    const BPList *curve = track->curve("reso1freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso2Freq
                {
                    const BPList *curve = track->curve("reso2freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso3Freq
                {
                    const BPList *curve = track->curve("reso3freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso4Freq
                {
                    const BPList *curve = track->curve("reso4freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso1bw
                {
                    const BPList *curve = track->curve("reso1bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso2bw
                {
                    const BPList *curve = track->curve("reso2bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso3bw
                {
                    const BPList *curve = track->curve("reso3bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso4bw
                {
                    const BPList *curve = track->curve("reso4bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso1amp
                {
                    const BPList *curve = track->curve("reso1amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso2amp
                {
                    const BPList *curve = track->curve("reso2amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso3amp
                {
                    const BPList *curve = track->curve("reso3amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso4amp
                {
                    const BPList *curve = track->curve("reso4amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // harmonics
                {
                    const BPList *curve = track->curve("harmonics");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("harmonics"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // fx2depth
                {
                    const BPList *curve = track->curve("fx2depth");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("fx2depth"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
            }
        }

        // 3rd track
        {
            const Track *track = sequence.track(2);
            {
                const Common *common = track->common();
                CPPUNIT_ASSERT_EQUAL(string("DSB202"), common->version);
                CPPUNIT_ASSERT_EQUAL(string("Voice1"), common->name);
                CPPUNIT_ASSERT_EQUAL(string("181,162,123"), common->color);
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::DynamicsMode::STANDARD, common->dynamicsMode);
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::PlayMode::PLAY_WITH_SYNTH, common->playMode());
                CPPUNIT_ASSERT_EQUAL(VSQ_NS::PlayMode::PLAY_WITH_SYNTH, common->lastPlayMode());
            }
            CPPUNIT_ASSERT_EQUAL(3, track->events()->size());
            // 1st event of 3rd track
            {
                const Event *item = track->events()->get(0);
                CPPUNIT_ASSERT_EQUAL(1, item->id);
                CPPUNIT_ASSERT_EQUAL((tick_t)0, item->clock);
                CPPUNIT_ASSERT_EQUAL(EventType::SINGER, item->type);
                CPPUNIT_ASSERT_EQUAL(0, item->note);
                CPPUNIT_ASSERT_EQUAL(64, item->dynamics);
                CPPUNIT_ASSERT_EQUAL(8, item->pmBendDepth);
                CPPUNIT_ASSERT_EQUAL(0, item->pmBendLength);
                CPPUNIT_ASSERT_EQUAL(0, item->pmbPortamentoUse);
                CPPUNIT_ASSERT_EQUAL(50, item->demDecGainRate);
                CPPUNIT_ASSERT_EQUAL(50, item->demAccent);
                CPPUNIT_ASSERT_EQUAL(0, item->vibratoDelay);
                CPPUNIT_ASSERT_EQUAL(10, item->pMeanOnsetFirstNote);
                CPPUNIT_ASSERT_EQUAL(12, item->vMeanNoteTransition);
                CPPUNIT_ASSERT_EQUAL(24, item->d4mean);
                CPPUNIT_ASSERT_EQUAL(12, item->pMeanEndingNote);
                CPPUNIT_ASSERT_EQUAL((tick_t)0, item->getLength());
                {
                    const Handle *handle = &item->singerHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::SINGER, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(string(""), handle->caption);
                    CPPUNIT_ASSERT_EQUAL(string("$07010000"), handle->iconId);
                    CPPUNIT_ASSERT_EQUAL(string("MEIKO"), handle->ids);
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
                {
                    const Handle *handle = &item->noteHeadHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->iconDynamicsHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
            }

            // 2nd event of 3rd track
            {
                const Event *item = track->events()->get(1);
                CPPUNIT_ASSERT_EQUAL(2, item->id);
                CPPUNIT_ASSERT_EQUAL((tick_t)1920, item->clock);
                CPPUNIT_ASSERT_EQUAL(EventType::NOTE, item->type);
                CPPUNIT_ASSERT_EQUAL(63, item->note);
                CPPUNIT_ASSERT_EQUAL(72, item->dynamics);
                CPPUNIT_ASSERT_EQUAL(8, item->pmBendDepth);
                CPPUNIT_ASSERT_EQUAL(0, item->pmBendLength);
                CPPUNIT_ASSERT_EQUAL(0, item->pmbPortamentoUse);
                CPPUNIT_ASSERT_EQUAL(50, item->demDecGainRate);
                CPPUNIT_ASSERT_EQUAL(50, item->demAccent);
                CPPUNIT_ASSERT_EQUAL(960, item->vibratoDelay);
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
                    {
                        const Lyric lyric = item->lyricHandle.getLyricAt(0);
                        CPPUNIT_ASSERT_EQUAL(string("a"), lyric.phrase);
                        CPPUNIT_ASSERT_EQUAL(1.0, lyric.lengthRatio);
                        CPPUNIT_ASSERT_EQUAL(false, lyric.isProtected);
                        CPPUNIT_ASSERT_EQUAL(string("0"), lyric.getConsonantAdjustment());
                        CPPUNIT_ASSERT_EQUAL(string("a"), lyric.getPhoneticSymbol());
                    }
                }
                {
                    const Handle *handle = &item->vibratoHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->noteHeadHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::NOTE_HEAD, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(string("$01010008"), handle->iconId);
                    CPPUNIT_ASSERT_EQUAL(string("trill_whole"), handle->ids);
                    CPPUNIT_ASSERT_EQUAL(8, handle->original);
                    CPPUNIT_ASSERT_EQUAL(64, handle->depth);
                    CPPUNIT_ASSERT_EQUAL(64, handle->duration);
                    CPPUNIT_ASSERT_EQUAL(string("Trill Wholetone"), handle->caption);
                    CPPUNIT_ASSERT_EQUAL((tick_t)120, handle->getLength());
                }
                {
                    const Handle *handle = &item->iconDynamicsHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
            }

            // 3rd event of 3rd track
            {
                const Event *item = track->events()->get(2);
                CPPUNIT_ASSERT_EQUAL(3, item->id);
                CPPUNIT_ASSERT_EQUAL((tick_t)3360, item->clock);
                CPPUNIT_ASSERT_EQUAL(EventType::ICON, item->type);
                CPPUNIT_ASSERT_EQUAL(68, item->note);
                CPPUNIT_ASSERT_EQUAL(64, item->dynamics);
                CPPUNIT_ASSERT_EQUAL(8, item->pmBendDepth);
                CPPUNIT_ASSERT_EQUAL(0, item->pmBendLength);
                CPPUNIT_ASSERT_EQUAL(0, item->pmbPortamentoUse);
                CPPUNIT_ASSERT_EQUAL(50, item->demDecGainRate);
                CPPUNIT_ASSERT_EQUAL(50, item->demAccent);
                CPPUNIT_ASSERT_EQUAL(0, item->vibratoDelay);
                CPPUNIT_ASSERT_EQUAL(10, item->pMeanOnsetFirstNote);
                CPPUNIT_ASSERT_EQUAL(12, item->vMeanNoteTransition);
                CPPUNIT_ASSERT_EQUAL(24, item->d4mean);
                CPPUNIT_ASSERT_EQUAL(12, item->pMeanEndingNote);
                CPPUNIT_ASSERT_EQUAL((tick_t)480, item->getLength());
                {
                    const Handle *handle = &item->singerHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->lyricHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->vibratoHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->noteHeadHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, handle->getHandleType());
                }
                {
                    const Handle *handle = &item->iconDynamicsHandle;
                    CPPUNIT_ASSERT_EQUAL(HandleType::DYNAMICS, handle->getHandleType());
                    CPPUNIT_ASSERT_EQUAL(string("$05020005"), handle->iconId);
                    CPPUNIT_ASSERT_EQUAL(string("cresc_5"), handle->ids);
                    CPPUNIT_ASSERT_EQUAL(5, handle->original);
                    CPPUNIT_ASSERT_EQUAL(string("Zero Crescendo Curve"), handle->caption);
                    CPPUNIT_ASSERT_EQUAL((tick_t)480, handle->getLength());
                    CPPUNIT_ASSERT_EQUAL(0, handle->startDyn);
                    CPPUNIT_ASSERT_EQUAL(102, handle->endDyn);
                    CPPUNIT_ASSERT_EQUAL(1, handle->dynBP.size());
                    CPPUNIT_ASSERT_EQUAL(0.5, handle->dynBP.get(0).x);
                    CPPUNIT_ASSERT_EQUAL(40, handle->dynBP.get(0).y);
                }
            }

            // control curves
            {
                // pit
                {
                    const BPList *curve = track->curve("PIT");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("pit"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(8191, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(-8192, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // pbs
                {
                    const BPList *curve = track->curve("PBS");
                    CPPUNIT_ASSERT_EQUAL(2, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("pbs"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(24, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // dyn
                {
                    const BPList *curve = track->curve("DYN");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("dyn"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // bre
                {
                    const BPList *curve = track->curve("BRE");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("bre"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // bri
                {
                    const BPList *curve = track->curve("BRI");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("bri"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // cle
                {
                    const BPList *curve = track->curve("CLE");
                    CPPUNIT_ASSERT_EQUAL(0, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("cle"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // gen
                {
                    const BPList *curve = track->curve("GEN");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("gen"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // por
                {
                    const BPList *curve = track->curve("POR");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("por"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // ope
                {
                    const BPList *curve = track->curve("OPE");
                    CPPUNIT_ASSERT_EQUAL(127, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("ope"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string(""), curve->getData());
                }
                // reso1Freq
                {
                    const BPList *curve = track->curve("reso1freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2000=1"), curve->getData());
                }
                // reso2Freq
                {
                    const BPList *curve = track->curve("reso2freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2001=2"), curve->getData());
                }
                // reso3Freq
                {
                    const BPList *curve = track->curve("reso3freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2002=3"), curve->getData());
                }
                // reso4Freq
                {
                    const BPList *curve = track->curve("reso4freq");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4freq"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2003=4"), curve->getData());
                }
                // reso1bw
                {
                    const BPList *curve = track->curve("reso1bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2004=5"), curve->getData());
                }
                // reso2bw
                {
                    const BPList *curve = track->curve("reso2bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2006=7"), curve->getData());
                }
                // reso3bw
                {
                    const BPList *curve = track->curve("reso3bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2007=8"), curve->getData());
                }
                // reso4bw
                {
                    const BPList *curve = track->curve("reso4bw");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4bw"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2008=9"), curve->getData());
                }
                // reso1amp
                {
                    const BPList *curve = track->curve("reso1amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso1amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2009=10"), curve->getData());
                }
                // reso2amp
                {
                    const BPList *curve = track->curve("reso2amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso2amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2010=11"), curve->getData());
                }
                // reso3amp
                {
                    const BPList *curve = track->curve("reso3amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso3amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2011=12"), curve->getData());
                }
                // reso4amp
                {
                    const BPList *curve = track->curve("reso4amp");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("reso4amp"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2012=13"), curve->getData());
                }
                // harmonics
                {
                    const BPList *curve = track->curve("harmonics");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("harmonics"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2013=14"), curve->getData());
                }
                // fx2depth
                {
                    const BPList *curve = track->curve("fx2depth");
                    CPPUNIT_ASSERT_EQUAL(64, curve->getDefault());
                    CPPUNIT_ASSERT_EQUAL(string("fx2depth"), curve->getName());
                    CPPUNIT_ASSERT_EQUAL(127, curve->getMaximum());
                    CPPUNIT_ASSERT_EQUAL(0, curve->getMinimum());
                    CPPUNIT_ASSERT_EQUAL(1, curve->size());
                    CPPUNIT_ASSERT_EQUAL(string("2014=15"), curve->getData());
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
