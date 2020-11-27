#include <catch2/catch.hpp>
#include "cop/Field.hpp"

SCENARIO("The field has a value and can be serialized") {
    GIVEN(" a field" ) {
        std::vector data{ std::byte(0x0), std::byte(0x0)};
        auto it = data.begin(); auto end = data.end();

        static const uint16_t FIELD_DATA = 4080;
        cop::Field field;
        field.value(FIELD_DATA);

        THEN(" it is serialized") {

            field.send(it, end);

            REQUIRE(data[0] == std::byte(0x0F));
            REQUIRE(data[1] == std::byte(0xF0));
        }
    }

    GIVEN(" a buffer ") {
        static const auto FIELD_DATA_FIRST_BYTE = static_cast<unsigned char>(0xF0);
        static const auto FIELD_DATA_SECOND_BYTE = static_cast<unsigned char>(0x0F);
        std::vector data{ std::byte(FIELD_DATA_FIRST_BYTE), std::byte(FIELD_DATA_SECOND_BYTE)};
        auto it = data.cbegin(); auto end = data.cend();

        cop::Field field;
        THEN("it can be deserialised") {

            field.receive(it, end);

            REQUIRE(61455 == field.value());
        }
    }
}
