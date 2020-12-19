#include <catch2/catch.hpp>
#include <trompeloeil.hpp>
#include <cop/Channel.hpp>
#pragma warning (disable : 4619)
#pragma warning (disable : 4242)
//#include <boost/asio.hpp>
#pragma warning (default : 4619)
#pragma warning (default : 4242)

/*std::string read(boost::asio::ip::tcp::socket& socket) {
    boost::asio::streambuf buf;
    boost::asio::read_until( socket, buf, "\n" );
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}*/

static const int TEST_INT = 41;
static const int TEST_DATA = 42;
static const double TEST_DOUBLE = 2.9;


enum events {
    eMyFirstEvent = 1,
    eMySecondEvent
};

struct myFirstEvent : cop::Event<eMyFirstEvent> {
    int data = TEST_INT;
    template<class Coder>
    auto parse(Coder coder) { // cppcheck-suppress functionConst
        return coder | data;
    }
};

struct mySecondEvent : cop::Event<eMySecondEvent> {
    mySecondEvent() = default;
    mySecondEvent(int da, double dd) : data(da), d(dd){}

    template<class Coder>
    auto parse(Coder coder) { // cppcheck-suppress functionConst
         return coder | data | d;
    }
    bool operator==(const mySecondEvent& s) const {
        return data == s.data && d == s.d;
    }
private:
    int data = TEST_DATA;
    double d = TEST_DOUBLE;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
struct HandlerMock
{
    MAKE_MOCK1(  handle, void(mySecondEvent& e));
};
#pragma GCC diagnostic pop


struct Adapter {
    template<class Itr>
    void send(Itr begin, Itr end) noexcept {
        buffer.insert(buffer.end(), begin, end);
    }
    std::vector<std::byte> buffer{};
};

using EventsT = std::tuple<mySecondEvent>;

SCENARIO(" Events can be sent and received") {

    GIVEN(" a channel and an event") {
        Adapter adapter;
        HandlerMock handler;
        cop::Channel<HandlerMock, Adapter, EventsT, false> channel(handler, adapter);
        mySecondEvent event;

        using trompeloeil::_;

        THEN("the data can be sent and received") {
            REQUIRE_CALL(handler, handle(_));
            REQUIRE(cop::ProtocolErrc::success == channel.sendEvent(event));
            REQUIRE(cop::ProtocolErrc::success == channel.receive());
        }
    }
}
