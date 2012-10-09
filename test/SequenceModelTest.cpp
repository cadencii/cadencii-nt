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
        sequence.track[0] = track;
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
    }

    CPPUNIT_TEST_SUITE( SequenceModelTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( SequenceModelTest );
