#ifndef COP_CHANNEL_HPP
#define COP_CHANNEL_HPP

#include "COP_Event.hpp"
#include <functional>

namespace COP {
    class Channel {
    public:
        void sendEvent(const Event& e) {

        }

        void registerCallback(std::function<void(COP::ReceivedEvent&)> f) {

        }

    };
}

#endif // COP_CHANNEL_HPP

