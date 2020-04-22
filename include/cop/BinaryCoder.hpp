#ifndef COP_BINARYCODER_HPP
#define COP_BINARYCODER_HPP

#include "expected.hpp"

namespace cop {

    template<class ReadIt>
    class BinaryReceiveCoder {
    public:
        explicit BinaryReceiveCoder(ReadIt& it, ReadIt& end) : it_(it), end_(end){}
        template<typename T>
        tl::expected<BinaryReceiveCoder<ReadIt>, ProtocolErrc> operator | (T& t) {
            static_assert(std::is_fundamental<T>::value, "Parsed values need to be fundamental");
            auto error = deserialize(reinterpret_cast<std::byte*>(&t), sizeof(T));
            if(error == ProtocolErrc::success) {
                return *this;
            }
            return tl::unexpected(error);
        }

    private:
        template<typename T>
        friend
        tl::expected<BinaryReceiveCoder<ReadIt>, ProtocolErrc> operator |
        (tl::expected<BinaryReceiveCoder<ReadIt>, ProtocolErrc> expectedCoder, T& t){
            static_assert(std::is_fundamental<T>::value, "Parsed values need to be fundamental");
            if(expectedCoder) {
                auto error = expectedCoder.value().deserialize(reinterpret_cast<std::byte*>(&t), sizeof(T));
                if(error == ProtocolErrc::success) {
                    return expectedCoder;
                }
                return tl::unexpected(error);
            }
            return expectedCoder;
        }

        ProtocolErrc deserialize(std::byte* data, size_t size) {
            for(int i = 0; i < size; ++i) {
                if(it_ == end_) {
                    return ProtocolErrc::not_enough_data;
                }
                *data = *it_;
                ++it_;
                ++data;
            }
            return ProtocolErrc::success;
        }
        std::reference_wrapper<ReadIt> it_;
        std::reference_wrapper<ReadIt> end_;

    };

    template<class WriteIt>
    class BinarySendCoder {
    public:
        explicit BinarySendCoder(WriteIt& it, WriteIt& end) : it_(it), end_(end){}
        template<typename T>
        tl::expected<BinarySendCoder<WriteIt>, ProtocolErrc> operator | (const T t) const {
            static_assert(std::is_fundamental<T>::value, "Parsed values need to be fundamental");
            auto r = serialize(reinterpret_cast<std::byte const*>(&t), sizeof(T));
            if(r == ProtocolErrc::success) {
                return *this;
            }
            return tl::unexpected(r);
        }

    private:
        template<typename T>
        friend
        tl::expected<BinarySendCoder<WriteIt>, ProtocolErrc> operator |
        (tl::expected<BinarySendCoder<WriteIt>, ProtocolErrc> e, T t){
            static_assert(std::is_fundamental<T>::value, "Parsed values need to be fundamental");
            if(e) {
                auto r = e.value().serialize(reinterpret_cast<std::byte*>(&t), sizeof(T));
                if(r == ProtocolErrc::success) {
                    return e;
                }
                return tl::unexpected(r);
            }
            return e;
        }

        ProtocolErrc serialize(std::byte const* data, size_t size) const {
            for(int i = 0; i < size; ++i) {
                if(it_ == end_) {
                    return ProtocolErrc::not_enough_space_in_buffer;
                }
                *it_ = *data;
                ++it_;
                ++data;
            }
            return ProtocolErrc::success;
        }
        std::reference_wrapper<WriteIt> it_;
        std::reference_wrapper<WriteIt> end_;

    };
}

#endif // COP_BINARYCODER_HPP

