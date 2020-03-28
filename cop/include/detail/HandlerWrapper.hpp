#ifndef COP_DETAIL_HANDLERWRAPPER_HPP
#define COP_DETAIL_HANDLERWRAPPER_HPP

#include "../Error.hpp"
#include "DynamicMemoryPolicy.hpp"
#include "InPlaceAllocationPolicy.hpp"
#include "../Event.hpp"

namespace cop::detail {
    
    template<class HandlerT, class AllMessagesT, bool UsingStaticMemory>
    class HandlerWrapper;
    
    template<class HandlerT, class ...Types, bool UsingStaticMemory>
    class HandlerWrapper<HandlerT, std::tuple<Types...>, UsingStaticMemory>
    {
    private:
        HandlerT handler_;
        template<typename T>
        using alloc_type = std::conditional<UsingStaticMemory,
                                            InPlaceAllocationPolicy<T, std::tuple<Types...> >, 
                                            DynamicMemoryPolicy<T>
                                        >::type;
        
        template<class First, class... Rest>
        cop::ProtocolErrc handle_impl(ID_t id) {
            if(id == First::ID) {
                alloc_type<First> factory;
                handler_.handle(*factory.allocateMessage().get());
                return ProtocolErrc::success;
            }
            if constexpr (sizeof...(Rest) > 0) {
                return handle_impl<Rest...>(id);
            }
            return ProtocolErrc::invalidMessageId;
        }

    public:
        cop::ProtocolErrc handle(ID_t id) {
            return handle_impl<Types...>(id);
        }
    };

}

#endif // COP_DETAIL_HANDLERWRAPPER_HPP

