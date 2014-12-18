#include "../Util.hpp"
#include "../../command/DeleteEventCommand.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace cadencii;
using namespace vsq;

TEST(DeleteEventCommandTest, testExecute)
{
    Sequence sequence( "foo", 1, 4, 4, 500000 );
    sequence.track(0).events().clear();
    Event singer( 0, EventType::SINGER );
    Event note( 480, EventType::NOTE );
    int idSinger = sequence.track(0).events().add(singer);
    int idNote = sequence.track(0).events().add(note);

    vector<int> idList;
    idList.push_back( idSinger );
    idList.push_back( idNote );
    DeleteEventCommand command( 0, idList );

    EXPECT_EQ(2, sequence.track(0).events().size());
    AbstractCommand *reverseCommand = command.execute( &sequence );
    EXPECT_EQ(0, sequence.track(0).events().size());

    AbstractCommand *trash = reverseCommand->execute( &sequence );
    EXPECT_EQ((tick_t)0, sequence.track(0).events().get(0)->tick);
    EXPECT_EQ((tick_t)480, sequence.track(0).events().get(1)->tick);

    delete reverseCommand;
    delete trash;
}
