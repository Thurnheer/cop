#ifndef COP_HEADER_HPP
#define COP_HEADER_HPP

#include "Error.hpp"
#include <bitset>
#include <functional>

namespace cop {

template<class Iterator>
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
    std::reference_wrapper<Iterator> it_;
    std::reference_wrapper<Iterator> end_;
    std::bitset<8> header_;

public:
    Header(Iterator& it, Iterator& end) : it_(it), end_(end), header_(0)
    { }

    ProtocolErrc receive() {
        header_ = std::to_integer<unsigned char>(*it_.get()++);
        return ProtocolErrc::success;
    }

    ProtocolErrc send() {
        if(it_.get() == end_.get()) {
            return ProtocolErrc::not_enough_space_in_buffer;
        }
        *it_.get()++ = std::byte(header_.to_ulong());
        return ProtocolErrc::success;
    }

    bool adressed() const {
        return header_[eAdressed];
    }

    void adressed(bool val) {
        header_[eAdressed] = val;
    }

    bool event() const {
        return header_[eEvent] && !header_[eCommand];
    }

    void event(bool val) {
        header_[eEvent] = val;
        header_[eCommand] = !val;
    }

    bool command() const {
        return header_[eCommand] && !header_[eEvent];
    }

    void command(bool val) {
        header_[eEvent] = !val;
        header_[eCommand] = val;
    }

    bool reply() const {
        return !header_[eCommand] && !header_[eEvent];
    }

    void reply(bool val) {
        header_[eEvent] = !val;
        header_[eCommand] = !val;
    }

};

}

#endif // COP_HEADER_HPP

