#ifndef COP_DETAIL_HANDLERWRAPPER_HPP
#define COP_DETAIL_HANDLERWRAPPER_HPP

#include "../Error.hpp"
#include "DynamicMemoryPolicy.hpp"
#include "InPlaceAllocationPolicy.hpp"
#include "../Id.hpp"
#include "../BinaryCoder.hpp"

namespace cop::detail {
    
    template<class HandlerT, class WriteIt, class AllMessagesT, bool UsingStaticMemory>
    class HandlerWrapper;
    
    template<class HandlerT, class WriteIt, class ...Types, bool UsingStaticMemory>
    class HandlerWrapper<HandlerT, WriteIt, std::tuple<Types...>, UsingStaticMemory>
    {
    private:
        HandlerT handler_;
        template<typename T>
        using alloc_type = std::conditional<UsingStaticMemory,
                                            InPlaceAllocationPolicy<T, std::tuple<Types...> >, 
                                            DynamicMemoryPolicy<T>
                                        >::type;
        
        template<class First, class... Rest>
        cop::ProtocolErrc handle_impl(ID_t id, WriteIt& it, WriteIt& end) {
            if(id == First::ID) {
                alloc_type<First> factory;
                auto Msg = factory.allocateMessage();
                auto er = Msg->parse(BinaryReceiveCoder(it, end));
                if(er) {
                    handler_.handle(*Msg.get());
                    return ProtocolErrc::success;
                }
                return er.error();
            }
            if constexpr (sizeof...(Rest) > 0) {
                return handle_impl<Rest...>(id);
            }
            return ProtocolErrc::invalid_message_id;
        }

    public:
        //TODO how to pass iterator as reference?
        cop::ProtocolErrc handle(ID_t id, WriteIt it, WriteIt end) {
            return handle_impl<Types...>(id, it, end);
        }
    };

}

#endif // COP_DETAIL_HANDLERWRAPPER_HPP

