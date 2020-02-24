#ifndef COP_MESSAGE_HPP
#define COP_MESSAGE_HPP


namespace COP {

    class Handler;

    class Message {
    public:
        void dispatch(Handler& handler) {
            dispatchImpl(handler);
        }

    protected:
        virtual void dispatchImpl(Handler& handler) = 0;
    };
    
}

#endif // COP_MESSAGE_HPP

