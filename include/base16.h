#ifndef BIN2TEXT_BASE16_H
#define BIN2TEXT_BASE16_H
#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace b2t {

    // Base16 (Hex) encoding and decoding functions

    void base16_encode(std::string & out, const std::vector<uint8_t>& buf);
    void base16_encode(std::string & out, const uint8_t* buf, size_t bufLen);
    void base16_encode(std::string & out, std::string const& buf);

    void base16_decode(std::vector<uint8_t> & out, std::string const& encoded_string);
    void base16_decode(std::string & out, std::string const& encoded_string);

    // Implementation
    namespace {
        static const char to_base16[17] = "0123456789ABCDEF";
        
        inline uint8_t hex_char_to_value(char c) {
            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'A' && c <= 'F') return c - 'A' + 10;
            if (c >= 'a' && c <= 'f') return c - 'a' + 10;
            return 0; // Invalid hex character
        }
    }

    inline void base16_encode(std::string & out, std::string const& buf) {
       if (buf.empty())
          base16_encode(out, NULL, 0);
       else
          base16_encode(out, reinterpret_cast<uint8_t const*>(&buf[0]), buf.size());
    }


    inline void base16_encode(std::string & out, std::vector<uint8_t> const& buf) {
       if (buf.empty())
          base16_encode(out, NULL, 0);
       else
          base16_encode(out, &buf[0], buf.size());
    }

    inline void base16_encode(std::string & ret, uint8_t const* buf, size_t bufLen) {
       if (!buf) {
          ret.clear();
          return;
       }

       ret.clear();
       ret.reserve(bufLen * 2); // Each byte becomes 2 hex characters

       for (size_t i = 0; i < bufLen; ++i) {
          ret.push_back(to_base16[(buf[i] >> 4) & 0x0F]);
          ret.push_back(to_base16[buf[i] & 0x0F]);
       }
    }


    template <class Out>
    inline void base16_decode_any( Out & ret, std::string const& in) {
        typedef typename Out::value_type T;

       ret.clear();
       ret.reserve(in.size() / 2); // Each 2 hex chars become 1 byte

       for (size_t i = 0; i < in.size(); i += 2) {
          if (i + 1 < in.size()) {
             uint8_t high = hex_char_to_value(in[i]);
             uint8_t low = hex_char_to_value(in[i + 1]);
             ret.push_back(static_cast<T>((high << 4) | low));
          } else if (i < in.size()) {
             // Odd number of characters - treat as if padded with 0
             uint8_t high = hex_char_to_value(in[i]);
             ret.push_back(static_cast<T>((high << 4)));
          }
       }
    }

    inline void base16_decode(std::vector<uint8_t> & out, std::string const& encoded_string) {
       base16_decode_any(out, encoded_string);
    }

    inline void base16_decode(std::string & out, std::string const& encoded_string) {
       base16_decode_any(out, encoded_string);
    }

} // namespace b2t


#endif // BIN2TEXT_BASE16_H