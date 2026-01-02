#include "doctest.h"
#include "base64.h"
#include <string>
#include <vector>
#include <cstdint>

// Test base64 encoding functions
TEST_CASE("Base64 encoding with std::string") {
    std::string input = "Hello, World!";
    std::string output;
    
    b2t::base64_encode(output, input);
    
    CHECK(output == "SGVsbG8sIFdvcmxkIQ==");
}

TEST_CASE("Base64 encoding with std::vector<uint8_t>") {
    std::vector<uint8_t> input = {'H', 'e', 'l', 'l', 'o'};
    std::string output;

    b2t::base64_encode(output, input);

    CHECK(output == "SGVsbG8=");
}

TEST_CASE("Base64 encoding with uint8_t* and length") {
    const char* input = "Test";
    std::string output;
    
    b2t::base64_encode(output, reinterpret_cast<const uint8_t*>(input), 4);
    
    CHECK(output == "VGVzdA==");
}

// Test base64 decoding functions
TEST_CASE("Base64 decoding to std::string") {
    std::string input = "SGVsbG8sIFdvcmxkIQ=="; // "Hello, World!"
    std::string output;
    
    b2t::base64_decode(output, input);
    
    CHECK(output == "Hello, World!");
}

TEST_CASE("Base64 decoding to std::vector<uint8_t>") {
    std::string input = "SGVsbG8="; // "Hello"
    std::vector<uint8_t> output;

    b2t::base64_decode(output, input);

    std::vector<uint8_t> expected = {'H', 'e', 'l', 'l', 'o'};
    CHECK(output == expected);
}

// Test edge cases
TEST_CASE("Base64 encoding empty string") {
    std::string input = "";
    std::string output;
    
    b2t::base64_encode(output, input);
    
    CHECK(output == "");
}

TEST_CASE("Base64 decoding empty string") {
    std::string input = "";
    std::string output;
    
    b2t::base64_decode(output, input);
    
    CHECK(output == "");
}

TEST_CASE("Base64 encoding single character") {
    std::string input = "A";
    std::string output;
    
    b2t::base64_encode(output, input);
    
    CHECK(output == "QQ==");
}

TEST_CASE("Base64 decoding single character") {
    std::string input = "QQ==";
    std::string output;
    
    b2t::base64_decode(output, input);
    
    CHECK(output == "A");
}

TEST_CASE("Base64 encoding two characters") {
    std::string input = "AB";
    std::string output;
    
    b2t::base64_encode(output, input);
    
    CHECK(output == "QUI=");
}

TEST_CASE("Base64 decoding two characters") {
    std::string input = "QUI=";
    std::string output;
    
    b2t::base64_decode(output, input);
    
    CHECK(output == "AB");
}

// Test round-trip encoding/decoding
TEST_CASE("Base64 round-trip: encode then decode") {
    std::string original = "The quick brown fox jumps over the lazy dog.";
    std::string encoded;
    std::string decoded;
    
    b2t::base64_encode(encoded, original);
    b2t::base64_decode(decoded, encoded);
    
    CHECK(original == decoded);
}

TEST_CASE("Base64 round-trip with binary data") {
    std::vector<uint8_t> original = {0, 1, 2, 3, 255, 254, 253};
    std::string encoded;
    std::vector<uint8_t> decoded;
    
    b2t::base64_encode(encoded, original);
    b2t::base64_decode(decoded, encoded);
    
    CHECK(original == decoded);
}

// Test special characters and longer strings
TEST_CASE("Base64 encoding special characters") {
    std::string input = "Hello\nWorld\t!";
    std::string output;
    
    b2t::base64_encode(output, input);
    
    std::string decoded;
    b2t::base64_decode(decoded, output);
    CHECK(input == decoded);
}

TEST_CASE("Base64 encoding longer string") {
    std::string input = "This is a longer string to test the base64 encoding and decoding functionality.";
    std::string output;
    
    b2t::base64_encode(output, input);
    
    std::string decoded;
    b2t::base64_decode(decoded, output);
    CHECK(input == decoded);
}

