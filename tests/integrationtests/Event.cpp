#include <catch2/catch.hpp>
#include "trompeloeil.hpp"
#include "Event.hpp"
#include "Channel.hpp"
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

struct Handler
{
    void handle(myREvent& e) {
            REQUIRE(e.data == 44);
    }

    void handle(myEvent& e) {
            REQUIRE(e.data == 41);
    }
};

struct HandlerMock
{
    MAKE_MOCK1 (handle, void(myREvent&));

    MAKE_MOCK1 (handle, void(myEvent& ));
};

SCENARIO( "Events can be generated", "[Event]" ) {
    GIVEN( "An event containing an id" ) {
        
        COP::IdLayer<Handler, AllMessages> idlayer;
        
        WHEN("the id layer reads an id") {

            auto error = idlayer.read(eMyEvent);

            THEN("a message is created") {
                REQUIRE(COP::ProtocolErrc::success ==  error );
            }
        
            /*THEN("it will create the message") {
                myEvent* a = dynamic_cast<myEvent*>(ptr.get());

                if(a) {
                    REQUIRE(41 == a->data);
                }
                else {
                    REQUIRE(false);
                }
            }*/
        }
        
    }
}


