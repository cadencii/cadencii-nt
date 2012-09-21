#include "../Util.hpp"
#include "../../enum/QuantizeMode.hpp"

using namespace std;
using namespace cadencii;
using namespace VSQ_NS;

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

    void getQuantizeUnitTick(){
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, QuantizeMode::getQuantizeUnitTick( QuantizeMode::QUARTER ) );
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, QuantizeMode::getQuantizeUnitTick( QuantizeMode::EIGHTH ) );
        CPPUNIT_ASSERT_EQUAL( (tick_t)120, QuantizeMode::getQuantizeUnitTick( QuantizeMode::SIXTEENTH ) );
        CPPUNIT_ASSERT_EQUAL( (tick_t)60, QuantizeMode::getQuantizeUnitTick( QuantizeMode::THIRTY_SECOND ) );
        CPPUNIT_ASSERT_EQUAL( (tick_t)30, QuantizeMode::getQuantizeUnitTick( QuantizeMode::SIXTY_FOURTH ) );
        CPPUNIT_ASSERT_EQUAL( (tick_t)15, QuantizeMode::getQuantizeUnitTick( QuantizeMode::HUNDRED_TWENTY_EIGHTH ) );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1, QuantizeMode::getQuantizeUnitTick( QuantizeMode::NONE ) );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1, QuantizeMode::getQuantizeUnitTick( (QuantizeMode::QuantizeModeEnum)-1 ) );
    }

    CPPUNIT_TEST_SUITE( QuantizeModeTest );
    CPPUNIT_TEST( toString );
    CPPUNIT_TEST( fromString );
    CPPUNIT_TEST( getQuantizeUnitTick );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( QuantizeModeTest );
