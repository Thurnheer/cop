#ifndef COP_CHANNEL_HPP
#define COP_CHANNEL_HPP

#include <cop/Id.hpp>
#include <cop/Error.hpp>
#include <cop/TransportLinkLayer.hpp>
#include <cop/DataLinkLayer.hpp>
#include <array>
#include <vector>
#include <tuple>
#include <type_traits>

namespace cop {

template<bool UsingDynamicMemory, class Enable = void>
struct Container
{
    using type = std::array<std::byte, 1000>;
};

template<bool UsingDynamicMemory>
struct Container<UsingDynamicMemory, typename std::enable_if<UsingDynamicMemory> >
{
    using type = std::vector<std::byte>;
};

template<class Handler, class Adapter, class DataT, bool UsingDynamicMemory = true>
class Channel{
public:
    Channel(Handler& handler, Adapter& adapter)
    : buffer_()
    , adapter_(adapter)
    , tll_(handler)
    {}

    template<class Message>
    [[nodiscard]] ProtocolErrc sendEvent(Message& e) {
        auto it = buffer_.begin(); auto end = buffer_.end();
        auto errorCode = tll_.sendEvent(e, it, end);
        if(ProtocolErrc::success == errorCode) {
            DataLinkLayer dll(it, end);
            return dll.send(adapter_);
        }
        return errorCode;
    }

    [[nodiscard]] ProtocolErrc receive() {
        auto it = buffer_.begin(); auto end = buffer_.end();
        cop::DataLinkLayer<ReadIt> dll(it, end);
        for(auto i = adapter_.buffer.begin(); i != adapter_.buffer.end(); ++i) {
            if(cop::ProtocolErrc::success == dll.receive(*i)) {
                break;
            }
        }
        return tll_.receive(it, end);
    }

private:
    Container<UsingDynamicMemory>::type buffer_;
    Adapter adapter_;
    using ReadIt = Container<UsingDynamicMemory>::type::iterator;
    TransportLinkLayer<Handler, ReadIt, DataT, UsingDynamicMemory > tll_;
};

}

#endif // COP_CHANNEL_HPP

