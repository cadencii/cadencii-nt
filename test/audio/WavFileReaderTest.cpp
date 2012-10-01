#include "../Util.hpp"
#include "../../audio/WavFileReader.hpp"

using namespace std;
using namespace cadencii::audio;

class WavFileReaderTest : public CppUnit::TestCase{
public:
    void stereo16_44100(){
        WavFileReader reader( TestUtil::getFixtureRootPath() + "/fixture/audio/WavFileReaderTest/44100stereo16.wav" );

        const double amplitude = 32768.0;
        double fixture[25];
        fixture[0] = 0 / amplitude;
        fixture[1] = 1629 / amplitude;
        fixture[2] = 3157 / amplitude;
        fixture[3] = 4486 / amplitude;
        fixture[4] = 5533 / amplitude;
        fixture[5] = 6232 / amplitude;
        fixture[6] = 6540 / amplitude;
        fixture[7] = 6437 / amplitude;
        fixture[8] = 5929 / amplitude;
        fixture[9] = 5049 / amplitude;
        fixture[10] = 3852 / amplitude;
        fixture[11] = 2412 / amplitude;
        fixture[12] = 821 / amplitude;
        fixture[13] = -821 / amplitude;
        fixture[14] = -2412 / amplitude;
        fixture[15] = -3852 / amplitude;
        fixture[16] = -5049 / amplitude;
        fixture[17] = -5929 / amplitude;
        fixture[18] = -6437 / amplitude;
        fixture[19] = -6540 / amplitude;
        fixture[20] = -6232 / amplitude;
        fixture[21] = -5533 / amplitude;
        fixture[22] = -4486 / amplitude;
        fixture[23] = -3157 / amplitude;
        fixture[24] = -1629 / amplitude;

        double actualLeft[25];
        double actualRight[25];

        // fixture には、44100 サンプル分のデータが入っている。
        // また、左右で波形が反転している
        int count = 44100 / 25;
        for( int i = 0; i < count; i++ ){
            reader.pull( actualLeft, actualRight, 25 );
            for( int j = 0; j < 25; j++ ){
                ostringstream oss;
                oss << "; i=" << i << "; j=" << j << "; index=" << (i * 25 + j);
                CPPUNIT_ASSERT_EQUAL_MESSAGE( "left" + oss.str(), fixture[j], actualLeft[j] );
                CPPUNIT_ASSERT_EQUAL_MESSAGE( "right" + oss.str(), -fixture[j], actualRight[j] );
            }
        }

        // 44101 サンプル以降を取得しようとすると、無音となる
        reader.pull( actualLeft, actualRight, 25 );
        for( int i = 0; i < 25; i++ ){
            ostringstream oss;
            oss << "; i=" << i << "; index=" << (count * 25 + i);
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "left" + oss.str(), 0.0, actualLeft[i] );
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "right" + oss.str(), 0.0, actualRight[i] );
        }
    }

    CPPUNIT_TEST_SUITE( WavFileReaderTest );
    CPPUNIT_TEST( stereo16_44100 );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( WavFileReaderTest );
