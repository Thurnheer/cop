#include <catch2/catch.hpp>
#include <trompeloeil.hpp>
#include "cop/detail/HandlerWrapper.hpp"
#include "cop/Event.hpp"
#include <array>

static const int TEST_INT = 41;
static const int TEST_DATA = 42;
static const double TEST_DOUBLE = 2.9;

enum events {
    eMyFirstEvent = 1,
    eMySecondEvent
};

struct myFirstEvent : cop::Event<eMyFirstEvent> {
    template<class Coder>
    auto parse(Coder coder) { // cppcheck-suppress functionConst
        return coder | data;
    }
private:
    int data = TEST_INT;
};

struct mySecondEvent : cop::Event<eMySecondEvent> {
    mySecondEvent() = default;
    mySecondEvent(int da, double dd) : data(da), d(dd){}

    [[nodiscard]] auto getData() const { return data; }
    [[nodiscard]] auto getD() const { return d; }

    template<class Coder>
    auto parse(Coder coder) { // cppcheck-suppress functionConst
         return coder | data | d;
    }
private:
    int data = TEST_DATA;
    double d = TEST_DOUBLE;
};

using AllMessages = std::tuple<myFirstEvent, mySecondEvent>;

SCENARIO( "The HandlerWrapper generates Events", "[generate Events]" ) {

    GIVEN( "An event containing an id" ) {
        
        struct Handler
        {
            static void handle(mySecondEvent& e) { // cppcheck-suppress constParameter
                REQUIRE(e.getData() == 1);
                REQUIRE(e.getD() == 0.0);
            }
        };
        
        WHEN("the id layer reads an id") {
            static const int BUFFERSIZE = 1023;
            std::array<std::byte, BUFFERSIZE> buf
                {std::byte{1}, std::byte{0}, std::byte{0}, std::byte{0},
                 std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}};
            using ReadIt = std::array<std::byte, BUFFERSIZE>::const_iterator;
            Handler handler;
            cop::detail::HandlerWrapper<Handler, ReadIt, std::tuple<mySecondEvent>, true>
            dut(handler);
        

            const auto error = dut.handleEvent(eMySecondEvent, buf.begin(), buf.end());

            THEN("a message is created") {
                REQUIRE(cop::ProtocolErrc::success ==  error );
            }
        
        }

        
    }
    

}
