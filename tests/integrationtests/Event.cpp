#include <catch2/catch.hpp>
#include "trompeloeil.hpp"
#include "Event.hpp"
#include "Channel.hpp"

/*class Mock {
public:
    MAKE_MOCK1(read, void(COP::ReceivedEvent&));
};
Mock m;
REQUIRE_CALL(m, read(trompeloeil::_)).TIMES(1);*/

SCENARIO( "Events can be sent and received", "[Event]" ) {
    GIVEN( "An event containing a boolean and a channel" ) {
        enum events {
            eMyEvent = 1
        };
        struct myEvent : COP::Event<eMyEvent> {
            int data;
        };

        struct myREvent : COP::ReceivedEvent<eMyEvent> {
            int data;
        };
        struct myEvent myE;
        struct myREvent myR;
        COP::Channel sender;
        COP::Channel receiver;

        WHEN( "the event is sent" ) {
            sender.sendEvent(myE);

            THEN( "the buffer will be empty" ) {
                REQUIRE( true );
            }
        }
        WHEN( "the event is sent and somebody has registered for the event" ) {
            myE.data = 42;
            sender.sendEvent(myE);

            THEN( "the data is written to the destination buffer" ) {
                REQUIRE( myE.data == myR.data );
            }
        }
    }
}


