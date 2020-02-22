#ifndef COP_EVENT_HPP
#define COP_EVENT_HPP

namespace COP {

    using ID_t = uint32_t;
    
    struct EventBase{};

    template<ID_t id>
    class ReceivedEvent : public EventBase {
    public:
        static const ID_t ID = id;
        auto read() {
            return 42;
        }
    };

    template<ID_t id>
    class Event  : public EventBase{
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

#endif // COP_EVENT_HPP
