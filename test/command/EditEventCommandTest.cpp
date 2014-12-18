#include "../Util.hpp"
#include "../../command/EditEventCommand.hpp"
#include <gtest/gtest.h>

using namespace cadencii;
using namespace vsq;

TEST(EditEventCommandTest, testExecute)
{
    Sequence sequence( "foo", 1, 4, 4, 500000 );
    Track track( "name", "singer" );
    track.events().clear();
    Event singer( 0, EventType::SINGER );
    track.events().add( singer, 1 );
    Event note( 1920, EventType::NOTE );
    note.note = 60;
    track.events().add( note, 2 );
    sequence.track(0) = track;

    Event editedNote = note;
    editedNote.note = 61;
    EditEventCommand command( 0, 2, editedNote );
    AbstractCommand *inverseCommand = command.execute( &sequence );

    EXPECT_EQ(61, sequence.track(0).events().findFromId(2)->note);

    AbstractCommand *garbage = inverseCommand->execute( &sequence );
    EXPECT_EQ(60, sequence.track(0).events().findFromId(2)->note);

    delete inverseCommand;
    delete garbage;
}
