#ifndef COP_CHANNEL_HPP
#define COP_CHANNEL_HPP

#include "Event.hpp"

namespace COP {
    class Channel {
    public:
        void sendEvent(const EventBase& e) {

        }

    private:
        template<typename BeginItr, typename EndItr>
        void read(BeginItr& beginItr, EndItr& endItr) {

        }
    };
}

#endif // COP_CHANNEL_HPP

