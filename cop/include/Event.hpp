#ifndef COP_EVENT_HPP
#define COP_EVENT_HPP

#include "Message.hpp"
#include "Error.hpp"

namespace COP {

    using ID_t = uint32_t;

    template<class derived>
    class EventBase : public Message {
    protected:
    };

    template<ID_t id, class Derived>
    class ReceivedEvent : public EventBase<Derived> {
    public:
        static const ID_t ID = id;
        auto read() {
            return 42;
        }
    };

    template<ID_t id, class Derived>
    class Event  : public EventBase<Derived>{
    public:
        static const ID_t ID = id;
        template<typename BeginItr, typename EndItr>
        ProtocolErrc write(BeginItr& beginItr, const EndItr& endItr) {
            return ProtocolErrc::success;
        }
        template<typename T>
        ProtocolErrc write(const T& t) {
            return ProtocolErrc::success;
        }

    };

}

#endif // COP_EVENT_HPP

