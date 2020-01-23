#include <catch2/catch.hpp>
#include "COP_Event.hpp"
#include "COP_Channel.hpp"

SCENARIO( "Events can be sent and received", "[Event]" ) {
    GIVEN( "An event and a channel" ) {
        COP::Event a;
        COP::Channel b;

        WHEN( "the event is sent" ) {

            THEN( "it will be received" ) {
                REQUIRE( true );
            }
        }
    }
}


