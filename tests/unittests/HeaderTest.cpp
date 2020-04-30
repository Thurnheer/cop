#include <catch2/catch.hpp>
#include "cop/Header.hpp"
#include <vector>

SCENARIO("The header checks which optional fields and what kind of message there is", "[header]") {
    GIVEN(" a header with a command bit set") {
        std::vector<std::byte> data(1, std::byte(0x0));
        using It = std::vector<std::byte>::iterator;
        auto it = data.begin(); auto end = data.end();
        cop::Header<It> header(it, end);
        
        THEN("it is interpreted") {
            header.event(true);
            header.send();
            
            REQUIRE(data[0] == std::byte(0x2));

        }
    }

    GIVEN(" a byte buffer" ) {
        std::vector<std::byte> data(1, std::byte(0x4));
        using It = std::vector<std::byte>::const_iterator;
        auto it = data.cbegin(); auto end = data.cend();
        cop::Header<It> header(it, end);

        THEN("the header is constructed") {
            header.receive();

            REQUIRE(header.command());
        }
    }
}
