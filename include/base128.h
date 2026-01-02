#ifndef BIN2TEXT_BASE128_H
#define BIN2TEXT_BASE128_H
#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace b2t {

    // Base128 encoding and decoding functions
    // Note: Base128 is not a standard encoding like Base64/32, but we implement it as a custom encoding
    // using 7 bits per character to represent 7 bits of data per character

    void base128_encode(std::string & out, const std::vector<uint8_t>& buf);
    void base128_encode(std::string & out, const uint8_t* buf, size_t bufLen);
    void base128_encode(std::string & out, std::string const& buf);

    void base128_decode(std::vector<uint8_t> & out, std::string const& encoded_string);
    void base128_decode(std::string & out, std::string const& encoded_string);

    // Implementation
    namespace {
        // Use printable ASCII characters from 33 to 126 (excluding space and DEL)
        static const char to_base128[128] = 
            "!\"#$%&'()*+,-./0123456789:;<=>?@"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
            "`abcdefghijklmnopqrstuvwxyz{|}~";
        
        inline uint8_t base128_char_to_value(char c) {
            if (c >= 33 && c <= 126) {
                return static_cast<uint8_t>(c - 33);
            }
            return 0; // Invalid character
        }
    }

    inline void base128_encode(std::string & out, std::string const& buf) {
       if (buf.empty())
          base128_encode(out, NULL, 0);
       else
          base128_encode(out, reinterpret_cast<uint8_t const*>(&buf[0]), buf.size());
    }


    inline void base128_encode(std::string & out, std::vector<uint8_t> const& buf) {
       if (buf.empty())
          base128_encode(out, NULL, 0);
       else
          base128_encode(out, &buf[0], buf.size());
    }

    inline void base128_encode(std::string & ret, uint8_t const* buf, size_t bufLen) {
       if (!buf) {
          ret.clear();
          return;
       }

       ret.clear();
       // Each 7 bytes of input becomes 8 bytes of output (7*8=56 bits, 8*7=56 bits)
       ret.reserve((bufLen * 8 + 6) / 7); // Calculate required space

       size_t bit_idx = 0;
       size_t byte_idx = 0;
       uint64_t buffer = 0;

       while (byte_idx < bufLen) {
          // Add 8 bits from input to buffer
          buffer |= (static_cast<uint64_t>(buf[byte_idx]) << bit_idx);
          bit_idx += 8;
          byte_idx++;

          // When we have at least 7 bits, output a character
          while (bit_idx >= 7) {
             ret.push_back(to_base128[buffer & 0x7F]); // Take lowest 7 bits
             buffer >>= 7; // Remove the 7 bits we just used
             bit_idx -= 7;
          }
       }

       // Output remaining bits if any
       if (buffer > 0 || bit_idx > 0) {
          ret.push_back(to_base128[buffer & 0x7F]);
       }
    }


    template <class Out>
    inline void base128_decode_any( Out & ret, std::string const& in) {
        typedef typename Out::value_type T;

       ret.clear();
       // Each 8 characters of input produces 7 bytes of output (approximately)
       ret.reserve((in.size() * 7 + 7) / 8);

       size_t bit_idx = 0;
       uint64_t buffer = 0;

       for (size_t i = 0; i < in.size(); ++i) {
          uint8_t val = base128_char_to_value(in[i]);
          buffer |= (static_cast<uint64_t>(val) << bit_idx);
          bit_idx += 7;

          // When we have at least 8 bits, output a byte
          while (bit_idx >= 8) {
             ret.push_back(static_cast<T>(buffer & 0xFF)); // Take lowest 8 bits
             buffer >>= 8; // Remove the 8 bits we just used
             bit_idx -= 8;
          }
       }

       // Output remaining bytes if any (there should be at most one)
       if (bit_idx > 0) {
          ret.push_back(static_cast<T>(buffer & 0xFF));
       }
    }

    inline void base128_decode(std::vector<uint8_t> & out, std::string const& encoded_string) {
       base128_decode_any(out, encoded_string);
    }

    inline void base128_decode(std::string & out, std::string const& encoded_string) {
       base128_decode_any(out, encoded_string);
    }

} // namespace b2t


#endif // BIN2TEXT_BASE128_H