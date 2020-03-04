#ifndef COP_IDLAYER_HPP
#define COP_IDLAYER_HPP

#include "Error.hpp"
#include "Utilities.hpp"
#include <memory>
#include <array>
#include <algorithm>
#include <type_traits>

namespace COP {

template<class Handler, class AllMessages>
class IdLayer {
public:
    
    using NewMsg = std::unique_ptr<Message>;

    ProtocolErrc read( ID_t id) {
        /*Field field;
        if(auto e = field.read()) {
            return e;
        }*/
        return handler_.handle(id);
        
        /*msg = createMessage(id);

        if(!msg) {
            return ProtocolErrc::invalidMessageId;
        }

        if(auto e = next_.read(iter, end)) {
            msg.reset();
            return e;
        }*/
        //return ProtocolErrc::success;
    }

    /*ProtocolErrc write(const Message& msg, WriteIterator& iter, EndIterator& end) const
    {
        Field field;
        field.value = msg.id();
        if(auto e = field.write(iter, end)) {
            return e;
        }
        return next_.write(msg, iter, end);
    }*/

private:

    template<class Message>
    class Factory {
    private:
        template<typename Message>
        struct DynamicMemoryPolicy {
            using MessagePtr = std::unique_ptr<Message>;

            template<typename MessageT>
            MessagePtr allocateMessage() {
                return std::make_unique(MessageT);
            }
        };
        template<typename MessageT, class AllMessagesT>
        class InPlaceAllocationPolicy {
        private:
            using InPlaceStorage = typename TupleAsAlignedUnion<AllMessagesT>::type;
            InPlaceStorage storage_;
        public:
            template<typename MessageT>
            struct InPlaceDeleter {

            };
            using MessagePtr = std::unique_ptr<MessageT, InPlaceDeleter<MessageT> >;

            template<typename MessageT>
            MessagePtr allocateMessage() {
                new (&storage_) MessageT();
                return MessagePtr( reinterpret_cast<MessageT*>(&storage_),
                        InPlaceDeleter<MessageT>());
            }
        };
        using ptr_type = std::conditional<UsingStaticMemory,
                                          InPlaceAllocationPolicy<Message, AllMessages>, 
                                          DynamicMemoryPolicy<Message> >::type;
    public:
        std::unique_ptr<Message> create() const { return std::make_unique<Message>(); }
    };
    
    template<class HandlerT, class AllMessagesT>
    class HandlerWrapper;
    
    template<class HandlerT, class ...Types>
    class HandlerWrapper<HandlerT, std::tuple<Types...> >
    {
    private:
        HandlerT handler_;
        
        template<class First, class... Rest>
        ProtocolErrc handle_impl(ID_t id) {
            if(id == First::ID) {
                Factory<First> first;
                handler_.handle(*first.create().get());
                return ProtocolErrc::success;
            }
            if constexpr (sizeof...(Rest) > 0) {
                return handle_impl<Rest...>(id);
            }
            return ProtocolErrc::invalidMessageId;
        }

    public:
        ProtocolErrc handle(ID_t id) {
            return handle_impl<Types...>(id);
        }
    };
    
    HandlerWrapper<Handler, AllMessages> handler_;
    
    //NextT next_;
};

}

#endif // COP_IDLAYER_HPP

