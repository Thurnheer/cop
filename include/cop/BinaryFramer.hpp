#ifndef COP_BINARYFRAMER_HPP
#define COP_BINARYFRAMER_HPP

#include "Error.hpp"
#include <functional>

namespace cop {

template<class Iterator>
class BinaryFramer {
    static constexpr std::byte FRAME_START_END{'A'};
    static constexpr std::byte ESCAPE_CHARACTER{'\\'};

    enum class ReceiveState : uint8_t {
        ready,
        transfering,
        stuffing
    };

    std::reference_wrapper<Iterator> it_;
    std::reference_wrapper<Iterator> end_;
    ReceiveState state_;
public:
    BinaryFramer(Iterator& it, Iterator& end)
    : it_(it)
    , end_(end)
    , state_(ReceiveState::ready)
    {}

    ProtocolErrc receive(std::byte data) {
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
                break;
            }
            if(ESCAPE_CHARACTER == data) {
                state_ = ReceiveState::stuffing;
                break;
            }
            if(it_.get() == end_.get()) {
                state_ = ReceiveState::ready;
                return ProtocolErrc::not_enough_space_in_buffer;
            }
            *it_.get() = data;
            ++it_.get();
            break;
        }
        case ReceiveState::stuffing:
        {
            state_ = ReceiveState::transfering;
            if(it_.get() == end_.get()) {
                state_ = ReceiveState::ready;
                return ProtocolErrc::not_enough_space_in_buffer;
            }
            *it_.get() = data;
            ++it_.get();
            break;
        }

        default:
            return ProtocolErrc::framing_error;

        }
        return ProtocolErrc::success;
    }

    std::optional<std::byte> send() {
        if(it_.get() == end_.get() && state_ != ReceiveState::ready) {
            state_ = ReceiveState::ready;
            return FRAME_START_END;
        }
        if(it_.get() != end_.get()) {
            switch(state_) {
                case ReceiveState::ready:
                {
                    state_ = ReceiveState::transfering;
                    return FRAME_START_END;
                }
                case ReceiveState::transfering:
                {
                    if(FRAME_START_END == *it_.get() || ESCAPE_CHARACTER == *it_.get()) {
                        state_ = ReceiveState::stuffing;
                        return ESCAPE_CHARACTER;
                    }
                    return *it_.get()++;
                }
                case ReceiveState::stuffing:
                {
                    state_ = ReceiveState::transfering;
                    return *it_.get()++;
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

