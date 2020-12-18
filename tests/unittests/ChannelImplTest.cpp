#include "cop/detail/ChannelImpl.hpp"
#include <catch2/catch.hpp>
#include <array>

enum EventIds {
    eFirstEvent = 1,
    eSecondEvent
};

static const int DATA_THREE = 3;
static const int DATA_NINE = 9;
static const int DATA_THIRTYTWO = 32;
static const float DATA_FLOAT = 1.0F;

struct FirstEvent : cop::Event<eFirstEvent> {
    int data = DATA_THREE; // cppcheck-suppress unusedStructMember
    template<class Coder>
    auto parse(Coder coder) {
        return coder | data;
    }
};

struct SecondEvent : cop::Event<eSecondEvent> {
    int idata = DATA_NINE; // cppcheck-suppress unusedStructMember
    float ddata = DATA_FLOAT; // cppcheck-suppress unusedStructMember
    template<class Coder>
    auto parse(Coder coder) {
        return coder | idata | ddata;
    }
};

enum CommandIds {
    eFirstCommand = 1,
    eSecondCommand
};
struct FirstCommand : cop::Command<eFirstCommand> {
    int idata = DATA_THIRTYTWO; // cppcheck-suppress unusedStructMember
    float fdata = DATA_FLOAT; // cppcheck-suppress unusedStructMember
    template<class Coder>
    auto parse(Coder coder) {
        return coder | idata | fdata;
    }
};

static const int BUFFER_SIZE = 32;

SCENARIO( "The channel implemenation sends messages" ) {
    GIVEN ( "an event containing an id" ) {

        WHEN( "the event is sent" ) {

            std::array<std::byte, BUFFER_SIZE> buf{};

            cop::detail::ChannelImpl channel;
            channel.sendEvent(FirstEvent(), buf.begin(), buf.end());
            THEN ( "it will be serialised" ) {
                REQUIRE(buf[0] == std::byte{0x03});
            }
        }

        WHEN( " a event with multiple data is sent" ) {
            static const char SOME_DATA = 0x42;
            std::array<std::byte, BUFFER_SIZE> buf{
                std::byte(SOME_DATA), std::byte(SOME_DATA), std::byte(SOME_DATA), std::byte(SOME_DATA),
                std::byte(SOME_DATA), std::byte(SOME_DATA), std::byte(SOME_DATA), std::byte(SOME_DATA),
                std::byte(SOME_DATA), std::byte(SOME_DATA)
            };

            cop::detail::ChannelImpl channel;
            channel.sendEvent(SecondEvent(), buf.begin(), buf.end());

            THEN ( "it will be serialised" ) {
                REQUIRE(buf[0] == std::byte{0x09});
                REQUIRE(buf[1] == std::byte{0x00});
                REQUIRE(buf[2] == std::byte{0x00});
                REQUIRE(buf[3] == std::byte{0x00});
                REQUIRE(buf[4] == std::byte{0x00});
                REQUIRE(buf[5] == std::byte{0x00});
                REQUIRE(buf[6] == std::byte{0x80});
                REQUIRE(buf[7] == std::byte{0x3F});
                REQUIRE(buf[8] == std::byte{SOME_DATA});
                REQUIRE(buf[9] == std::byte{SOME_DATA});
            }
        }
    }

    GIVEN( "an command containing an id" ) {
        WHEN( "the command is sent" ) {
            static const char SOME_DATA = 0x42;
            std::array<std::byte, BUFFER_SIZE> buf{
                std::byte(SOME_DATA), std::byte(SOME_DATA), std::byte(SOME_DATA), std::byte(SOME_DATA),
                std::byte(SOME_DATA), std::byte(SOME_DATA), std::byte(SOME_DATA), std::byte(SOME_DATA),
                std::byte(SOME_DATA), std::byte(SOME_DATA)
            };

            cop::detail::ChannelImpl channel;
            channel.sendCommand(FirstCommand(), buf.begin(), buf.end());

            THEN ( "it will be serialised" ) {
                REQUIRE(buf[0] == std::byte{0x20});
                REQUIRE(buf[1] == std::byte{0x00});
                REQUIRE(buf[2] == std::byte{0x00});
                REQUIRE(buf[3] == std::byte{0x00});
                REQUIRE(buf[4] == std::byte{0x00});
                REQUIRE(buf[5] == std::byte{0x00});
                REQUIRE(buf[6] == std::byte{0x80});
                REQUIRE(buf[7] == std::byte{0x3F});
                REQUIRE(buf[8] == std::byte{SOME_DATA});
                REQUIRE(buf[9] == std::byte{SOME_DATA});
            }
        }
    }
}
