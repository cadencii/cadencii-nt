#include "Util.hpp"
#include "../ItemSelectionManager.hpp"
#include "../vsq/Event.hpp"

using namespace VSQ_NS;
using namespace cadencii;

class ItemSelectionManagerTest : public CppUnit::TestCase{
public:
    void test(){
        ItemSelectionManager manager;
        Event *item = new Event( 0, EventType::NOTE );
        CPPUNIT_ASSERT( false == manager.isContains( item ) );
        manager.add( item );
        CPPUNIT_ASSERT( true == manager.isContains( item ) );
        manager.clear();
        CPPUNIT_ASSERT( false == manager.isContains( item ) );
        delete item;
    }

    CPPUNIT_TEST_SUITE( ItemSelectionManagerTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( ItemSelectionManagerTest );
