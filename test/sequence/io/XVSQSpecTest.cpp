#include "../../Util.hpp"
#include "../../../sequence/io/XVSQSpec.hpp"

using namespace std;
using namespace cadencii;

class XVSQSpecTest : public CppUnit::TestCase {
public:
    void getElementName() {
        XVSQSpec spec;
        CPPUNIT_ASSERT_EQUAL(string("PIT"), spec.getElementName("PIT"));
        CPPUNIT_ASSERT_EQUAL(string("PIT"), spec.getElementName("piT"));
        CPPUNIT_ASSERT_EQUAL(string("reso4AmpBPList"), spec.getElementName("reso4amp"));
        CPPUNIT_ASSERT_EQUAL(string(""), spec.getElementName("hogehoge---"));
    }

    void getCurveName() {
        XVSQSpec spec;
        CPPUNIT_ASSERT_EQUAL(string("pit"), spec.getCurveName("PIT"));
        CPPUNIT_ASSERT_EQUAL(string("reso4amp"), spec.getCurveName("reso4AmpBPList"));
        CPPUNIT_ASSERT_EQUAL(string(""), spec.getCurveName("hogehoge---"));
    }

    CPPUNIT_TEST_SUITE(XVSQSpecTest);
    CPPUNIT_TEST(getElementName);
    CPPUNIT_TEST(getCurveName);
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(XVSQSpecTest);
