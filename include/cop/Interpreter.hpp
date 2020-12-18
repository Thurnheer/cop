#ifndef COP_INTERPRETER_HPP
#define COP_INTERPRETER_HPP

#include "Framer.hpp"
#include "Error.hpp"
#include "detail/Utilities.hpp"
#include <tuple>

namespace cop {

template<class Iterator, typename ...Types>
class Interpreter;

// needs a tuple definition of the frame in correct order
template<class Iterator, typename ...Types>
class Interpreter<Iterator, std::tuple<Types...> > {
    using FrameDefinition = std::tuple<Types...>;
    std::reference_wrapper<Iterator> it_;
    std::reference_wrapper<Iterator> end_;
    std::tuple<Types...> frame_;

public:
    Interpreter(Iterator& it, Iterator& end) : it_(it), end_(end), frame_()
    {
        if constexpr(std::is_same<
            FramerT, typename std::tuple_element<0,
            FrameDefinition>::type::type>::value)
        {
            std::get<0>(frame_) = std::move(typename std::tuple_element<0, FrameDefinition>::type());
        }
    }

    ProtocolErrc interpret(std::byte data) {
        if constexpr(std::is_same<
            FramerT, typename std::tuple_element<0,
            FrameDefinition>::type::type>::value)
        {
            return std::get<0>(frame_).receive(std::move(data), it_.get(), end_.get());
        }
        return ProtocolErrc::success;
    }
};

}

#endif // COP_INTERPRETER_HPP

