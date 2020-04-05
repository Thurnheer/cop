#ifndef COP_ID_HPP
#define COP_ID_HPP

#include "Error.hpp"

namespace cop {

    using ID_t = uint32_t;

    template<ID_t id>
    class Id_t {
    public:
        static const ID_t ID = id;
    };

    struct EventT {};

    struct CommandT {};

}

#endif // COP_ID_HPP

