#include <catch2/catch.hpp>
#include "cop/Crc.hpp"

static const auto ONE = static_cast<unsigned char>(0x01);
static const auto TWO = static_cast<unsigned char>(0x02);
static const auto THREE = static_cast<unsigned char>(0x03);
static const auto FOUR = static_cast<unsigned char>(0x04);
static const auto FIVE = static_cast<unsigned char>(0x05);
static const auto SIX = static_cast<unsigned char>(0x06);
static const auto SEVEN = static_cast<unsigned char>(0x07);
static const auto EIGHT = static_cast<unsigned char>(0x08);
static const auto NINE = static_cast<unsigned char>(0x09);
static const auto SPACE_FOR_CRC = static_cast<unsigned char>(0x00);

static const auto ASCII_ONE = static_cast<unsigned char>('1');
static const auto ASCII_TWO = static_cast<unsigned char>('2');
static const auto ASCII_THREE = static_cast<unsigned char>('3');
static const auto ASCII_FOUR = static_cast<unsigned char>('4');
static const auto ASCII_FIVE = static_cast<unsigned char>('5');
static const auto ASCII_SIX = static_cast<unsigned char>('6');
static const auto ASCII_SEVEN = static_cast<unsigned char>('7');
static const auto ASCII_EIGHT = static_cast<unsigned char>('8');
static const auto ASCII_NINE = static_cast<unsigned char>('9');

SCENARIO("The Crc will be calculated", "[CRC]") {
    GIVEN(" some data") {
        std::vector data{
           std::byte(ONE), std::byte(TWO), std::byte(THREE), std::byte(FOUR),
           std::byte(FIVE), std::byte(SIX), std::byte(SEVEN), std::byte(EIGHT),
           std::byte(NINE), std::byte(SPACE_FOR_CRC), std::byte(SPACE_FOR_CRC)
        };
        auto it = data.begin(); auto end = data.end();

        cop::Crc crc;

        THEN("the crc will be appended") {
            crc.send(it, end);

            REQUIRE(std::byte(0x42) == data[9]);
            REQUIRE(std::byte(0x04) == data[10]);

        }
    }

    GIVEN(" a message ") {
        std::vector data{
            std::byte(ASCII_ONE), std::byte(ASCII_TWO), std::byte(ASCII_THREE), std::byte(ASCII_FOUR),
            std::byte(ASCII_FIVE), std::byte(ASCII_SIX), std::byte(ASCII_SEVEN), std::byte(ASCII_EIGHT),
            std::byte(ASCII_NINE), std::byte(0xBB), std::byte(0x3D) // NOLINT
        };
        auto it = data.cbegin(); auto end = data.cend();


        THEN(" the crc will be checked") {
            cop::Crc crc;
            REQUIRE(cop::ProtocolErrc::success == crc.receive(it, end));
        }

        THEN(" the crc will be checked, and an error reported") {
            data[10] = std::byte(0xD3); // instead of 0x3D NOLINT
            cop::Crc crc;
            REQUIRE(cop::ProtocolErrc::crc_error == crc.receive(it, end));
        }
    }
}
