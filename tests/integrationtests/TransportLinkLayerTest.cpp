#include <catch2/catch.hpp>
#include "cop/TransportLinkLayer.hpp"
#include "cop/Event.hpp"
#include <tuple>

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

    template<class Coder>
    auto parse(Coder coder) {
         return coder | data | d;
    }
};

SCENARIO("The transport link layer chercks the header and creates the messages") {
    GIVEN("A message") {
        
        struct Handler
        {
            void handle(mySecondEvent& e) {
                REQUIRE(e.data == 5);
                REQUIRE(e.d == 0.0);
            }
        };
        const std::array<std::byte, 1023> MESSAGE
            {std::byte{0x02}, std::byte{0}, std::byte{2}, std::byte{5},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}};
        using ReadIt = std::array<std::byte, 1023>::const_iterator;

        auto it = MESSAGE.begin(); auto end = MESSAGE.end();

        cop::TransportLinkLayer<Handler, ReadIt, std::tuple<mySecondEvent> > tll;
        THEN("it will create the according class with data") {
            REQUIRE(cop::ProtocolErrc::success == tll.receive(it, end));
        }
    }
    GIVEN("A message with a wrong ID") {
        
        struct Handler
        {
            void handle(mySecondEvent& e) {
                REQUIRE(false);
                REQUIRE(e.data == 1);
                REQUIRE(e.d == 0.0);
            }
        };
        const std::array<std::byte, 1023> MESSAGE
            {std::byte{0x02}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}};
        using ReadIt = std::array<std::byte, 1023>::const_iterator;

        auto it = MESSAGE.begin(); auto end = MESSAGE.end();

        cop::TransportLinkLayer<Handler, ReadIt, std::tuple<mySecondEvent> > tll;
        THEN("it will return an error") {
            REQUIRE(cop::ProtocolErrc::invalid_message_id == tll.receive(it, end));
        }
    }
}
