#ifndef COP_EVENT_HPP
#define COP_EVENT_HPP

namespace COP {
    class ReceivedEvent {
    public:
        auto read() {
            return 42;
        }
    };
    class Event {
    public:
        template<typename BeginItr, typename EndItr>
        void write(BeginItr& beginItr, const EndItr& endItr) {

        }
        template<typename T>
        void write(const T& t) {

        }

    };
}

#endif // COP_EVENT_HPP
