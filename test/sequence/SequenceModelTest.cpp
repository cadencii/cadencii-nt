#include "../Util.hpp"
#include "../../sequence/SequenceModel.hpp"
#include "../../command/EditEventCommand.hpp"
#include <gtest/gtest.h>

using namespace vsq;
using namespace cadencii;

class SequenceModelStub : public SequenceModel{
public:
    SequenceModelStub() :
        SequenceModel()
    {
        sequence = Sequence( "foo", 1, 4, 4, 500000 );
        Track track( "name", "singer" );
        track.events().clear();
        Event singer( 0, EventType::SINGER );
        track.events().add( singer, 1 );
        Event note( 1920, EventType::NOTE );
        note.note = 60;
        track.events().add( note, 2 );
        Event note2( 1921, EventType::NOTE );
        track.events().add( note2, 3 );
        sequence.track(0) = track;
        sequence.updateTotalTicks();
    }
};

TEST(SequenceModelTest, test)
{
    SequenceModelStub model;

    Event editedNote = *model.getSequence()->track(0).events().findFromId(2);
    editedNote.note = 61;
    EditEventCommand *command = new EditEventCommand( 0, 2, editedNote );

    EXPECT_TRUE( false == model.canRedo() );
    EXPECT_TRUE( false == model.canUndo() );

    model.execute( command );

    EXPECT_EQ(61, model.getSequence()->track(0).events().findFromId(2)->note);
    EXPECT_TRUE( true == model.canUndo() );
    EXPECT_TRUE( false == model.canRedo() );

    model.undo();

    EXPECT_EQ(60, model.getSequence()->track(0).events().findFromId(2)->note);
    EXPECT_TRUE( false == model.canUndo() );
    EXPECT_TRUE( true == model.canRedo() );

    model.redo();

    EXPECT_EQ(61, model.getSequence()->track(0).events().findFromId(2)->note);
    EXPECT_TRUE( true == model.canUndo() );
    EXPECT_TRUE( false == model.canRedo() );

    model.reset( Sequence() );
    EXPECT_TRUE( false == model.canRedo() );
    EXPECT_TRUE( false == model.canUndo() );

    delete command;
}

/**
 * @brief コマンド実行後に、イベントの並べ替えが行われていること
 */
TEST(SequenceModelTest, testSortAfterExecute)
{
    SequenceModelStub model;
    vsq::Event::List const& list = model.getSequence()->track(0).events();

    Event editedNote = *model.getSequence()->track(0).events().findFromId(2);
    editedNote.tick = 1922;
    // 編集前
    // clock=0, SINGER, id=1
    // clock=1920, NOTE, id=2
    // clock=1921, NOTE, id=3
    EXPECT_EQ( 1, list.get( 0 )->id );
    EXPECT_EQ( 2, list.get( 1 )->id );
    EXPECT_EQ( 3, list.get( 2 )->id );
    EXPECT_EQ( (vsq::tick_t)1921, model.getSequence()->totalTicks() );
    EditEventCommand command( 0, 2, editedNote );
    model.execute( &command );

    // 編集後
    // clock=0, SINGER, id=1
    // clock=1921, NOTE, id=3
    // clock=1922, NOTE, id=2
    EXPECT_EQ( 1, list.get( 0 )->id );
    EXPECT_EQ( 3, list.get( 1 )->id );
    EXPECT_EQ( 2, list.get( 2 )->id );
    EXPECT_EQ( (vsq::tick_t)1922, model.getSequence()->totalTicks() );
}
