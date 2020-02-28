#ifndef COP_UTILITIES_HPP
#define COP_UTILITIES_HPP

#include <type_traits>

template <typename TupleT>
struct TupleAsAlignedUnion;

template <typename ...TypesT>
struct TupleAsAlignedUnion<std::tuple<TypesT...> >
{
    using type = typename std::aligned_union<0, TypesT...>::type;
};

namespace detail {

template<std::size_t remaining>
class TupleForEachHelper {
public:
    template<typename Tuple, typename Function>
    static void exec(Function&& func) {
        using Tpl = typename std::decay<Tuple>::type;
        static const std::size_t TupleSize = std::tuple_size<Tuple>::value;
        static_assert(remaining <= TupleSize, "Incorrect remaining");

        static const std::size_t idx = TupleSize - remaining;
        using ElemType = typename std::tuple_element<idx, Tpl>::type;
        func.template operator()<ElemType>();
        TupleForEachHelper<remaining - 1>::template exec<Tpl> (std::forward<Function>(func));
    }
};

template<>
class TupleForEachHelper<0> {
public:
    template <typename Tuple, typename Function>
    static void exec(Function&& func) {
        // noting to do
    }
};

}

template<typename Tuple, typename Function>
void TupleForEach(Function&& func) {
    using tuple = typename std::decay<Tuple>::type;
    static const std::size_t TupleSize = std::tuple_size<tuple>::value;

    detail::TupleForEachHelper<TupleSize>::template exec<tuple>(std::forward<Function>(func));
}

#endif // COP_UTILITIES_HPP

