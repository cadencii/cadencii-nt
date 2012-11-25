#include "../Util.hpp"
#include "../../command/AddEventCommand.hpp"

using namespace std;
using namespace cadencii;
using namespace VSQ_NS;

class AddEventCommandTest : public CppUnit::TestCase{
public:
    void testExecute(){
        Sequence sequence( "foo", 1, 4, 4, 500000 );
        sequence.track(0)->events()->clear();

        std::vector<Event> eventList;
        Event singer( 0, EventType::SINGER );
        Event note( 480, EventType::NOTE );
        eventList.push_back( singer );
        eventList.push_back( note );

        AddEventCommand command( 0, eventList );

        CPPUNIT_ASSERT_EQUAL(0, sequence.track(0)->events()->size());
        AbstractCommand *reverseCommand = command.execute( &sequence );
        CPPUNIT_ASSERT_EQUAL(2, sequence.track(0)->events()->size());
        CPPUNIT_ASSERT_EQUAL((tick_t)0, sequence.track(0)->events()->get(0)->clock);
        CPPUNIT_ASSERT_EQUAL((tick_t)480, sequence.track(0)->events()->get(1)->clock);

        AbstractCommand *trash = reverseCommand->execute( &sequence );
        CPPUNIT_ASSERT_EQUAL(0, sequence.track(0)->events()->size());

        delete reverseCommand;
        delete trash;
    }

    CPPUNIT_TEST_SUITE( AddEventCommandTest );
    CPPUNIT_TEST( testExecute );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( AddEventCommandTest );
