#include <catch2/catch.hpp>
#include <trompeloeil.hpp>
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
    mySecondEvent() = default;
    mySecondEvent(int da, double dd) : data(da), d(dd){}
    int data = 42;
    double d = 2.9;

    template<class Coder>
    auto parse(Coder coder) {
         return coder | data | d;
    }
    bool operator==(const mySecondEvent& s) {
        return data == s.data && d == s.d;
    }
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
struct HandlerMock
{
    MAKE_MOCK1(  handle, void(mySecondEvent& e));
};
#pragma GCC diagnostic pop

SCENARIO("The transport link layer chercks the header and creates the messages") {
    GIVEN("A message") {
        
        const std::array<std::byte, 1023> MESSAGE
            {std::byte{0x02}, std::byte{0}, std::byte{2}, std::byte{5},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}};
        using ReadIt = std::array<std::byte, 1023>::const_iterator;

        auto it = MESSAGE.begin(); auto end = MESSAGE.end();
        
        const mySecondEvent EVENT (5, 0.0);

        using trompeloeil::_;
        HandlerMock handler;
        REQUIRE_CALL(handler, handle(_))
            .LR_WITH(_1 == EVENT)
            .TIMES(1);
        cop::TransportLinkLayer<HandlerMock, ReadIt, std::tuple<mySecondEvent> > tll(handler);
        THEN("it will create the according class with data") {
            REQUIRE(cop::ProtocolErrc::success == tll.receive(it, end));
        }
    }
    GIVEN("A message with a wrong ID") {
        
        const std::array<std::byte, 1023> MESSAGE
            {std::byte{0x02}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}};
        using ReadIt = std::array<std::byte, 1023>::const_iterator;

        auto it = MESSAGE.begin(); auto end = MESSAGE.end();

        HandlerMock handler;
        using trompeloeil::_;
        FORBID_CALL(handler, handle(_));
        cop::TransportLinkLayer<HandlerMock, ReadIt, std::tuple<mySecondEvent> > tll(handler);
        THEN("it will return an error") {
            REQUIRE(cop::ProtocolErrc::invalid_message_id == tll.receive(it, end));
        }
    }
}
