#include "../Util.hpp"
#include "../../enum/QuantizeMode.hpp"

using namespace std;
using namespace cadencii;

class QuantizeModeTest : public CppUnit::TestCase{
public:
    void toString(){
        CPPUNIT_ASSERT_EQUAL( string( "NONE" ), QuantizeMode::toString( QuantizeMode::NONE ) );
        CPPUNIT_ASSERT_EQUAL( string( "QUARTER" ), QuantizeMode::toString( QuantizeMode::QUARTER ) );
        CPPUNIT_ASSERT_EQUAL( string( "EIGHTH" ), QuantizeMode::toString( QuantizeMode::EIGHTH ) );
        CPPUNIT_ASSERT_EQUAL( string( "SIXTEENTH" ), QuantizeMode::toString( QuantizeMode::SIXTEENTH ) );
        CPPUNIT_ASSERT_EQUAL( string( "THIRTY_SECOND" ), QuantizeMode::toString( QuantizeMode::THIRTY_SECOND ) );
        CPPUNIT_ASSERT_EQUAL( string( "SIXTY_FOURTH" ), QuantizeMode::toString( QuantizeMode::SIXTY_FOURTH ) );
        CPPUNIT_ASSERT_EQUAL( string( "HUNDRED_TWENTY_EIGHTH" ), QuantizeMode::toString( QuantizeMode::HUNDRED_TWENTY_EIGHTH ) );
        CPPUNIT_ASSERT_EQUAL( string( "" ), QuantizeMode::toString( (QuantizeMode::QuantizeModeEnum)-1 ) );
    }

    void fromString(){
        // 未知の文字列がきた場合は NONE になる
        CPPUNIT_ASSERT_EQUAL( QuantizeMode::NONE, QuantizeMode::fromString( "" ) );
        CPPUNIT_ASSERT_EQUAL( QuantizeMode::NONE, QuantizeMode::fromString( "NONE" ) );

        // 大文字、小文字の区別はされない
        CPPUNIT_ASSERT_EQUAL( QuantizeMode::QUARTER, QuantizeMode::fromString( "quArtEr" ) );
        CPPUNIT_ASSERT_EQUAL( QuantizeMode::EIGHTH, QuantizeMode::fromString( "EIGHTH" ) );
        CPPUNIT_ASSERT_EQUAL( QuantizeMode::SIXTEENTH, QuantizeMode::fromString( "SIXTEENTH" ) );
        CPPUNIT_ASSERT_EQUAL( QuantizeMode::THIRTY_SECOND, QuantizeMode::fromString( "THIRTY_SECOND" ) );
        CPPUNIT_ASSERT_EQUAL( QuantizeMode::SIXTY_FOURTH, QuantizeMode::fromString( "SIXTY_FOURTH" ) );
        CPPUNIT_ASSERT_EQUAL( QuantizeMode::HUNDRED_TWENTY_EIGHTH, QuantizeMode::fromString( "HUNDRED_TWENTY_EIGHTH" ) );
    }

    CPPUNIT_TEST_SUITE( QuantizeModeTest );
    CPPUNIT_TEST( toString );
    CPPUNIT_TEST( fromString );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( QuantizeModeTest );
