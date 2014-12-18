#include "Util.hpp"
#include "../ItemSelectionManager.hpp"
#include <libvsq/libvsq.h>
#include <set>
#include <gtest/gtest.h>

using namespace vsq;
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

TEST(ItemSelectionManagerTest, test)
{
    ItemSelectionStatusListenerStub listener;
    ItemSelectionManager manager;
    manager.addStatusListener( &listener );
    Event *item = new Event( 0, EventType::NOTE );
    EXPECT_TRUE( false == manager.isContains( item ) );
    EXPECT_EQ( 0, listener.getStatusChangedCount() );
    manager.add( item );
    EXPECT_TRUE( true == manager.isContains( item ) );
    EXPECT_EQ( 1, listener.getStatusChangedCount() );
    manager.clear();
    EXPECT_TRUE( false == manager.isContains( item ) );
    EXPECT_EQ( 2, listener.getStatusChangedCount() );
    delete item;
}

TEST(ItemSelectionManagerTest, testAddRemove)
{
    ItemSelectionStatusListenerStub listener;
    ItemSelectionManager manager;
    manager.addStatusListener( &listener );
    Event *item = new Event( 0, EventType::NOTE );
    manager.add( item );
    EXPECT_TRUE( true == manager.isContains( item ) );
    EXPECT_EQ( (size_t)1, manager.getEventItemList()->size() );
    EXPECT_EQ( 1, listener.getStatusChangedCount() );
    manager.add( item );
    EXPECT_EQ( (size_t)1, manager.getEventItemList()->size() );
    EXPECT_EQ( 1, listener.getStatusChangedCount() );
    manager.remove( item );
    EXPECT_EQ( 2, listener.getStatusChangedCount() );
    EXPECT_TRUE( false == manager.isContains( item ) );
    EXPECT_EQ( (size_t)0, manager.getEventItemList()->size() );
    EXPECT_NO_THROW( manager.remove( item ) );
    EXPECT_EQ( 2, listener.getStatusChangedCount() );
    delete item;
}

TEST(ItemSelectionManagerTest, moveItems)
{
    ItemSelectionManager manager;
    Event *itemA = new Event( 0, EventType::NOTE );
    itemA->tick = 480;
    itemA->note = 50;
    itemA->id = 1;
    Event *itemB = new Event( 0, EventType::NOTE );
    itemB->tick = 1920;
    itemB->note = 52;
    itemB->id = 2;
    manager.add( itemA );
    manager.add( itemB );

    manager.moveItems( 10, 5 );

    const std::map<const vsq::Event *, std::shared_ptr<vsq::Event>> *itemList = manager.getEventItemList();
    EXPECT_EQ( (tick_t)(480 + 10), itemList->at( itemA )->tick );
    EXPECT_EQ( 50 + 5, itemList->at( itemA )->note );
    EXPECT_EQ( (tick_t)(1920 + 10), itemList->at( itemB )->tick );
    EXPECT_EQ( 52 + 5, itemList->at( itemB )->note );

    delete itemA;
    delete itemB;
}

