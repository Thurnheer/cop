#ifndef COP_IDLAYER_HPP
#define COP_IDLAYER_HPP

#include "Error.hpp"
#include "Utilities.hpp"
#include <memory>
#include <array>
#include <algorithm>
#include <type_traits>

namespace COP {

template<class Handler, class AllMessages, bool UsingStaticMemory = false>
class IdLayer {
private:

    template<typename Message>
    struct DynamicMemoryPolicy {
        using MessagePtr = std::unique_ptr<Message>;

        MessagePtr allocateMessage() {
            return std::make_unique<Message>();
        }
    };

    template<typename Message, class AllMessagesT>
    class InPlaceAllocationPolicy {
    private:
        using InPlaceStorage = typename TupleAsAlignedUnion<AllMessages>::type;
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
    template<typename T>
    using alloc_type = std::conditional<UsingStaticMemory,
                                        InPlaceAllocationPolicy<T, AllMessages>, 
                                        DynamicMemoryPolicy<T>
                                     >::type;
    
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
        ProtocolErrc handle(ID_t id) {
            return handle_impl<Types...>(id);
        }
    };
    
    HandlerWrapper<Handler, AllMessages> handler_;
    
    //NextT next_;
public:
    
    template<typename T>
    using NewMsg = std::conditional<UsingStaticMemory,
                                    decltype(InPlaceAllocationPolicy<T, AllMessages>::MessagePtr),
                                    decltype(DynamicMemoryPolicy<T>::MessagePtr)
                                   >::type;

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

};

}

#endif // COP_IDLAYER_HPP

