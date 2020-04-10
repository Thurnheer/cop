#ifndef COP_DETAIL_DYNAMICMEMORYPOLICY_HPP
#define COP_DETAIL_DYNAMICMEMORYPOLICY_HPP

#include <memory>

namespace cop::detail {

    template<typename Message>
    struct DynamicMemoryPolicy {
        using MessagePtr = std::unique_ptr<Message>;

        MessagePtr allocateMessage() {
            return std::make_unique<Message>();
        }
    };

}

#endif // COP_DETAIL_DYNAMICMEMORYPOLICY_HPP

