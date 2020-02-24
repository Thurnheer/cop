#ifndef COP_CHANNEL_HPP
#define COP_CHANNEL_HPP

#include "Event.hpp"
#include "Error.hpp"

namespace COP {
    template<class Handler>
    class Channel {
    public:
        ProtocolErrc sendEvent(Message& e) {
            e.dispatch(handler_);
            return static_cast<ProtocolErrc>(0);
        }

    private:
        template<typename BeginItr, typename EndItr>
        ProtocolErrc read(BeginItr& beginItr, EndItr& endItr) {

            return static_cast<ProtocolErrc>(0);
        }
        Handler handler_;
    };
}

#endif // COP_CHANNEL_HPP

