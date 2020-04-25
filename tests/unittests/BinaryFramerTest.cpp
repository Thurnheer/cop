#include <catch2/catch.hpp>
#include "cop/BinaryFramer.hpp"


SCENARIO("The binary framer handles frames and byte stuffing", "[BinaryFramer]")
{
    GIVEN("A Frame") {
        std::vector<std::byte> data(2);
        using It = std::vector<std::byte>::iterator;

        auto it = data.begin(); auto end = data.end();
        cop::BinaryFramer<It> framer(it, end);
        THEN("it will read between start and end frame") {

            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x41)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x43)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x31)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x41)));
            REQUIRE(data.size() == 2);
            REQUIRE(data == std::vector<std::byte>{ std::byte(0x43), std::byte(0x31)} );
        }
    }

    GIVEN(" a to small buffer") {
        std::vector<std::byte> data(1);
        using It = std::vector<std::byte>::iterator;

        auto it = data.begin(); auto end = data.end();
        cop::BinaryFramer<It> framer(it, end);

        THEN("it will tell you that there is not enough space in the buffer") {
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x41)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x31)));
            REQUIRE(cop::ProtocolErrc::not_enough_space_in_buffer == framer.receive(std::byte(0x31)));
        }
    }


    GIVEN(" a frame inside garbage ") {
        std::vector<std::byte> data(2);
        using It = std::vector<std::byte>::iterator;

        auto it = data.begin(); auto end = data.end();
        cop::BinaryFramer<It> framer(it, end);

        THEN(" it will extract only the frame " ) {
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x41)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x43)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x31)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x41)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0xf2)));
            REQUIRE(data.size() == 2);
            REQUIRE(data == std::vector<std::byte>{ std::byte(0x43), std::byte(0x31)} );

        }
    }

    GIVEN(" a frame with stuffing ") {
        std::vector<std::byte> data(2);
        using It = std::vector<std::byte>::iterator;

        auto it = data.begin(); auto end = data.end();
        cop::BinaryFramer<It> framer(it, end);

        THEN(" it will destuff the information " ) {
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x41)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte('\\')));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x41)));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte('\\')));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte('\\')));
            REQUIRE(cop::ProtocolErrc::success == framer.receive(std::byte(0x41)));
            REQUIRE(data.size() == 2);
            REQUIRE(data == std::vector<std::byte>{ std::byte('A'), std::byte(0x5C)} );

        }
    }

    GIVEN(" some data ") {
        std::vector data {
            std::byte(0x32), std::byte(0x34), std::byte('A'), std::byte('\\'),
            std::byte(0x32), std::byte('\\'), std::byte(0x34), std::byte('A')
        };

        using It = std::vector<std::byte>::const_iterator;
        auto it = data.cbegin(); auto end = data.cend();
        cop::BinaryFramer<It> framer(it, end);
        
        THEN("it will be stuffed correctly") {
            REQUIRE(std::byte('A') == framer.send());
            REQUIRE(std::byte(0x32) == framer.send());
            REQUIRE(std::byte(0x34) == framer.send());
            REQUIRE(std::byte('\\') == framer.send());
            REQUIRE(std::byte('A') == framer.send());
            REQUIRE(std::byte('\\') == framer.send());
            REQUIRE(std::byte('\\') == framer.send());
            REQUIRE(std::byte(0x32) == framer.send());
            REQUIRE(std::byte('\\') == framer.send());
            REQUIRE(std::byte('\\') == framer.send());
            REQUIRE(std::byte(0x34) == framer.send());
            REQUIRE(std::byte('\\') == framer.send());
            REQUIRE(std::byte('A') == framer.send());
            REQUIRE(std::byte('A') == framer.send());
            auto optinalByte = framer.send();
            REQUIRE(!optinalByte);
        }
    }
}
