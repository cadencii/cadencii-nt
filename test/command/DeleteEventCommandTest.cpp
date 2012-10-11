#include "../Util.hpp"
#include "../../command/DeleteEventCommand.hpp"

using namespace std;
using namespace cadencii;
using namespace VSQ_NS;

class DeleteEventCommandTest : public CppUnit::TestCase{
public:
    void testExecute(){
        Sequence sequence( "foo", 1, 4, 4, 500000 );
        sequence.track[0].getEvents()->clear();
        Event singer( 0, EventType::SINGER );
        Event note( 480, EventType::NOTE );
        int idSinger = sequence.track[0].getEvents()->add( singer );
        int idNote = sequence.track[0].getEvents()->add( note );

        vector<int> idList;
        idList.push_back( idSinger );
        idList.push_back( idNote );
        DeleteEventCommand command( 0, idList );

        CPPUNIT_ASSERT_EQUAL( 2, sequence.track[0].getEvents()->size() );
        AbstractCommand *reverseCommand = command.execute( &sequence );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.track[0].getEvents()->size() );

        AbstractCommand *trash = reverseCommand->execute( &sequence );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, sequence.track[0].getEvents()->get( 0 )->clock );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, sequence.track[0].getEvents()->get( 1 )->clock );

        delete reverseCommand;
        delete trash;
    }

    CPPUNIT_TEST_SUITE( DeleteEventCommandTest );
    CPPUNIT_TEST( testExecute );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( DeleteEventCommandTest );
