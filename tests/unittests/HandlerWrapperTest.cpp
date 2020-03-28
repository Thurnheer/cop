#include <catch2/catch.hpp>
#include <trompeloeil.hpp>
#include "detail/HandlerWrapper.hpp"

enum events {
    eMyEvent = 1
};
struct myEvent : cop::Event<eMyEvent, myEvent> {
    int data = 41;
};

struct myREvent : cop::ReceivedEvent<eMyEvent, myREvent> {
    int data = 42;
};

using AllMessages = std::tuple<myEvent, myREvent>;

struct HandlerMock
{
    MAKE_MOCK1 (handle, void(myREvent&));

    MAKE_MOCK1 (handle, void(myEvent& ));
};


SCENARIO( "The HandlerWrapper generates Events", "[generate Events]" ) {

    GIVEN( "An event containing an id" ) {
        
        struct Handler
        {
            void handle(myREvent& e) {
                    REQUIRE(e.data == 42);
            }
        };
        
        WHEN("the id layer reads an id") {
            cop::detail::HandlerWrapper<Handler, std::tuple<myREvent>, true> handler;
        

            auto error = handler.handle(eMyEvent);

            THEN("a message is created") {
                REQUIRE(cop::ProtocolErrc::success ==  error );
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
