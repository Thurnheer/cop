#include <catch2/catch.hpp>
#include "trompeloeil.hpp"
#include "Event.hpp"
#include "Channel.hpp"
#include "Handler.hpp"
#include "IdLayer.hpp"

enum events {
    eMyEvent = 1
};
struct myEvent : COP::Event<eMyEvent, myEvent> {
    int data = 41;
};

struct myREvent : COP::ReceivedEvent<eMyEvent, myREvent> {
    int data = 42;
};

using AllMessages = std::tuple<myEvent, myREvent>;

HANDLER
DISPATCH

struct Handler : COP::Handler
{
    void handle(myREvent& e) {
            REQUIRE(e.data == 44);
    }

    void handle(myEvent& e) {
            REQUIRE(e.data == 46);
    }
};


SCENARIO( "Events can be generated", "[Event]" ) {
    GIVEN( "An event containing an id" ) {
        
        COP::IdLayer<AllMessages> idlayer;
        
        WHEN("the id layer reads an id") {

            auto ptr = idlayer.read(eMyEvent);

            THEN("a message is created") {
                REQUIRE( ptr );
            }
        
            THEN("it will create the message") {
                myEvent* a = dynamic_cast<myEvent*>(ptr.get());

                if(a) {
                    REQUIRE(41 == a->data);
                }
                else {
                    REQUIRE(false);
                }
            }
        }
        
    }
}


