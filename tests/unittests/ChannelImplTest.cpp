#include "detail/ChannelImpl.hpp"
#include <catch2/catch.hpp>

enum EventIds {
    eFirstEvent = 1,
    eSecondEvent
};

struct FirstEvent : cop::Event<eFirstEvent> {
    int data = 3;
    template<class Coder>
    void parse(Coder coder) {
        coder | data;
    }
};

struct SecondEvent : cop::Event<eSecondEvent> {
    double data = 2.3;
    template<class Coder>
    void parse(Coder coder) {
        coder | data;
    }
};

SCENARIO( "The channel implemenation sends messages" ) {
    GIVEN ( "an event containing an id" ) {

        WHEN( "the event is sent" ) {

            std::array<uint8_t, 32> buf;
            using ReadIt = std::array<uint8_t, 32>::iterator;

            cop::detail::ChannelImpl<ReadIt> channel;
            channel.sendEvent(std::move(FirstEvent()), buf.begin(), buf.end());
            THEN ( "it will be serialised" ) {
                REQUIRE(buf[0] == 3);
            }
        }
    }
}
