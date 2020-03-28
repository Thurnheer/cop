#ifndef COP_IDLAYER_HPP
#define COP_IDLAYER_HPP

#include "Error.hpp"
#include <memory>
#include <array>
#include <algorithm>
#include <type_traits>
#include "detail/HandlerWrapper.hpp"

//-----------------------------------------------------------------------------
//
// IdLayer
// Important notice:
//  - When UsingStaticMemory is selected the message will be allocated within
//    IdLayer. As a concequence it is not possible to receive new messages as 
//    long as the message is not destroied.
//  - This does not apply when using dynamic memory (the heap)
//
//-----------------------------------------------------------------------------

namespace cop {

template<class Handler, class AllMessages, bool UsingStaticMemory = false>
class IdLayer {
private:

    detail::HandlerWrapper<Handler, AllMessages, UsingStaticMemory> handler_;
    
    //NextT next_;
public:
    
    template<typename T>
    using NewMsg = std::conditional<UsingStaticMemory,
                                    decltype(detail::InPlaceAllocationPolicy<T, AllMessages>::MessagePtr),
                                    decltype(detail::DynamicMemoryPolicy<T>::MessagePtr)
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

