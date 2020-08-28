#ifndef COP_DETAIL_CHANNELIMPL_HPP
#define COP_DETAIL_CHANNELIMPL_HPP

#include "../Error.hpp"
#include "../Event.hpp"
#include "../Command.hpp"
#include "../BinaryCoder.hpp"
#include <type_traits>

namespace cop::detail {

    template<class ReadIt>
    class ChannelImpl {
    private:
    // TODO react on endianness with std::endian

    public:
        template<class Event>
        cop::ProtocolErrc sendEvent(Event event, ReadIt it, ReadIt end) const noexcept {
            static_assert(std::is_same_v<typename Event::type, EventT>, "event has to be of type EventT");
            auto r = event.parse(BinarySendCoder(it, end));
            if(r) {
                return ProtocolErrc::success;
            }
            return r.error();
        }

        template<class Command>
        cop::ProtocolErrc sendCommand(Command command, ReadIt it, ReadIt end) noexcept {
            static_assert(std::is_same_v<typename Command::type, CommandT>, "command has to be of type CommandT");
            auto r = command.parse(BinarySendCoder(it, end));
            if(r) {
                return ProtocolErrc::success;
            }
            return r.error();
        }
    };

}

#endif // COP_DETAIL_CHANNELIMPL_HPP

