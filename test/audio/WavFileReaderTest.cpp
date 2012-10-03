#include "../Util.hpp"
#include "../../audio/WavFileReader.hpp"

using namespace std;
using namespace cadencii::audio;

class WavFileReaderTest : public CppUnit::TestCase{
public:
    void stereo16_44100(){
        WavFileReader reader( TestUtil::getFixtureRootPath() + "/fixture/audio/WavFileReaderTest/44100stereo16.wav" );

        CPPUNIT_ASSERT_EQUAL( 44100, reader.getSampleRate() );
        CPPUNIT_ASSERT_EQUAL( (uint32_t)44100, reader.getTotalSamples() );

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

    void monoral16_44100(){
        WavFileReader reader( TestUtil::getFixtureRootPath() + "/fixture/audio/WavFileReaderTest/44100monoral16.wav" );

        CPPUNIT_ASSERT_EQUAL( 44100, reader.getSampleRate() );
        CPPUNIT_ASSERT_EQUAL( (uint32_t)44100, reader.getTotalSamples() );

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
        int count = 44100 / 25;
        for( int i = 0; i < count; i++ ){
            reader.pull( actualLeft, actualRight, 25 );
            for( int j = 0; j < 25; j++ ){
                ostringstream oss;
                oss << "; i=" << i << "; j=" << j << "; index=" << (i * 25 + j);
                CPPUNIT_ASSERT_EQUAL_MESSAGE( "left" + oss.str(), fixture[j], actualLeft[j] );
                CPPUNIT_ASSERT_EQUAL_MESSAGE( "right" + oss.str(), fixture[j], actualRight[j] );
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

    void stereo8_44100(){
        WavFileReader reader( TestUtil::getFixtureRootPath() + "/fixture/audio/WavFileReaderTest/44100stereo8.wav" );

        CPPUNIT_ASSERT_EQUAL( 44100, reader.getSampleRate() );
        CPPUNIT_ASSERT_EQUAL( (uint32_t)44100, reader.getTotalSamples() );

        const double amplitude = 128.0;
        double fixture[25];
        fixture[0] = (0x7F - 0x7F) / amplitude;
        fixture[1] = (0x85 - 0x7F) / amplitude;
        fixture[2] = (0x8B - 0x7F) / amplitude;
        fixture[3] = (0x90 - 0x7F) / amplitude;
        fixture[4] = (0x95 - 0x7F) / amplitude;
        fixture[5] = (0x97 - 0x7F) / amplitude;
        fixture[6] = (0x98 - 0x7F) / amplitude;
        fixture[7] = (0x98 - 0x7F) / amplitude;
        fixture[8] = (0x96 - 0x7F) / amplitude;
        fixture[9] = (0x93 - 0x7F) / amplitude;
        fixture[10] = (0x8E - 0x7F) / amplitude;
        fixture[11] = (0x88 - 0x7F) / amplitude;
        fixture[12] = (0x82 - 0x7F) / amplitude;
        fixture[13] = (0x7C - 0x7F) / amplitude;
        fixture[14] = (0x76 - 0x7F) / amplitude;
        fixture[15] = (0x70 - 0x7F) / amplitude;
        fixture[16] = (0x6B - 0x7F) / amplitude;
        fixture[17] = (0x68 - 0x7F) / amplitude;
        fixture[18] = (0x66 - 0x7F) / amplitude;
        fixture[19] = (0x66 - 0x7F) / amplitude;
        fixture[20] = (0x67 - 0x7F) / amplitude;
        fixture[21] = (0x69 - 0x7F) / amplitude;
        fixture[22] = (0x6E - 0x7F) / amplitude;
        fixture[23] = (0x73 - 0x7F) / amplitude;
        fixture[24] = (0x79 - 0x7F) / amplitude;

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

    void monoral8_44100(){
        WavFileReader reader( TestUtil::getFixtureRootPath() + "/fixture/audio/WavFileReaderTest/44100monoral8.wav" );

        CPPUNIT_ASSERT_EQUAL( 44100, reader.getSampleRate() );
        CPPUNIT_ASSERT_EQUAL( (uint32_t)44100, reader.getTotalSamples() );

        const double amplitude = 128.0;
        double fixture[25];
        fixture[0] = (0x7F - 0x7F) / amplitude;
        fixture[1] = (0x85 - 0x7F) / amplitude;
        fixture[2] = (0x8B - 0x7F) / amplitude;
        fixture[3] = (0x90 - 0x7F) / amplitude;
        fixture[4] = (0x95 - 0x7F) / amplitude;
        fixture[5] = (0x97 - 0x7F) / amplitude;
        fixture[6] = (0x98 - 0x7F) / amplitude;
        fixture[7] = (0x98 - 0x7F) / amplitude;
        fixture[8] = (0x96 - 0x7F) / amplitude;
        fixture[9] = (0x93 - 0x7F) / amplitude;
        fixture[10] = (0x8E - 0x7F) / amplitude;
        fixture[11] = (0x88 - 0x7F) / amplitude;
        fixture[12] = (0x82 - 0x7F) / amplitude;
        fixture[13] = (0x7C - 0x7F) / amplitude;
        fixture[14] = (0x76 - 0x7F) / amplitude;
        fixture[15] = (0x70 - 0x7F) / amplitude;
        fixture[16] = (0x6B - 0x7F) / amplitude;
        fixture[17] = (0x68 - 0x7F) / amplitude;
        fixture[18] = (0x66 - 0x7F) / amplitude;
        fixture[19] = (0x66 - 0x7F) / amplitude;
        fixture[20] = (0x67 - 0x7F) / amplitude;
        fixture[21] = (0x69 - 0x7F) / amplitude;
        fixture[22] = (0x6E - 0x7F) / amplitude;
        fixture[23] = (0x73 - 0x7F) / amplitude;
        fixture[24] = (0x79 - 0x7F) / amplitude;

        double actualLeft[25];
        double actualRight[25];

        // fixture には、44100 サンプル分のデータが入っている。
        int count = 44100 / 25;
        for( int i = 0; i < count; i++ ){
            reader.pull( actualLeft, actualRight, 25 );
            for( int j = 0; j < 25; j++ ){
                ostringstream oss;
                oss << "; i=" << i << "; j=" << j << "; index=" << (i * 25 + j);
                CPPUNIT_ASSERT_EQUAL_MESSAGE( "left" + oss.str(), fixture[j], actualLeft[j] );
                CPPUNIT_ASSERT_EQUAL_MESSAGE( "right" + oss.str(), fixture[j], actualRight[j] );
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
    CPPUNIT_TEST( monoral16_44100 );
    CPPUNIT_TEST( stereo8_44100 );
    CPPUNIT_TEST( monoral8_44100 );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( WavFileReaderTest );