// Test padding scenarios
TEST_CASE("Base64 encoding with different padding requirements") {
    // String length 1 (needs 2 padding characters)
    std::string input1 = "A";
    std::string output1;
    b2t::base64_encode(output1, input1);
    CHECK(output1 == "QQ==");

    // String length 2 (needs 1 padding character)
    std::string input2 = "AB";
    std::string output2;
    b2t::base64_encode(output2, input2);
    CHECK(output2 == "QUI=");

    // String length 3 (no padding needed)
    std::string input3 = "ABC";
    std::string output3;
    b2t::base64_encode(output3, input3);
    CHECK(output3 == "QUJD");
}

// Additional tests for binary data with null bytes
TEST_CASE("Base64 encoding with null bytes") {
    std::vector<uint8_t> input = {0, 1, 0, 2, 0};
    std::string output;

    b2t::base64_encode(output, input);

    std::vector<uint8_t> decoded;
    b2t::base64_decode(decoded, output);

    CHECK(input == decoded);
}

// Test various sizes to ensure proper padding
TEST_CASE("Base64 encoding various sizes") {
    // 0 bytes
    std::vector<uint8_t> input0 = {};
    std::string output0;
    b2t::base64_encode(output0, input0);
    CHECK(output0 == "");

    // 1 byte -> 4 characters with padding
    std::vector<uint8_t> input1 = {65}; // 'A'
    std::string output1;
    b2t::base64_encode(output1, input1);
    CHECK(output1 == "QQ==");

    // 2 bytes -> 4 characters with padding
    std::vector<uint8_t> input2 = {65, 66}; // 'AB'
    std::string output2;
    b2t::base64_encode(output2, input2);
    CHECK(output2 == "QUI=");

    // 3 bytes -> 4 characters without padding
    std::vector<uint8_t> input3 = {65, 66, 67}; // 'ABC'
    std::string output3;
    b2t::base64_encode(output3, input3);
    CHECK(output3 == "QUJD");

    // 4 bytes -> 8 characters with padding
    std::vector<uint8_t> input4 = {65, 66, 67, 68}; // 'ABCD'
    std::string output4;
    b2t::base64_encode(output4, input4);
    CHECK(output4 == "QUJDRA==");
}

// Test decoding with various padding scenarios
TEST_CASE("Base64 decoding with various inputs") {
    // Valid base64 strings with different padding
    std::string encoded1 = "QQ==";  // "A"
    std::string decoded1;
    b2t::base64_decode(decoded1, encoded1);
    CHECK(decoded1 == "A");

    std::string encoded2 = "QUI=";  // "AB"
    std::string decoded2;
    b2t::base64_decode(decoded2, encoded2);
    CHECK(decoded2 == "AB");

    std::string encoded3 = "QUJD";  // "ABC"
    std::string decoded3;
    b2t::base64_decode(decoded3, encoded3);
    CHECK(decoded3 == "ABC");

    std::string encoded4 = "QUJDRA==";  // "ABCD"
    std::string decoded4;
    b2t::base64_decode(decoded4, encoded4);
    CHECK(decoded4 == "ABCD");
}

// Test round-trip consistency for various inputs
TEST_CASE("Base64 round-trip consistency") {
    std::vector<std::string> test_cases = {
        "",
        "A",
        "AB",
        "ABC",
        "ABCD",
        "Hello, World!",
        "The quick brown fox jumps over the lazy dog.",
        "1234567890",
        "!@#$%^&*()",
        "Multi-byte characters: àáâãäåæçèé",
        "Binary-like: \x00\x01\x02\x03\xff\xfe\xfd"
    };

    for (const auto& test_case : test_cases) {
        std::string encoded;
        b2t::base64_encode(encoded, test_case);

        std::string decoded;
        b2t::base64_decode(decoded, encoded);

        CHECK(test_case == decoded);
    }
}

// Test vector<uint8_t> round-trip
TEST_CASE("Base64 vector<uint8_t> round-trip") {
    std::vector<std::vector<uint8_t>> test_cases = {
        {},
        {0},
        {0, 1},
        {0, 1, 2},
        {0, 1, 2, 3},
        {255, 254, 253, 252},
        {0, 128, 255, 100, 50, 25, 0}
    };

    for (const auto& test_case : test_cases) {
        std::string encoded;
        b2t::base64_encode(encoded, test_case);

        std::vector<uint8_t> decoded;
        b2t::base64_decode(decoded, encoded);

        CHECK(test_case == decoded);
    }
}
