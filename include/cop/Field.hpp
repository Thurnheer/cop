#ifndef COP_FIELD_HPP
#define COP_FIELD_HPP

#include "Error.hpp"

namespace cop {

class Field {
    uint16_t value_;
public:
    Field() noexcept : value_(0)
    {}

    template<class Iterator>
    ProtocolErrc send(Iterator& it, Iterator& end) noexcept {
        if(it == end - 1) {
            return ProtocolErrc::not_enough_space_in_buffer;
        }

        *it++ = std::byte(value_ >> 8);
        *it++ = std::byte(value_ & 0x00FF);
        return ProtocolErrc::success;
    }

    template<class Iterator>
    ProtocolErrc receive(Iterator& it, Iterator& end) noexcept {
        if(it == end) {
            return ProtocolErrc::unexpected_frame_end;
        }
        value_ = std::to_integer<uint16_t>(*it++);
        value_ = static_cast<uint16_t>(value_ << 8);
        value_ = static_cast<uint16_t>(value_ + std::to_integer<uint16_t>(*it++));
        return ProtocolErrc::success;
    }

    uint16_t value() noexcept {
        return value_;
    }

    void value(uint16_t val) noexcept {
        value_ = val;
    }

};


}

#endif // COP_FIELD_HPP

