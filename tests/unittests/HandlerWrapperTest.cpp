#include <catch2/catch.hpp>
#include <trompeloeil.hpp>
#include "cop/detail/HandlerWrapper.hpp"

enum events {
    eMyFirstEvent = 1,
    eMySecondEvent
};

struct myFirstEvent : cop::Id_t<eMyFirstEvent> {
    int data = 41;
    template<class Coder>
    auto parse(Coder coder) {
        return coder | data;
    }
};

struct mySecondEvent : cop::Id_t<eMySecondEvent> {
    int data = 42;
    double d = 2.9;

    template<class Coder>
    auto parse(Coder coder) {
         return coder | data | d;
    }
};

using AllMessages = std::tuple<myFirstEvent, mySecondEvent>;


SCENARIO( "The HandlerWrapper generates Events", "[generate Events]" ) {

    GIVEN( "An event containing an id" ) {
        
        struct Handler
        {
            void handle(mySecondEvent& e) {
                REQUIRE(e.data == 1);
                REQUIRE(e.d == 0.0);
            }
        };
        
        WHEN("the id layer reads an id") {
            std::array<uint8_t, 1023> buf{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            using WriteIt = std::array<uint8_t, 1023>::const_iterator;
            cop::detail::HandlerWrapper<Handler, WriteIt, std::tuple<mySecondEvent>, true> handler;
        

            auto error = handler.handle(eMySecondEvent, buf.begin(), buf.end());

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
