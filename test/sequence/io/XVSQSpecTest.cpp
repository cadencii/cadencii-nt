#include "../../Util.hpp"
#include "../../../sequence/io/XVSQSpec.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace cadencii;

TEST(XVSQSpecTest, getElementName)
{
    XVSQSpec spec;
    EXPECT_EQ(string("PIT"), spec.getElementName("PIT"));
    EXPECT_EQ(string("PIT"), spec.getElementName("piT"));
    EXPECT_EQ(string("reso4AmpBPList"), spec.getElementName("reso4amp"));
    EXPECT_EQ(string(""), spec.getElementName("hogehoge---"));
}

TEST(XVSQSpecTest, getCurveName)
{
    XVSQSpec spec;
    EXPECT_EQ(string("pit"), spec.getCurveName("PIT"));
    EXPECT_EQ(string("reso4amp"), spec.getCurveName("reso4AmpBPList"));
    EXPECT_EQ(string(""), spec.getCurveName("hogehoge---"));
}
