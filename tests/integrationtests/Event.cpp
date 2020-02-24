#include <catch2/catch.hpp>
#include "trompeloeil.hpp"
#include "Event.hpp"
#include "Channel.hpp"
#include "Handler.hpp"

/*class Mock {
public:
    MAKE_MOCK1(read, void(COP::ReceivedEvent&));
};
Mock m;
REQUIRE_CALL(m, read(trompeloeil::_)).TIMES(1);*/

enum events {
    eMyEvent = 1
};
struct myEvent : COP::Event<eMyEvent, myEvent> {
    int data;
};

struct myREvent : COP::ReceivedEvent<eMyEvent, myREvent> {
    int data;
};

using AllMessages = std::tuple<myEvent, myREvent>;

HANDLER
DISPATCH

struct Handler : COP::Handler
{
    void handle(myREvent& e) {
            REQUIRE(e.data == 42);
    }

    void handle(myEvent& e) {
            REQUIRE(e.data == 42);
    }
};


SCENARIO( "Events can be sent and received", "[Event]" ) {
    GIVEN( "An event containing a boolean and a channel" ) {
        COP::Channel<Handler> sender;
        COP::Channel<Handler> receiver;
        struct myEvent myE;
        struct myREvent myR;


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


