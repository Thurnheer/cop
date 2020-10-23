#include <catch2/catch.hpp>
#include <trompeloeil.hpp>
#include "cop/TransportLinkLayer.hpp"
#include "cop/DataLinkLayer.hpp"
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


struct Adapter {
    Adapter() : buffer(){}
    std::vector<std::byte> buffer;
    void send(std::byte& t) noexcept {
        buffer.push_back(t);
    }
};


struct Channel {
    Channel(HandlerMock& handler) : buffer_(1023, std::byte(0)), adapter_(), tll_(handler) {}
    std::vector<std::byte> buffer_;

    using ReadIt = std::vector<std::byte>::iterator;
    Adapter adapter_;

    cop::TransportLinkLayer<HandlerMock, ReadIt, std::tuple<mySecondEvent> > tll_;

    void sendEvent(mySecondEvent& event) {
        auto it = buffer_.begin(); auto end = buffer_.end();
        tll_.sendEvent(event, it, end);
        cop::DataLinkLayer<ReadIt> dll_(it, end);
        dll_.send(adapter_);
    }

    void receive() {
        auto it = buffer_.begin(); auto end = buffer_.end();
        cop::DataLinkLayer<ReadIt> dll(it, end);
        for(auto i = adapter_.buffer.begin(); i != adapter_.buffer.end(); ++i) {
            if(cop::ProtocolErrc::success == dll.receive(*i)) {
                break;
            }
        }
        tll_.receive(it, end);
    }

};


SCENARIO(" Events can be sent and received") {

    GIVEN(" a channel and an event") {
        HandlerMock handler;
        Channel channel(handler);
        mySecondEvent event;

        using trompeloeil::_;

        THEN("the data can be sent and received") {
            REQUIRE_CALL(handler, handle(_));
            channel.sendEvent(event);
            channel.receive();
        }
    }
}
