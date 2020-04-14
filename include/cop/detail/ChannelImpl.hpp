#ifndef COP_DETAIL_CHANNELIMPL_HPP
#define COP_DETAIL_CHANNELIMPL_HPP

#include "../Error.hpp"
#include "../Event.hpp"
#include "../BinaryCoder.hpp"
#include <type_traits>

namespace cop::detail {

    template<class ReadIt>
    class ChannelImpl {
    private:

    public:
        template<class Event>
        cop::ProtocolErrc sendEvent(Event& event, ReadIt it, ReadIt end) const {
            static_assert(std::is_same_v<typename Event::type, EventT>, "event has to be of type EventT");
            event.parse(BinarySendCoder(it, end));
            return cop::ProtocolErrc::success;
        }

        template<class Event>
        cop::ProtocolErrc sendEvent(Event event, ReadIt it, ReadIt end) const {
            static_assert(std::is_same_v<typename Event::type, EventT>, "event has to be of type EventT");
            event.parse(BinarySendCoder(it, end));
            return cop::ProtocolErrc::success;
        }

        template<class Command>
        cop::ProtocolErrc sendCommand(Command& command, ReadIt it, ReadIt end) {
            static_assert(std::is_same_v<typename Command::type, CommandT>, "command has to be of type CommandT");
            command.parse(BinarySendCoder(it, end));
            return cop::ProtocolErrc::success;
        }

        template<class Command>
        cop::ProtocolErrc sendCommand(Command command, ReadIt it, ReadIt end) {
            static_assert(std::is_same_v<typename Command::type, CommandT>, "command has to be of type CommandT");
            command.parse(BinarySendCoder(it, end));
            return cop::ProtocolErrc::success;
        }
    };

}

#endif // COP_DETAIL_CHANNELIMPL_HPP

