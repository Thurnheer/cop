#ifndef COP_BINARYFRAMER_HPP
#define COP_BINARYFRAMER_HPP

#include "Framer.hpp"
#include "Error.hpp"
#include <functional>

namespace cop {

class BinaryFramer {
    enum class ReceiveState : uint8_t {
        ready,
        transfering,
        stuffing
    };

    ReceiveState state_;
public:
    static constexpr std::byte FRAME_START_END{'A'};
    static constexpr std::byte ESCAPE_CHARACTER{'\\'};

    using type = FramerT;
    BinaryFramer() noexcept
    : state_(ReceiveState::ready)
    {}

    template<class Iterator>
    ProtocolErrc receive(std::byte data, Iterator& it, Iterator& end) noexcept {
        switch(state_) {
        case ReceiveState::ready:
        {
            if(FRAME_START_END == data) {
                state_ = ReceiveState::transfering;
            }
            break;
        }
        case ReceiveState::transfering:
        {
            if(FRAME_START_END == data) {
                state_ = ReceiveState::ready;
                return ProtocolErrc::success;
            }
            if(ESCAPE_CHARACTER == data) {
                state_ = ReceiveState::stuffing;
                break;
            }
            if(it == end) {
                state_ = ReceiveState::ready;
                return ProtocolErrc::not_enough_space_in_buffer;
            }
            *it = data;
            ++it;
            break;
        }
        case ReceiveState::stuffing:
        {
            state_ = ReceiveState::transfering;
            if(it == end) {
                state_ = ReceiveState::ready;
                return ProtocolErrc::not_enough_space_in_buffer;
            }
            *it = data;
            ++it;
            break;
        }

        default:
            return ProtocolErrc::framing_error;

        }
        return ProtocolErrc::receiving;
    }

    template<class Iterator>
    std::optional<std::byte> send(Iterator& it, Iterator& end) noexcept {
        if(it == end && state_ != ReceiveState::ready) {
            state_ = ReceiveState::ready;
            return FRAME_START_END;
        }
        if(it != end) {
            switch(state_) {
                case ReceiveState::ready:
                {
                    state_ = ReceiveState::transfering;
                    return FRAME_START_END;
                }
                case ReceiveState::transfering:
                {
                    if(FRAME_START_END == *it || ESCAPE_CHARACTER == *it) {
                        state_ = ReceiveState::stuffing;
                        return ESCAPE_CHARACTER;
                    }
                    return *it++;
                }
                case ReceiveState::stuffing:
                {
                    state_ = ReceiveState::transfering;
                    return *it++;
                }
                default:
                {
                    return std::optional<std::byte>();
                }
            }
        }
        return std::optional<std::byte>();
    }
};

}

#endif // COP_BINARYFRAMER_HPP

