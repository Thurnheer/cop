#include <catch2/catch.hpp>
#include "cop/DataLinkLayer.hpp"

SCENARIO("The data link layer checks the framing and crc", "[DataLinkLayer]") {
    GIVEN("a frame") {
        static const int BUFFER_SIZE = 6;
        const std::vector FRAME {
            std::byte('A'), std::byte(0), std::byte(1), std::byte(2),
            std::byte(3), std::byte(0xA1), std::byte(0x10), std::byte('A')
        };
        std::vector<std::byte> buffer(BUFFER_SIZE, std::byte(0));
        auto it = buffer.begin(); auto end = buffer.end();
        THEN("it will be detected and the crc checked") {
            cop::DataLinkLayer dll(it, end);
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(FRAME[0]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(FRAME[1]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(FRAME[2]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(FRAME[3]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(FRAME[4]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(FRAME[5]));
            REQUIRE(cop::ProtocolErrc::receiving == dll.receive(FRAME[6]));
            REQUIRE(cop::ProtocolErrc::success == dll.receive(FRAME[7]));
        }
    }

    GIVEN("some data") {
        struct FrameAdapter {
            using Itr = std::vector<std::byte>::iterator;
            void send(Itr begin, Itr end) noexcept {
                data_.insert(data_.end(), begin, end);
            }
        private:
            std::vector<std::byte> data_{};
        };
        const std::vector DATA {
            std::byte('A'), std::byte('B'), std::byte('T')
        };
        static const int BUFFER_SIZE = 5;
        std::vector<std::byte> buffer(BUFFER_SIZE, std::byte(0));
        auto it = buffer.begin(); auto end = buffer.end();
        
        THEN("it will be packet within a frame") {
            buffer[0] = DATA[0];
            buffer[1] = DATA[1];
            buffer[2] = DATA[2];

            cop::DataLinkLayer dll(it, end);
            FrameAdapter frameAdapter{};
            REQUIRE(cop::ProtocolErrc::success == dll.send(frameAdapter));

            REQUIRE(buffer[0] == std::byte('A'));
            REQUIRE(buffer[1] == std::byte('B'));
            REQUIRE(buffer[2] == std::byte('T'));
            REQUIRE(buffer[3] == std::byte(0x4B));
            REQUIRE(buffer[4] == std::byte(0x61));
        }
    }
}
