#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "base16.h"
#include <string>
#include <vector>

using namespace std;
using namespace b2t;

TEST_CASE("Base16 encoding and decoding") {
    string input = "Hello, World!";
    string encoded, decoded;
    
    // Test encoding
    base16_encode(encoded, input);
    CHECK(encoded == "48656C6C6F2C20576F726C6421");
    
    // Test decoding
    base16_decode(decoded, encoded);
    CHECK(decoded == input);
    
    // Test with empty string
    string empty_input = "";
    string empty_encoded, empty_decoded;
    base16_encode(empty_encoded, empty_input);
    CHECK(empty_encoded == "");
    base16_decode(empty_decoded, empty_encoded);
    CHECK(empty_decoded == "");
    
    // Test with binary data
    vector<uint8_t> binary_data = {0x00, 0x01, 0x02, 0xFF, 0xFE, 0xFD};
    string binary_encoded, binary_decoded;
    base16_encode(binary_encoded, binary_data);
    CHECK(binary_encoded == "000102FFFEFD");
    base16_decode(binary_decoded, binary_encoded);
    CHECK(binary_decoded.size() == 6);
    CHECK(binary_decoded[0] == 0x00);
    CHECK(binary_decoded[1] == 0x01);
    CHECK(binary_decoded[2] == 0x02);
    CHECK(binary_decoded[3] == 0xFF);
    CHECK(binary_decoded[4] == 0xFE);
    CHECK(binary_decoded[5] == 0xFD);
}