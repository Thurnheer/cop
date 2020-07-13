#include <catch2/catch.hpp>
#include "cop/DataLinkLayer.hpp"

SCENARIO("The data link layer checks the framing and crc", "[DataLinkLayer]") {
    GIVEN("a frame") {

        const std::vector DATA {
            std::byte('A'), std::byte(0), std::byte(1), std::byte(2),
            std::byte(3), std::byte(0xA1), std::byte(0x10), std::byte('A')
        };
        std::vector<std::byte> buffer(6, std::byte(0));
        auto it = buffer.begin(); auto end = buffer.end();
        THEN("it will be detected and the crc checked") {
            cop::DataLinkLayer dll(it, end);
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(DATA[0]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(DATA[1]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(DATA[2]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(DATA[3]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(DATA[4]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(DATA[5]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(DATA[6]));
            REQUIRE(cop::ProtocolErrc::success == dll.receive(DATA[7]));
        }
    }
}