TEST(ItemSelectionManagerTest, moveItemsWithRejected)
{
    {
        ItemSelectionManager manager;
        Event *itemA = new Event( 0, EventType::NOTE );
        itemA->tick = 480;
        itemA->note = 50;
        itemA->id = 1;
        Event *itemB = new Event( 0, EventType::NOTE );
        itemB->tick = 1920;
        itemB->note = 52;
        itemB->id = 2;
        manager.add( itemA );
        manager.add( itemB );

        manager.moveItems( -481, 1 );
        // moveItems method rejected because clock goes little than 0
        int actualDeltaClock = -480;
        int actualDeltaNoteNumber = 1;
        const std::map<const vsq::Event *, std::shared_ptr<vsq::Event>> *itemList = manager.getEventItemList();
        EXPECT_EQ( (tick_t)(480 + actualDeltaClock), itemList->at( itemA )->tick );
        EXPECT_EQ( 50 + actualDeltaNoteNumber, itemList->at( itemA )->note );
        EXPECT_EQ( (tick_t)(1920 + actualDeltaClock), itemList->at( itemB )->tick );
        EXPECT_EQ( 52 + actualDeltaNoteNumber, itemList->at( itemB )->note );

        delete itemA;
        delete itemB;
    }

    {
        ItemSelectionManager manager;
        Event *itemA = new Event( 0, EventType::NOTE );
        itemA->tick = 480;
        itemA->note = 50;
        itemA->id = 1;
        Event *itemB = new Event( 0, EventType::NOTE );
        itemB->tick = 1920;
        itemB->note = 52;
        itemB->id = 2;
        manager.add( itemA );
        manager.add( itemB );

        manager.moveItems( 1, 76 );
        // moveItems method rejected because note goes larger than 127.
        int actualDeltaClock = 1;
        int actualDeltaNoteNumber = 75;
        const std::map<const vsq::Event *, std::shared_ptr<vsq::Event>> *itemList = manager.getEventItemList();
        EXPECT_EQ( (tick_t)(480 + actualDeltaClock), itemList->at( itemA )->tick );
        EXPECT_EQ( 50 + actualDeltaNoteNumber, itemList->at( itemA )->note );
        EXPECT_EQ( (tick_t)(1920 + actualDeltaClock), itemList->at( itemB )->tick );
        EXPECT_EQ( 52 + actualDeltaNoteNumber, itemList->at( itemB )->note );

        delete itemA;
        delete itemB;
    }

    {
        ItemSelectionManager manager;
        Event *itemA = new Event( 0, EventType::NOTE );
        itemA->tick = 480;
        itemA->note = 50;
        itemA->id = 1;
        Event *itemB = new Event( 0, EventType::NOTE );
        itemB->tick = 1920;
        itemB->note = 52;
        itemB->id = 2;
        manager.add( itemA );
        manager.add( itemB );

        manager.moveItems( 2, -51 );
        // moveItems method rejected because note goes little than 0.
        int actualDeltaClock = 2;
        int actualDeltaNoteNumber = -50;
        const std::map<const vsq::Event *, std::shared_ptr<vsq::Event>> *itemList = manager.getEventItemList();
        EXPECT_EQ( (tick_t)(480 + actualDeltaClock), itemList->at( itemA )->tick );
        EXPECT_EQ( 50 + actualDeltaNoteNumber, itemList->at( itemA )->note );
        EXPECT_EQ( (tick_t)(1920 + actualDeltaClock), itemList->at( itemB )->tick );
        EXPECT_EQ( 52 + actualDeltaNoteNumber, itemList->at( itemB )->note );

        delete itemA;
        delete itemB;
    }
}

TEST(ItemSelectionManagerTest, testRevertSelectionStatusTo)
{
    ItemSelectionManager manager;
    Event *itemA = new Event( 0, EventType::NOTE );
    itemA->tick = 480;
    itemA->note = 50;
    itemA->id = 1;
    Event *itemB = new Event( 0, EventType::NOTE );
    itemB->tick = 1920;
    itemB->note = 52;
    itemB->id = 2;
    manager.add( itemA );
    manager.add( itemB );

    ItemSelectionManager target;
    target.revertSelectionStatusTo( manager );
    EXPECT_TRUE( true == target.isContains( itemA ) );
    EXPECT_TRUE( true == target.isContains( itemB ) );

    EXPECT_EQ( (size_t)2, target.getEventItemList()->size() );

    delete itemA;
    delete itemB;
}

TEST(ItemSelectionManagerTest, testAddRemoveUsingList)
{
    ItemSelectionManager manager;
    Event itemA( 0, EventType::NOTE );
    itemA.tick = 480;
    itemA.note = 50;
    Event itemB( 0, EventType::NOTE );
    itemB.tick = 1920;
    itemB.note = 52;
    std::set<const vsq::Event *> list;
    list.insert( &itemA );
    list.insert( &itemB );

    manager.add( list );
    EXPECT_TRUE( true == manager.isContains( &itemA ) );
    EXPECT_TRUE( true == manager.isContains( &itemB ) );

    manager.remove( list );
    EXPECT_TRUE( false == manager.isContains( &itemA ) );
    EXPECT_TRUE( false == manager.isContains( &itemB ) );
}

TEST(ItemSelectionManagerTest, updateSelectedItemContents)
{
    Sequence sequence;
    Event itemA( 0, EventType::NOTE );
    itemA.tick = 480;
    itemA.note = 50;
    int id = sequence.track(0).events().add(itemA);
    itemA.id = id;

    ItemSelectionManager manager;
    manager.add( &itemA );

    // edit note item in the sequence.
    int i = sequence.track(0).events().findIndexFromId(id);
    itemA.tick = 1920;
    sequence.track(0).events().set(i, itemA);

    {
        // assert status before calling updateSelectedContents.
        const std::map<const vsq::Event *, std::shared_ptr<vsq::Event>> *eventItemlist = manager.getEventItemList();
        auto index = eventItemlist->find(&itemA);
        EXPECT_TRUE( index != eventItemlist->end() );
        EXPECT_EQ( (tick_t)480, index->second->tick );
    }

    // call test target.
    manager.updateSelectedContents( 0, &sequence );

    {
        // assert status after calling updateSelectedContents.
        const std::map<const vsq::Event *, std::shared_ptr<vsq::Event>> *eventItemlist = manager.getEventItemList();
        auto index = eventItemlist->find(&itemA);
        EXPECT_TRUE( index != eventItemlist->end() );
        EXPECT_EQ( (tick_t)1920, index->second->tick );
    }
}
