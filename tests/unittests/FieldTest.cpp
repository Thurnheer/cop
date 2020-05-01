#include <catch2/catch.hpp>
#include "cop/Field.hpp"

SCENARIO("The field has a value and can be serialized") {
    GIVEN(" a field" ) {
        std::vector data{ std::byte(0x0), std::byte(0x0)};
        auto it = data.begin(); auto end = data.end();

        cop::Field field(it, end);
        field.value(4080);

        THEN(" it is serialized") {

            field.send();

            REQUIRE(data[0] == std::byte(0x0F));
            REQUIRE(data[1] == std::byte(0xF0));
        }
    }

    GIVEN(" a buffer ") {
        std::vector data{ std::byte(0xF0), std::byte(0x0F)};
        auto it = data.cbegin(); auto end = data.cend();

        cop::Field field(it, end);
        THEN("it can be deserialised") {

            field.receive();

            REQUIRE(61455 == field.value());
        }
    }
}
