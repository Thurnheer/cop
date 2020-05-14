#include <catch2/catch.hpp>
#include "cop/Header.hpp"
#include <vector>

SCENARIO("The header checks which optional fields and what kind of message there is", "[header]") {
    GIVEN(" a header with a command bit set") {
        std::vector<std::byte> data(1, std::byte(0x0));
        auto it = data.begin(); auto end = data.end();
        
        THEN("it is interpreted") {
            cop::Header header;
            header.event(true);
            header.send(it, end);
            
            REQUIRE(data[0] == std::byte(0x2));

        }
    }

    GIVEN(" a byte buffer" ) {
        std::vector<std::byte> data(1, std::byte(0x4));
        auto it = data.cbegin(); auto end = data.cend();

        THEN("the header is constructed") {
            cop::Header header;
            header.receive(it, end);

            REQUIRE(header.command());
        }
    }
}
