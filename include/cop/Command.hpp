#ifndef COP_COMMAND_HPP
#define COP_COMMAND_HPP

#include "Id.hpp"

namespace cop {
    
    template<ID_t id>
    class Command : public Id_t<id> {
    public:
        using type = CommandT;
    };
}

#endif // COP_COMMAND_HPP

