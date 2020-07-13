#include <catch2/catch.hpp>
#include "cop/BinaryFramer.hpp"


SCENARIO("The binary framer handles frames and byte stuffing", "[BinaryFramer]")
{
    static const std::byte FrameStart = cop::BinaryFramer::FRAME_START_END;
    static const std::byte FrameEnd = cop::BinaryFramer::FRAME_START_END;

    GIVEN("A Frame") {
        std::vector<std::byte> data(2);

        auto it = data.begin(); auto end = data.end();
        cop::BinaryFramer framer;
        THEN("it will read between start and end frame") {

            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(FrameStart), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0x43), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0x31), it, end));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(FrameEnd), it, end));
            REQUIRE(data.size() == 2);
            REQUIRE(data == std::vector<std::byte>{ std::byte(0x43), std::byte(0x31)} );
        }
    }

    GIVEN(" a to small buffer") {
        std::vector<std::byte> data(1);

        auto it = data.begin(); auto end = data.end();
        cop::BinaryFramer framer;

        THEN("it will tell you that there is not enough space in the buffer") {
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(FrameStart), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0x31), it, end));
            REQUIRE(cop::ProtocolErrc::not_enough_space_in_buffer == framer.receive(std::byte(0x31), it, end));
        }
    }


    GIVEN(" a frame inside garbage ") {
        std::vector<std::byte> data(2);

        auto it = data.begin(); auto end = data.end();
        cop::BinaryFramer framer;

        THEN(" it will extract only the frame " ) {
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(FrameStart), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0x43), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0x31), it, end));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(FrameEnd), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0xf2), it, end));
            REQUIRE(data.size() == 2);
            REQUIRE(data == std::vector<std::byte>{ std::byte(0x43), std::byte(0x31)} );

        }
    }

    GIVEN(" a frame with stuffing ") {
        std::vector<std::byte> data(2);

        auto it = data.begin(); auto end = data.end();
        cop::BinaryFramer framer;

        THEN(" it will destuff the information " ) {
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(FrameStart), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte('\\'), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte(0x41), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte('\\'), it, end));
            REQUIRE(cop::ProtocolErrc::receiving == framer.receive(std::byte('\\'), it, end));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(FrameEnd), it, end));
            REQUIRE(data.size() == 2);
            REQUIRE(data == std::vector<std::byte>{ std::byte('A'), std::byte(0x5C)} );

        }
    }

    GIVEN(" some data ") {
        std::vector data {
            std::byte(0x32), std::byte(0x34), std::byte('A'), std::byte('\\'),
            std::byte(0x32), std::byte('\\'), std::byte(0x34), std::byte('A')
        };

        auto it = data.cbegin(); auto end = data.cend();
        cop::BinaryFramer framer;
        
        THEN("it will be stuffed correctly") {
            REQUIRE(std::byte('A') == framer.send(it, end));
            REQUIRE(std::byte(0x32) == framer.send(it, end));
            REQUIRE(std::byte(0x34) == framer.send(it, end));
            REQUIRE(std::byte('\\') == framer.send(it, end));
            REQUIRE(std::byte('A') == framer.send(it, end));
            REQUIRE(std::byte('\\') == framer.send(it, end));
            REQUIRE(std::byte('\\') == framer.send(it, end));
            REQUIRE(std::byte(0x32) == framer.send(it, end));
            REQUIRE(std::byte('\\') == framer.send(it, end));
            REQUIRE(std::byte('\\') == framer.send(it, end));
            REQUIRE(std::byte(0x34) == framer.send(it, end));
            REQUIRE(std::byte('\\') == framer.send(it, end));
            REQUIRE(std::byte('A') == framer.send(it, end));
            REQUIRE(std::byte('A') == framer.send(it, end));
            auto optinalByte = framer.send(it, end);
            REQUIRE(!optinalByte);
        }
    }
}
