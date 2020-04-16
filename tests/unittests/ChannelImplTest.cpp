#include "cop/detail/ChannelImpl.hpp"
#include <catch2/catch.hpp>

enum EventIds {
    eFirstEvent = 1,
    eSecondEvent
};

struct FirstEvent : cop::Event<eFirstEvent> {
    int data = 3;
    template<class Coder>
    auto parse(Coder coder) {
        return coder | data;
    }
};

struct SecondEvent : cop::Event<eSecondEvent> {
    int idata = 9;
    float ddata = 1.0;
    template<class Coder>
    auto parse(Coder coder) {
        return coder | idata | ddata;
    }
};

SCENARIO( "The channel implemenation sends messages" ) {
    GIVEN ( "an event containing an id" ) {

        WHEN( "the event is sent" ) {

            std::array<std::byte, 32> buf;
            using ReadIt = std::array<std::byte, 32>::iterator;

            cop::detail::ChannelImpl<ReadIt> channel;
            channel.sendEvent(std::move(FirstEvent()), buf.begin(), buf.end());
            THEN ( "it will be serialised" ) {
                REQUIRE(buf[0] == std::byte{0x03});
            }
        }

        WHEN( " a event with multiple data is sent" ) {
            std::array<std::byte, 32> buf{
                std::byte(0x42), std::byte(0x42), std::byte(0x42), std::byte(0x42),
                std::byte(0x42), std::byte(0x42), std::byte(0x42), std::byte(0x42),
                std::byte(0x42), std::byte(0x42)
            };
            using ReadIt = std::array<std::byte, 32>::iterator;

            cop::detail::ChannelImpl<ReadIt> channel;
            channel.sendEvent(std::move(SecondEvent()), buf.begin(), buf.end());

            THEN ( "it will be serialised" ) {
                REQUIRE(buf[0] == std::byte{0x09});
                REQUIRE(buf[1] == std::byte{0x00});
                REQUIRE(buf[2] == std::byte{0x00});
                REQUIRE(buf[3] == std::byte{0x00});
                REQUIRE(buf[4] == std::byte{0x00});
                REQUIRE(buf[5] == std::byte{0x00});
                REQUIRE(buf[6] == std::byte{0x80});
                REQUIRE(buf[7] == std::byte{0x3F});
                REQUIRE(buf[8] == std::byte{0x42});
                REQUIRE(buf[9] == std::byte{0x42});
            }
        }
    }
}
