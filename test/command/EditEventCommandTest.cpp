#include "../Util.hpp"
#include "../../command/EditEventCommand.hpp"

using namespace cadencii;
using namespace VSQ_NS;

class EditEventCommandTest : public CppUnit::TestCase{
public:
    void testExecute(){
        Sequence sequence( "foo", 1, 4, 4, 500000 );
        Track track( "name", "singer" );
        track.getEvents()->clear();
        Event singer( 0, EventType::SINGER );
        track.getEvents()->add( singer, 1 );
        Event note( 1920, EventType::NOTE );
        note.note = 60;
        track.getEvents()->add( note, 2 );
        sequence.track[0] = track;

        Event editedNote = note;
        editedNote.note = 61;
        EditEventCommand command( 0, 2, editedNote );
        AbstractCommand *inverseCommand = command.execute( &sequence );

        CPPUNIT_ASSERT_EQUAL( 61, sequence.track[0].getEvents()->findFromId( 2 )->note );

        AbstractCommand *garbage = inverseCommand->execute( &sequence );
        CPPUNIT_ASSERT_EQUAL( 60, sequence.track[0].getEvents()->findFromId( 2 )->note );

        delete inverseCommand;
        delete garbage;
    }

    CPPUNIT_TEST_SUITE( EditEventCommandTest );
    CPPUNIT_TEST( testExecute );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( EditEventCommandTest );
