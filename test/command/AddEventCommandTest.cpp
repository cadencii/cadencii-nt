#include "../Util.hpp"
#include "../../command/AddEventCommand.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace cadencii;
using namespace vsq;

TEST(AddEventCommandTest, testExecute)
{
    Sequence sequence( "foo", 1, 4, 4, 500000 );
    sequence.track(0).events().clear();

    std::vector<Event> eventList;
    Event singer( 0, EventType::SINGER );
    Event note( 480, EventType::NOTE );
    eventList.push_back( singer );
    eventList.push_back( note );

    AddEventCommand command( 0, eventList );

    EXPECT_EQ(0, sequence.track(0).events().size());
    AbstractCommand *reverseCommand = command.execute( &sequence );
    EXPECT_EQ(2, sequence.track(0).events().size());
    EXPECT_EQ((tick_t)0, sequence.track(0).events().get(0)->tick);
    EXPECT_EQ((tick_t)480, sequence.track(0).events().get(1)->tick);

    AbstractCommand *trash = reverseCommand->execute( &sequence );
    EXPECT_EQ(0, sequence.track(0).events().size());

    delete reverseCommand;
    delete trash;
}
