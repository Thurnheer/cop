#ifndef COP_IDLAYER_HPP
#define COP_IDLAYER_HPP

#include "Error.hpp"
#include "Utilities.hpp"
#include <memory>
#include <array>
#include <algorithm>

namespace COP {

template<class AllMessages>
class IdLayer {
public:
    
    using NewMsg = std::unique_ptr<Message>;

    IdLayer() {
        TupleForEach<AllMessages>(MessageFactoryCreator(registry_));
    }
    
    NewMsg read( ID_t id) {
        /*Field field;
        if(auto e = field.read()) {
            return e;
        }*/
        return createMessage(id);
        
        /*msg = createMessage(id);

        if(!msg) {
            return ProtocolErrc::invalidMessageId;
        }

        if(auto e = next_.read(iter, end)) {
            msg.reset();
            return e;
        }*/
        //return ProtocolErrc::success;
    }

    /*ProtocolErrc write(const Message& msg, WriteIterator& iter, EndIterator& end) const
    {
        Field field;
        field.value = msg.id();
        if(auto e = field.write(iter, end)) {
            return e;
        }
        return next_.write(msg, iter, end);
    }*/

private:

    static const auto RegistrySize = std::tuple_size<AllMessages>::value;
    
    class Factory {
    public:
        ID_t id() const {
            return idImpl();
        }
        NewMsg createMsg() const {
            return createMsgImpl();
        }
    private:
        virtual ID_t idImpl() const = 0;
        virtual NewMsg createMsgImpl() const = 0;
    };

    using Registry = std::array<const Factory*, RegistrySize>;

    template <typename ActualMessage>
    class MessageFactory : public Factory
    {
    protected:
        virtual ID_t idImpl() const override
        {
            return ActualMessage::ID;
        }

        virtual NewMsg createMsgImpl() const override {
            return NewMsg(new ActualMessage);
        }
    };

    class MessageFactoryCreator {
    public: 
        MessageFactoryCreator(Registry& registry)
        : registry_(registry)
        {}

        template <typename MessageT>
        void operator()() {
            static const MessageFactory<MessageT> factory;
            registry_[idx_++] = &factory;
        }
    private:
        Registry& registry_;
        unsigned int idx_ = 0;
    };

    NewMsg createMessage(ID_t id) {
        const auto f = std::find_if(registry_.cbegin(), registry_.cend(), [&](const auto factory) {
            return id == factory->id();
        });
        if(f != registry_.cend()) {
            return (*f)->createMsg();
        }
        return NewMsg();
    }
    
    Registry registry_;
    //NextT next_;
};

}

#endif // COP_IDLAYER_HPP

