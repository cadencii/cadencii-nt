#include "../Util.hpp"
#include "../../enum/QuantizeMode.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace cadencii;
using namespace vsq;

TEST(QuantizeModeTest, toString)
{
    EXPECT_EQ( string( "NONE" ), QuantizeMode::toString( QuantizeMode::NONE ) );
    EXPECT_EQ( string( "QUARTER" ), QuantizeMode::toString( QuantizeMode::QUARTER ) );
    EXPECT_EQ( string( "EIGHTH" ), QuantizeMode::toString( QuantizeMode::EIGHTH ) );
    EXPECT_EQ( string( "SIXTEENTH" ), QuantizeMode::toString( QuantizeMode::SIXTEENTH ) );
    EXPECT_EQ( string( "THIRTY_SECOND" ), QuantizeMode::toString( QuantizeMode::THIRTY_SECOND ) );
    EXPECT_EQ( string( "SIXTY_FOURTH" ), QuantizeMode::toString( QuantizeMode::SIXTY_FOURTH ) );
    EXPECT_EQ( string( "HUNDRED_TWENTY_EIGHTH" ), QuantizeMode::toString( QuantizeMode::HUNDRED_TWENTY_EIGHTH ) );
    EXPECT_EQ( string( "" ), QuantizeMode::toString( (QuantizeMode::QuantizeModeEnum)-1 ) );
}

TEST(QuantizeModeTest, fromString)
{
    // 未知の文字列がきた場合は NONE になる
    EXPECT_EQ( QuantizeMode::NONE, QuantizeMode::fromString( "" ) );
    EXPECT_EQ( QuantizeMode::NONE, QuantizeMode::fromString( "NONE" ) );

    // 大文字、小文字の区別はされない
    EXPECT_EQ( QuantizeMode::QUARTER, QuantizeMode::fromString( "quArtEr" ) );
    EXPECT_EQ( QuantizeMode::EIGHTH, QuantizeMode::fromString( "EIGHTH" ) );
    EXPECT_EQ( QuantizeMode::SIXTEENTH, QuantizeMode::fromString( "SIXTEENTH" ) );
    EXPECT_EQ( QuantizeMode::THIRTY_SECOND, QuantizeMode::fromString( "THIRTY_SECOND" ) );
    EXPECT_EQ( QuantizeMode::SIXTY_FOURTH, QuantizeMode::fromString( "SIXTY_FOURTH" ) );
    EXPECT_EQ( QuantizeMode::HUNDRED_TWENTY_EIGHTH, QuantizeMode::fromString( "HUNDRED_TWENTY_EIGHTH" ) );
}

TEST(QuantizeModeTest, getQuantizeUnitTick)
{
    EXPECT_EQ( (tick_t)480, QuantizeMode::getQuantizeUnitTick( QuantizeMode::QUARTER ) );
    EXPECT_EQ( (tick_t)240, QuantizeMode::getQuantizeUnitTick( QuantizeMode::EIGHTH ) );
    EXPECT_EQ( (tick_t)120, QuantizeMode::getQuantizeUnitTick( QuantizeMode::SIXTEENTH ) );
    EXPECT_EQ( (tick_t)60, QuantizeMode::getQuantizeUnitTick( QuantizeMode::THIRTY_SECOND ) );
    EXPECT_EQ( (tick_t)30, QuantizeMode::getQuantizeUnitTick( QuantizeMode::SIXTY_FOURTH ) );
    EXPECT_EQ( (tick_t)15, QuantizeMode::getQuantizeUnitTick( QuantizeMode::HUNDRED_TWENTY_EIGHTH ) );
    EXPECT_EQ( (tick_t)1, QuantizeMode::getQuantizeUnitTick( QuantizeMode::NONE ) );
    EXPECT_EQ( (tick_t)1, QuantizeMode::getQuantizeUnitTick( (QuantizeMode::QuantizeModeEnum)-1 ) );
}
