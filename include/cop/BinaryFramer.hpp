#ifndef COP_BINARYFRAMER_HPP
#define COP_BINARYFRAMER_HPP

#include "Framer.hpp"
#include "Error.hpp"
#include <functional>
#include <optional>

namespace cop {

class BinaryFramer {
    enum class FrameState : uint8_t {
        ready,
        transfering,
        stuffing
    };

    FrameState state_;
public:
    static constexpr std::byte FRAME_START_END{'A'};
    static constexpr std::byte ESCAPE_CHARACTER{'\\'};

    using type = FramerT;
    BinaryFramer() noexcept
    : state_(FrameState::ready)
    {}

    template<class Iterator>
    ProtocolErrc receive(std::byte data, Iterator& it, Iterator& end) noexcept {
        switch(state_) {
        case FrameState::ready:
        {
            if(FRAME_START_END == data) {
                state_ = FrameState::transfering;
            }
            break;
        }
        case FrameState::transfering:
        {
            if(FRAME_START_END == data) {
                state_ = FrameState::ready;
                return ProtocolErrc::success;
            }
            if(ESCAPE_CHARACTER == data) {
                state_ = FrameState::stuffing;
                break;
            }
            if(it == end) {
                state_ = FrameState::ready;
                return ProtocolErrc::not_enough_space_in_buffer;
            }
            *it = data;
            ++it;
            break;
        }
        case FrameState::stuffing:
        {
            state_ = FrameState::transfering;
            if(it == end) {
                state_ = FrameState::ready;
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
        if(it == end && state_ != FrameState::ready) {
            state_ = FrameState::ready;
            return FRAME_START_END;
        }
        if(it != end) {
            switch(state_) {
                case FrameState::ready:
                {
                    state_ = FrameState::transfering;
                    return FRAME_START_END;
                }
                case FrameState::transfering:
                {
                    if(FRAME_START_END == *it || ESCAPE_CHARACTER == *it) {
                        state_ = FrameState::stuffing;
                        return ESCAPE_CHARACTER;
                    }
                    return *it++;
                }
                case FrameState::stuffing:
                {
                    state_ = FrameState::transfering;
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

