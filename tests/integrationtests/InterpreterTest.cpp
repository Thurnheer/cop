#include <catch2/catch.hpp>
#include "cop/Interpreter.hpp"
#include "cop/Event.hpp"
#include "cop/detail/HandlerWrapper.hpp"
#include "cop/BinaryFramer.hpp"

enum myId {
    myId = 100
};

struct myEvent : cop::Event<myId> {
    char data = 'S';
    template<class Coder>
    auto parse(Coder coder) {
        return coder | data;
    }
};

SCENARIO("The Frame defines how to interpret or serialise Messages", "[Frame]")
{
    GIVEN("some data, and an event handler") {

        struct Handler {
            void handle(myEvent& e) {
                REQUIRE(e.data == 'T');
            }
        };
        
        std::vector buffer(6, std::byte(0));
        THEN("it will be interpreted and an Event generated") {
            const std::vector OUTPUT {
                // header (event), Id field (id: 1)
                std::byte(2), std::byte(0), std::byte(1), std::byte('T'),

                // data (char), crc
                std::byte(0xD8), std::byte(0x3D)
            };

            using Frame = std::tuple<cop::BinaryFramer>;
            using It = std::vector<std::byte>::iterator;
            auto it = buffer.begin(); auto end = buffer.end();
            cop::Interpreter<It, Frame> interpreter(it, end);
            REQUIRE(cop::ProtocolErrc::success == interpreter.interpret(std::byte('A'))); // Frame start
            REQUIRE(cop::ProtocolErrc::success == interpreter.interpret(std::byte(2))); // Header
            REQUIRE(cop::ProtocolErrc::success == interpreter.interpret(std::byte(0))); // id field first byte
            REQUIRE(cop::ProtocolErrc::success == interpreter.interpret(std::byte(1))); // id field second byte
            REQUIRE(cop::ProtocolErrc::success == interpreter.interpret(std::byte('T'))); // Data
            REQUIRE(cop::ProtocolErrc::success == interpreter.interpret(std::byte(0xD8))); // CRC
            REQUIRE(cop::ProtocolErrc::success == interpreter.interpret(std::byte(0x3D))); // CRC
            REQUIRE(cop::ProtocolErrc::success == interpreter.interpret(std::byte('A'))); // Frame end
            REQUIRE(OUTPUT == buffer);
        }
    }
}
