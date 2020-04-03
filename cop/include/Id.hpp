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

}

#endif // COP_EVENT_HPP

