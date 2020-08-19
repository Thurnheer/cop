#ifndef COP_DETAIL_HANDLERWRAPPER_HPP
#define COP_DETAIL_HANDLERWRAPPER_HPP

#include "../Error.hpp"
#include "DynamicMemoryPolicy.hpp"
#include "InPlaceAllocationPolicy.hpp"
#include "../Id.hpp"
#include "../BinaryCoder.hpp"
#include "Utilities.hpp"

namespace cop::detail {
    
    template<class HandlerT, class WriteIt, class AllMessagesT, bool UsingStaticMemory>
    class HandlerWrapper;
    
    template<class HandlerT, class WriteIt, class ...Types, bool UsingStaticMemory>
    class HandlerWrapper<HandlerT, WriteIt, std::tuple<Types...>, UsingStaticMemory>
    {
    private:
        template<typename T>
        using EventFilter = std::is_same<cop::EventT, T>;
        using AllEvents = filter<EventFilter, std::tuple<Types...> >::type;
        template<typename T>
        using CommandFilter = std::is_same<cop::CommandT, T>;
        using AllCommands = filter<CommandFilter, std::tuple<Types...> >::type;
        template<typename T>
        using alloc_type = std::conditional<UsingStaticMemory,
                                            InPlaceAllocationPolicy<T, std::tuple<Types...> >, 
                                            DynamicMemoryPolicy<T>
                                        >::type;
        
        template<class AllTypes, typename T = void>
        struct HandleInvoker;
        template<class AllTypes> // For empty tuples
        struct HandleInvoker<AllTypes, typename std::enable_if<std::is_same_v<std::tuple<>,
        AllTypes> >::type > {
            cop::ProtocolErrc handle(ID_t, WriteIt&, WriteIt&) {
                return ProtocolErrc::invalid_message_type;
            }
        };
        template<class FirstType, class ...AllTypes>
        struct HandleInvoker<std::tuple<FirstType, AllTypes...> > {
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
            cop::ProtocolErrc handle(ID_t id, WriteIt& it, WriteIt& end) {
                return handle_impl<FirstType, AllTypes...>(id, it, end);
            }
        private:
            HandlerT handler_;
        };

        HandleInvoker<AllEvents> eventHandler_;
        HandleInvoker<AllCommands> commandHandler_;

    public:
        //TODO how to pass iterator as reference?
        cop::ProtocolErrc handleEvent(ID_t id, WriteIt it, WriteIt end) {
            return eventHandler_.handle(id, it, end);
        }
        cop::ProtocolErrc handleCommand(ID_t id, WriteIt it, WriteIt end) {
            return commandHandler_.handle(id, it, end);
        }
    };

}

#endif // COP_DETAIL_HANDLERWRAPPER_HPP

