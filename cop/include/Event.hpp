#ifndef COP_EVENT_HPP
#define COP_EVENT_HPP

#include "Error.hpp"
#include "Id.hpp"

namespace cop {

    template<ID_t id>
    class Event : public Id_t<id>{
    public:
        using type = EventT;
    };

}

#endif // COP_EVENT_HPP

