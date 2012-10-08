#include "Util.hpp"
#include "../ItemSelectionManager.hpp"
#include "../vsq/Event.hpp"

using namespace VSQ_NS;
using namespace cadencii;

class ItemSelectionStatusListenerStub : public ItemSelectionStatusListener{
private:
    int statusChangedCount;

public:
    ItemSelectionStatusListenerStub(){
        statusChangedCount = 0;
    }

    void statusChanged(){
        statusChangedCount++;
    }

    int getStatusChangedCount(){
        return statusChangedCount;
    }
};

class ItemSelectionManagerTest : public CppUnit::TestCase{
public:
    void test(){
        ItemSelectionStatusListenerStub listener;
        ItemSelectionManager manager;
        manager.addStatusListener( &listener );
        Event *item = new Event( 0, EventType::NOTE );
        CPPUNIT_ASSERT( false == manager.isContains( item ) );
        CPPUNIT_ASSERT_EQUAL( 0, listener.getStatusChangedCount() );
        manager.add( item );
        CPPUNIT_ASSERT( true == manager.isContains( item ) );
        CPPUNIT_ASSERT_EQUAL( 1, listener.getStatusChangedCount() );
        manager.clear();
        CPPUNIT_ASSERT( false == manager.isContains( item ) );
        CPPUNIT_ASSERT_EQUAL( 2, listener.getStatusChangedCount() );
        delete item;
    }

    CPPUNIT_TEST_SUITE( ItemSelectionManagerTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( ItemSelectionManagerTest );
