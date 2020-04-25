#ifndef COP_BINARYFRAMER_HPP
#define COP_BINARYFRAMER_HPP

#include "Error.hpp"
#include <functional>

namespace cop {

template<class WriteIt>
class BinaryReceiveFramer {
    static const std::byte FRAME_START_END{'A'};
    static const std::byte ESCAPE_CHARACTER{'\\'};

    enum class ReceiveState : uint8_t {
        ReadyToReceive,
        Receiving,
        Stuffing
    };

    std::reference_wrapper<WriteIt> it_;
    std::reference_wrapper<WriteIt> end_;
    ReceiveState state_;
public:
    BinaryReceiveFramer(WriteIt& it, WriteIt& end)
    : it_(it)
    , end_(end)
    , state_(ReceiveState::ReadyToReceive)
    {}

    ProtocolErrc receive(std::byte data) {
        switch(state_) {
        case ReceiveState::ReadyToReceive:
        {
            if(FRAME_START_END == data) {
                state_ = ReceiveState::Receiving;
            }
            break;
        }
        case ReceiveState::Receiving:
        {
            if(FRAME_START_END == data) {
                state_ = ReceiveState::ReadyToReceive;
                break;
            }
            if(ESCAPE_CHARACTER == data) {
                state_ = ReceiveState::Stuffing;
                break;
            }
            if(it_.get() == end_.get()) {
                state_ = ReceiveState::ReadyToReceive;
                return ProtocolErrc::not_enough_space_in_buffer;
            }
            *it_.get() = data;
            ++it_.get();
            break;
        }
        case ReceiveState::Stuffing:
        {
            state_ = ReceiveState::Receiving;
            if(it_.get() == end_.get()) {
                state_ = ReceiveState::ReadyToReceive;
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
};

}

#endif // COP_BINARYFRAMER_HPP

