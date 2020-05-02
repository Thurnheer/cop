#ifndef COP_FIELD_HPP
#define COP_FIELD_HPP

#include "Error.hpp"

namespace cop {

template<class Iterator>
class Field {
    std::reference_wrapper<Iterator> it_;
    std::reference_wrapper<Iterator> end_;
    uint16_t value_;
public:
    Field(Iterator& it, Iterator& end) noexcept : it_(it), end_(end), value_(0)
    {}

    ProtocolErrc send() noexcept {
        if(it_.get() == end_.get()) {
            return ProtocolErrc::not_enough_space_in_buffer;
        }

        *it_.get()++ = std::byte(value_ >> 8);
        *it_.get()++ = std::byte(value_ & 0x00FF);
        return ProtocolErrc::success;
    }

    ProtocolErrc receive() noexcept {
        value_ = std::to_integer<uint16_t>(*it_.get()++);
        value_ = static_cast<uint16_t>(value_ << 8);
        value_ = static_cast<uint16_t>(value_ + std::to_integer<uint16_t>(*it_.get()++));
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

