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

    void testAddRemove(){
        ItemSelectionStatusListenerStub listener;
        ItemSelectionManager manager;
        manager.addStatusListener( &listener );
        Event *item = new Event( 0, EventType::NOTE );
        manager.add( item );
        CPPUNIT_ASSERT( true == manager.isContains( item ) );
        CPPUNIT_ASSERT_EQUAL( (size_t)1, manager.getEventItemList()->size() );
        CPPUNIT_ASSERT_EQUAL( 1, listener.getStatusChangedCount() );
        manager.add( item );
        CPPUNIT_ASSERT_EQUAL( (size_t)1, manager.getEventItemList()->size() );
        CPPUNIT_ASSERT_EQUAL( 1, listener.getStatusChangedCount() );
        manager.remove( item );
        CPPUNIT_ASSERT_EQUAL( 2, listener.getStatusChangedCount() );
        CPPUNIT_ASSERT( false == manager.isContains( item ) );
        CPPUNIT_ASSERT_EQUAL( (size_t)0, manager.getEventItemList()->size() );
        CPPUNIT_ASSERT_NO_THROW( manager.remove( item ) );
        CPPUNIT_ASSERT_EQUAL( 2, listener.getStatusChangedCount() );
        delete item;
    }

    void testMoveItems(){
        ItemSelectionManager manager;
        Event *itemA = new Event( 0, EventType::NOTE );
        itemA->clock = 480;
        itemA->note = 50;
        Event *itemB = new Event( 0, EventType::NOTE );
        itemB->clock = 1920;
        itemB->note = 52;
        manager.add( itemA );
        manager.add( itemB );

        manager.moveItems( 10, 5 );

        const map<const VSQ_NS::Event *, VSQ_NS::Event> *itemList = manager.getEventItemList();
        CPPUNIT_ASSERT_EQUAL( (tick_t)(480 + 10), itemList->at( itemA ).clock );
        CPPUNIT_ASSERT_EQUAL( 50 + 5, itemList->at( itemA ).note );
        CPPUNIT_ASSERT_EQUAL( (tick_t)(1920 + 10), itemList->at( itemB ).clock );
        CPPUNIT_ASSERT_EQUAL( 52 + 5, itemList->at( itemB ).note );
    }

    CPPUNIT_TEST_SUITE( ItemSelectionManagerTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST( testAddRemove );
    CPPUNIT_TEST( testMoveItems );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( ItemSelectionManagerTest );
