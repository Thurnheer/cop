#ifndef COP_DETAIL_EMPTY_HPP
#define COP_DETAIL_EMPTY_HPP

namespace cop::detail {
    class Empty {
    public:
        template<typename ...T>
        auto receive(T...);
    };
}

#endif // COP_DETAIL_EMPTY_HPP

