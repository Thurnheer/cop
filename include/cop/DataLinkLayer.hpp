#ifndef COP_DATALINKLAYER_HPP
#define COP_DATALINKLAYER_HPP

#include "cop/BinaryFramer.hpp"
#include "cop/Crc.hpp"
#include "cop/detail/Empty.hpp"
#include <cstddef>
#include <concepts>

namespace cop {

template<class T>
concept Sendable = requires(T t, std::byte data)
{
    { t.send(data) } noexcept;
};

template<class Iterator, class Next = detail::Empty>
class DataLinkLayer {
    Iterator startIterator_;
    Iterator& endIterator_;
    Iterator it_;
    Iterator end_;
    BinaryFramer framer_;
    Crc crc_;
    Next next_;

public:
    DataLinkLayer(Iterator it, Iterator& end) : startIterator_(it), endIterator_(end), it_(it),
    end_(end), framer_(), crc_(), next_()
    {

    }

    [[nodiscard]] ProtocolErrc receive(std::byte data) {
        const auto result = framer_.receive(std::forward<std::byte>(data), it_, end_);
        if(ProtocolErrc::success == result)
        {
            if constexpr (std::is_same_v<Next, detail::Empty>)
                return crc_.receive(startIterator_, endIterator_);
            else {
                if(ProtocolErrc::success == crc_.receive(startIterator_, endIterator_)) {
                    return next_.receive(it_, end_);
                }
            }
        }
        return result;
    }

    template<class FrameAdapter>
    [[nodiscard]] ProtocolErrc send(FrameAdapter& frameAdapter) {
        auto ret = crc_.send(it_, end_);
        if(ProtocolErrc::success == ret) {
            frameAdapter.send(it_, end_);
        }
        return ret;
    }
};

}

#endif // COP_DATALINKLAYER_HPP

