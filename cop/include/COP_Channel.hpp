#ifndef COP_CHANNEL_HPP
#define COP_CHANNEL_HPP

#include "COP_Event.hpp"
#include <functional>

namespace COP {
    class Channel {
    public:
        void sendEvent(const Event& e) {

        }

        template<typename T>
        void registerEvent(T& t) {
            t = 42;
        }

    private:
        template<typename BeginItr, typename EndItr>
        void read(BeginItr& beginItr, EndItr& endItr) {

        }
    };
}

#endif // COP_CHANNEL_HPP

