#ifndef COP_HEADER_HPP
#define COP_HEADER_HPP

#include "Error.hpp"
#include <bitset>
#include <functional>

namespace cop {

class Header {
    enum headerbits {
        eAdressed = 0,
        eEvent = 1,
        eCommand = 2,
        eFragmented = 3,
        eReserved1 = 4,
        eReserved2 = 5,
        eReserved3 = 6,
        eAditionalHeader =  7
    };
    std::bitset<8> header_;

public:
    Header() noexcept : header_(0)
    { }

    template<class Iterator>
    ProtocolErrc receive(Iterator& it, Iterator& end) noexcept {
        if(it == end) {
            return ProtocolErrc::not_enough_space_in_buffer;
        }
        header_ = std::to_integer<unsigned char>(*it++);
        return ProtocolErrc::success;
    }

    template<class Iterator>
    ProtocolErrc send(Iterator& it, Iterator& end) noexcept {
        if(it == end) {
            return ProtocolErrc::not_enough_space_in_buffer;
        }
        *it++ = std::byte(header_.to_ulong());
        return ProtocolErrc::success;
    }

    bool adressed() const noexcept {
        return header_[eAdressed];
    }

    void adressed(bool val) noexcept {
        header_[eAdressed] = val;
    }

    bool event() const noexcept {
        return header_[eEvent] && !header_[eCommand];
    }

    void event(bool val) noexcept {
        header_[eEvent] = val;
        header_[eCommand] = !val;
    }

    bool command() const noexcept {
        return header_[eCommand] && !header_[eEvent];
    }

    void command(bool val) noexcept {
        header_[eEvent] = !val;
        header_[eCommand] = val;
    }

    bool reply() const noexcept {
        return !header_[eCommand] && !header_[eEvent];
    }

    void reply(bool val) noexcept {
        header_[eEvent] = !val;
        header_[eCommand] = !val;
    }

};

}

#endif // COP_HEADER_HPP

