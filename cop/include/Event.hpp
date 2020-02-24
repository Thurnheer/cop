#ifndef COP_EVENT_HPP
#define COP_EVENT_HPP

#include "Message.hpp"
#include "Handler.hpp"

namespace COP {

    using ID_t = uint32_t;

    template<class derived>
    class EventBase : public Message {
    protected:
        virtual void dispatchImpl(Handler& handler) override;
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
        void write(BeginItr& beginItr, const EndItr& endItr) {

        }
        template<typename T>
        void write(const T& t) {

        }

    };

}

#define DISPATCH namespace COP { \
    template<typename derived> \
    void COP::EventBase<derived>::dispatchImpl(COP::Handler& handler) { \
        handler.handle(static_cast<derived&>(*this)); \
    }}
#endif // COP_EVENT_HPP
