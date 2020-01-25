#include <catch2/catch.hpp>
#include "trompeloeil.hpp"
#include "COP_Event.hpp"
#include "COP_Channel.hpp"

/*class Mock {
public:
    MAKE_MOCK1(read, void(COP::ReceivedEvent&));
};
Mock m;
REQUIRE_CALL(m, read(trompeloeil::_)).TIMES(1);*/

SCENARIO( "Events can be sent and received", "[Event]" ) {
    GIVEN( "An event containing a boolean and a channel" ) {
        COP::Event a;
        COP::Channel b;

        WHEN( "the event is sent" ) {
            b.sendEvent(a);

            THEN( "the buffer will be empty" ) {
                REQUIRE( true );
            }
        }
        WHEN( "the event is sent and somebody has registered for the event" ) {
            static constexpr int data = 42;
            a.write(data);
            b.sendEvent(a);

            int receiveBuffer = 0;
            
            b.registerEvent(receiveBuffer);

            THEN( "the data is writte to the destination buffer" ) {
                REQUIRE( receiveBuffer == data );
            }
        }
    }
}


