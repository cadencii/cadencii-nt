#include "Util.hpp"
#include "../ItemSelectionManager.hpp"
#include "../vsq/Event.hpp"
#include <set>

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

class ItemSelectionManagerStub : public ItemSelectionManager{
public:
    const std::map<int, VSQ_NS::Event> *getOriginalItemList(){
        return &originalEventItemList;
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
        itemA->id = 1;
        Event *itemB = new Event( 0, EventType::NOTE );
        itemB->clock = 1920;
        itemB->note = 52;
        itemB->id = 2;
        manager.add( itemA );
        manager.add( itemB );

        manager.moveItems( 10, 5 );

        const map<int, VSQ_NS::Event> *itemList = manager.getEventItemList();
        CPPUNIT_ASSERT_EQUAL( (tick_t)(480 + 10), itemList->at( itemA->id ).clock );
        CPPUNIT_ASSERT_EQUAL( 50 + 5, itemList->at( itemA->id ).note );
        CPPUNIT_ASSERT_EQUAL( (tick_t)(1920 + 10), itemList->at( itemB->id ).clock );
        CPPUNIT_ASSERT_EQUAL( 52 + 5, itemList->at( itemB->id ).note );

        delete itemA;
        delete itemB;
    }

    void testRevertSelectionStatusTo(){
        ItemSelectionManager manager;
        Event *itemA = new Event( 0, EventType::NOTE );
        itemA->clock = 480;
        itemA->note = 50;
        itemA->id = 1;
        Event *itemB = new Event( 0, EventType::NOTE );
        itemB->clock = 1920;
        itemB->note = 52;
        itemB->id = 2;
        manager.add( itemA );
        manager.add( itemB );

        ItemSelectionManager target;
        target.revertSelectionStatusTo( manager );
        CPPUNIT_ASSERT( true == target.isContains( itemA ) );
        CPPUNIT_ASSERT( true == target.isContains( itemB ) );

        CPPUNIT_ASSERT_EQUAL( (size_t)2, target.getEventItemList()->size() );

        delete itemA;
        delete itemB;
    }

    void testAddRemoveUsingList(){
        ItemSelectionManager manager;
        Event itemA( 0, EventType::NOTE );
        itemA.clock = 480;
        itemA.note = 50;
        Event itemB( 0, EventType::NOTE );
        itemB.clock = 1920;
        itemB.note = 52;
        std::set<const VSQ_NS::Event *> list;
        list.insert( &itemA );
        list.insert( &itemB );

        manager.add( list );
        CPPUNIT_ASSERT( true == manager.isContains( &itemA ) );
        CPPUNIT_ASSERT( true == manager.isContains( &itemB ) );

        manager.remove( list );
        CPPUNIT_ASSERT( false == manager.isContains( &itemA ) );
        CPPUNIT_ASSERT( false == manager.isContains( &itemB ) );
    }

    void updateSelectedItemContents(){
        Sequence sequence;
        Event itemA( 0, EventType::NOTE );
        itemA.clock = 480;
        itemA.note = 50;
        int id = sequence.track[0].events()->add( itemA );
        itemA.id = id;

        ItemSelectionManagerStub manager;
        manager.add( &itemA );

        // edit note item in the sequence.
        int i = sequence.track[0].events()->findIndexFromId( id );
        itemA.clock = 1920;
        sequence.track[0].events()->set( i, itemA );

        {
            // assert status before calling updateSelectedContents.
            const map<int, VSQ_NS::Event> *originalItemList = manager.getOriginalItemList();
            map<int, VSQ_NS::Event>::const_iterator index = originalItemList->find( id );
            CPPUNIT_ASSERT( index != originalItemList->end() );
            CPPUNIT_ASSERT_EQUAL( (tick_t)480, index->second.clock );
        }

        // call test target.
        manager.updateSelectedContents( 0, &sequence );

        {
            // assert status after calling updateSelectedContents.
            const map<int, VSQ_NS::Event> *originalItemList = manager.getOriginalItemList();
            map<int, VSQ_NS::Event>::const_iterator index = originalItemList->find( id );
            CPPUNIT_ASSERT( index != originalItemList->end() );
            CPPUNIT_ASSERT_EQUAL( (tick_t)1920, index->second.clock );
        }
    }

    CPPUNIT_TEST_SUITE( ItemSelectionManagerTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST( testAddRemove );
    CPPUNIT_TEST( testMoveItems );
    CPPUNIT_TEST( testRevertSelectionStatusTo );
    CPPUNIT_TEST( testAddRemoveUsingList );
    CPPUNIT_TEST( updateSelectedItemContents );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( ItemSelectionManagerTest );
