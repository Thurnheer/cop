#ifndef COP_BINARYCODER_HPP
#define COP_BINARYCODER_HPP

#include "expected.hpp"

namespace cop {

    template<class WriteIt>
    class BinaryReceiveCoder {
    public:
        explicit BinaryReceiveCoder(WriteIt& it, WriteIt& end) : it_(it), end_(end){}
        template<typename T>
        tl::expected<BinaryReceiveCoder<WriteIt>, ProtocolErrc> operator | (T& t) {
            auto r = serialize(reinterpret_cast<uint8_t*>(&t), sizeof(T));
            if(r == ProtocolErrc::success) {
                return *this;
            }
            return tl::unexpected(r);
        }

    private:
        template<typename T>
        friend
        tl::expected<BinaryReceiveCoder<WriteIt>, ProtocolErrc> operator |
        (tl::expected<BinaryReceiveCoder<WriteIt>, ProtocolErrc> e, T& t){
            if(e) {
                auto r = e.value().serialize(reinterpret_cast<uint8_t*>(&t), sizeof(T));
                if(r == ProtocolErrc::success) {
                    return e;
                }
                return tl::unexpected(r);
            }
            return e;
        }

        ProtocolErrc serialize(uint8_t* data, size_t size) {
            for(int i = 0; i < size; ++i) {
                if(it_ == end_) {
                    return ProtocolErrc::not_enough_space_in_buffer;
                }
                *data = *it_;
                ++it_;
                ++data;
            }
            return ProtocolErrc::success;
        }
        std::reference_wrapper<WriteIt> it_;
        std::reference_wrapper<WriteIt> end_;

    };

    template<class ReadIt>
    class BinarySendCoder {
    public:
        explicit BinarySendCoder(ReadIt& it, ReadIt& end) : it_(it), end_(end){}
        template<typename T>
        void operator | (const T& t) const {
            serialize(reinterpret_cast<uint8_t const*>(&t), sizeof(T));
        }

    private:
        void serialize(uint8_t const* data, size_t size) const {
            for(int i = 0; i < size; ++i) {
                *it_ = *data;
                ++it_;
                ++data;
            }
        }
        ReadIt& it_;
        ReadIt& end_;

    };
}

#endif // COP_BINARYCODER_HPP

