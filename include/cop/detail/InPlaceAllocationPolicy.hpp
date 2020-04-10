#ifndef COP_DETAIL_INPLACEALLOCATIONPOLICY_HPP
#define COP_DETAIL_INPLACEALLOCATIONPOLICY_HPP

#include "Utilities.hpp"

namespace cop::detail {

    template<typename Message, class AllMessagesT>
    class InPlaceAllocationPolicy {
    private:
        using InPlaceStorage = typename TupleAsAlignedUnion<AllMessagesT>::type;
        InPlaceStorage storage_;
    public:
        template<typename T>
        struct InPlaceDeleter {
            void operator()(T* obj) {
                obj->~T();
            }
        };
        using MessagePtr = std::unique_ptr<Message, InPlaceDeleter<Message> >;

        MessagePtr allocateMessage() {
            new (&storage_) Message();
            return MessagePtr( reinterpret_cast<Message*>(&storage_),
                    InPlaceDeleter<Message>());
        }
    };
    
}

#endif // COP_DETAIL_INPLACEALLOCATIONPOLICY_HPP

