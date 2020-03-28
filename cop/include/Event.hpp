#ifndef COP_EVENT_HPP
#define COP_EVENT_HPP

#include "Error.hpp"

namespace cop {

    using ID_t = uint32_t;

    template<ID_t id>
    class Id_t {
    public:
        static const ID_t ID = id;
    };

    template<ID_t id, class Derived>
    class ReceivedEvent : public Id_t<id> {
    public:
        auto read() {
            return 42;
        }
    };

    template<ID_t id, class Derived>
    class Event  : public Id_t<id>{
    public:
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

