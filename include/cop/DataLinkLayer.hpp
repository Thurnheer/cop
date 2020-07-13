#ifndef COP_DATALINKLAYER_HPP
#define COP_DATALINKLAYER_HPP

#include "cop/BinaryFramer.hpp"
#include "cop/Crc.hpp"

namespace cop {

template<class Iterator>
class DataLinkLayer {
    Iterator startIterator_;
    Iterator endIterator_;
    Iterator it_;
    Iterator end_;
    BinaryFramer framer_;
    Crc crc_;

public:
    DataLinkLayer(Iterator it, Iterator end) : startIterator_(it), endIterator_(end), it_(it), end_(end), framer_(), crc_() {

    }

    ProtocolErrc receive(std::byte data) {
        if(ProtocolErrc::success == framer_.receive(std::forward<std::byte>(data), it_, end_))
        {
            return crc_.receive(startIterator_, endIterator_);
        }
        return ProtocolErrc::receiving;
    }
};

}

#endif // COP_DATALINKLAYER_HPP

