#include <catch2/catch.hpp>
#include <trompeloeil.hpp>
#include "cop/detail/HandlerWrapper.hpp"
#include "cop/Event.hpp"

enum events {
    eMyFirstEvent = 1,
    eMySecondEvent
};

struct myFirstEvent : cop::Event<eMyFirstEvent> {
    int data = 41;
    template<class Coder>
    auto parse(Coder coder) {
        return coder | data;
    }
};

struct mySecondEvent : cop::Event<eMySecondEvent> {
    int data = 42;
    double d = 2.9;
    int* c;

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
            std::array<std::byte, 1023> buf
                {std::byte{1}, std::byte{0}, std::byte{0}, std::byte{0},
                 std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}};
            using ReadIt = std::array<std::byte, 1023>::const_iterator;
            cop::detail::HandlerWrapper<Handler, ReadIt, std::tuple<mySecondEvent>, true> handler;
        

            auto error = handler.handleEvent(eMySecondEvent, buf.begin(), buf.end());

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
