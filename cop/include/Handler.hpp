#ifndef COP_HANDLER_HPP
#define COP_HANDLER_HPP

#include <tuple>

namespace COP {

    class Message;

    template<typename CommonT, typename AllT>
    class GenericHandler;

    template<typename CommonT, typename FirstT, typename ...RestT>
    class GenericHandler<CommonT, std::tuple<FirstT, RestT...> >
    : public GenericHandler<CommonT, std::tuple<RestT...> >
    {
        using Base = GenericHandler<CommonT, std::tuple<RestT...> >;
    public:
        using Base::handle;
        virtual void handle(FirstT& msg) {
            this->handle(static_cast<CommonT&>(msg));
        }
    };

    template<typename CommonT>
    class GenericHandler<CommonT, std::tuple<> >{
    public:
        void handle(CommonT&) {
            // do nothing
        }
    };

}
#define HANDLER  namespace COP {  class Handler : public GenericHandler<Message, AllMessages> {};}

#endif // COP_HANDLER_HPP

