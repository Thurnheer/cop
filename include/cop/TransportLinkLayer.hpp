#ifndef COP_TRANSPORT_LAYER_HPP
#define COP_TRANSPORT_LAYER_HPP

#include "cop/Error.hpp"
#include "cop/Field.hpp"
#include "cop/detail/HandlerWrapper.hpp"
#include "cop/Header.hpp"

//-----------------------------------------------------------------------------
//
// Transport link layer
// Important notice:
//  - When UsingStaticMemory is selected the message will be allocated within
//    IdLayer. As a concequence it is not possible to receive new messages as 
//    long as the message is not destroied.
//  - This does not apply when using dynamic memory (the heap)
//
//-----------------------------------------------------------------------------

namespace cop {

template<class Handler, class WriteIt, class AllMessages, bool UsingStaticMemory = false>
class TransportLinkLayer {

detail::HandlerWrapper<Handler, WriteIt, AllMessages, UsingStaticMemory> handler_;

public:
    TransportLinkLayer(Handler& handler) : handler_(handler){}

    ProtocolErrc receive(WriteIt& it, WriteIt& end) {
        Header header;
        auto ret = header.receive(it, end);
        if(ProtocolErrc::success == ret) {
            Field id;
            ret = id.receive(it, end);
            if(ProtocolErrc::success == ret) {
                if(header.command()) {
                    return handler_.handleCommand(id.value(), it, end);
                }
                else if(header.event()) {
                    return handler_.handleEvent(id.value(), it, end);
                }
                else {
                    return ProtocolErrc::invalid_message_type;
                }
            }
        }
        return ret;
    }
};
}


#endif // COP_TRANSPORT_LAYER_HPP

