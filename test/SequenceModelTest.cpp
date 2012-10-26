#include "Util.hpp"
#include "../SequenceModel.hpp"
#include "../command/EditEventCommand.hpp"

using namespace VSQ_NS;
using namespace cadencii;

class SequenceModelStub : public SequenceModel{
public:
    SequenceModelStub() :
        SequenceModel()
    {
        sequence = Sequence( "foo", 1, 4, 4, 500000 );
        Track track( "name", "singer" );
        track.getEvents()->clear();
        Event singer( 0, EventType::SINGER );
        track.getEvents()->add( singer, 1 );
        Event note( 1920, EventType::NOTE );
        note.note = 60;
        track.getEvents()->add( note, 2 );
        Event note2( 1921, EventType::NOTE );
        track.getEvents()->add( note2, 3 );
        sequence.track[0] = track;
        sequence.updateTotalClocks();
    }
};

class SequenceModelTest : public CppUnit::TestCase{
public:
    void test(){
        SequenceModelStub model;

        Event editedNote = *model.getSequence()->track[0].getConstEvents()->findFromId( 2 );
        editedNote.note = 61;
        EditEventCommand *command = new EditEventCommand( 0, 2, editedNote );

        CPPUNIT_ASSERT( false == model.canRedo() );
        CPPUNIT_ASSERT( false == model.canUndo() );

        model.execute( command );

        CPPUNIT_ASSERT_EQUAL( 61, model.getSequence()->track[0].getConstEvents()->findFromId( 2 )->note );
        CPPUNIT_ASSERT( true == model.canUndo() );
        CPPUNIT_ASSERT( false == model.canRedo() );

        model.undo();

        CPPUNIT_ASSERT_EQUAL( 60, model.getSequence()->track[0].getConstEvents()->findFromId( 2 )->note );
        CPPUNIT_ASSERT( false == model.canUndo() );
        CPPUNIT_ASSERT( true == model.canRedo() );

        model.redo();

        CPPUNIT_ASSERT_EQUAL( 61, model.getSequence()->track[0].getConstEvents()->findFromId( 2 )->note );
        CPPUNIT_ASSERT( true == model.canUndo() );
        CPPUNIT_ASSERT( false == model.canRedo() );

        model.reset( Sequence() );
        CPPUNIT_ASSERT( false == model.canRedo() );
        CPPUNIT_ASSERT( false == model.canUndo() );

        delete command;
    }

    /**
     * @brief コマンド実行後に、イベントの並べ替えが行われていること
     */
    void testSortAfterExecute(){
        SequenceModelStub model;
        const VSQ_NS::Event::List *list = model.getSequence()->track[0].getConstEvents();

        Event editedNote = *model.getSequence()->track[0].getConstEvents()->findFromId( 2 );
        editedNote.clock = 1922;
        // 編集前
        // clock=0, SINGER, id=1
        // clock=1920, NOTE, id=2
        // clock=1921, NOTE, id=3
        CPPUNIT_ASSERT_EQUAL( 1, list->get( 0 )->id );
        CPPUNIT_ASSERT_EQUAL( 2, list->get( 1 )->id );
        CPPUNIT_ASSERT_EQUAL( 3, list->get( 2 )->id );
        CPPUNIT_ASSERT_EQUAL( (VSQ_NS::tick_t)1921, model.getSequence()->getTotalClocks() );
        EditEventCommand command( 0, 2, editedNote );
        model.execute( &command );

        // 編集後
        // clock=0, SINGER, id=1
        // clock=1921, NOTE, id=3
        // clock=1922, NOTE, id=2
        CPPUNIT_ASSERT_EQUAL( 1, list->get( 0 )->id );
        CPPUNIT_ASSERT_EQUAL( 3, list->get( 1 )->id );
        CPPUNIT_ASSERT_EQUAL( 2, list->get( 2 )->id );
        CPPUNIT_ASSERT_EQUAL( (VSQ_NS::tick_t)1922, model.getSequence()->getTotalClocks() );
    }

    CPPUNIT_TEST_SUITE( SequenceModelTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST( testSortAfterExecute );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( SequenceModelTest );
