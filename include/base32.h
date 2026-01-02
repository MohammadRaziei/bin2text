#ifndef BIN2TEXT_BASE32_H
#define BIN2TEXT_BASE32_H
#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace b2t {

    // Base32 encoding and decoding functions

    void base32_encode(std::string & out, const std::vector<uint8_t>& buf);
    void base32_encode(std::string & out, const uint8_t* buf, size_t bufLen);
    void base32_encode(std::string & out, std::string const& buf);

    void base32_decode(std::vector<uint8_t> & out, std::string const& encoded_string);
    void base32_decode(std::string & out, std::string const& encoded_string);

    // Implementation
    namespace {
        static const uint8_t from_base32[128] = {
            // 8 rows of 16 = 128
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 20-2F (' ' to '/')
            0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, // 30-3F ('0' to '?')
            0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 40-4F ('@' to 'O')
            0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, // 50-5F ('P' to '_') - Upper case letters
            0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  // 60-6F ('`' to 'o') - Lower case letters
        };

        static const char to_base32[33] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
    }

    inline void base32_encode(std::string & out, std::string const& buf) {
       if (buf.empty())
          base32_encode(out, NULL, 0);
       else
          base32_encode(out, reinterpret_cast<uint8_t const*>(&buf[0]), buf.size());
    }


    inline void base32_encode(std::string & out, std::vector<uint8_t> const& buf) {
       if (buf.empty())
          base32_encode(out, NULL, 0);
       else
          base32_encode(out, &buf[0], buf.size());
    }

    inline void base32_encode(std::string & ret, uint8_t const* buf, size_t bufLen) {
       if (!buf) {
          ret.clear();
          return;
       }

       ret.clear();
       ret.reserve(((bufLen + 4) / 5) * 8); // Reserve enough space

       size_t idx = 0;
       size_t c;
       uint8_t tmp;
       while (idx < bufLen) {
           c = (bufLen - idx < 5) ? bufLen - idx : 5;

           // Encode 5 bytes at a time
           switch (c) {
           case 5:
               tmp = (buf[idx + 4] & 0x1F);
               ret.push_back(to_base32[tmp]);
               tmp = ((buf[idx + 3] & 0x03) << 3) | ((buf[idx + 4] & 0xE0) >> 5);
               ret.push_back(to_base32[tmp]);
               // fallthrough
           case 4:
               tmp = ((buf[idx + 2] & 0x0F) << 1) | ((buf[idx + 3] & 0x80) >> 7);
               ret.push_back(to_base32[tmp]);
               tmp = ((buf[idx + 3] & 0x7C) >> 2);
               ret.push_back(to_base32[tmp]);
               // fallthrough
           case 3:
               tmp = (buf[idx + 2] & 0x07) << 3 | ((buf[idx + 1] & 0xE0) >> 5);
               ret.push_back(to_base32[tmp]);
               tmp = ((buf[idx + 0] & 0x1F) << 1) | ((buf[idx + 1] & 0x80) >> 7);
               ret.push_back(to_base32[tmp]);
               // fallthrough
           case 2:
               tmp = ((buf[idx + 1] & 0x7C) >> 2);
               ret.push_back(to_base32[tmp]);
               tmp = ((buf[idx + 0] & 0x03) << 3) | ((buf[idx + 1] & 0x80) >> 5);
               ret.push_back(to_base32[tmp]);
               // fallthrough
           case 1:
               tmp = ((buf[idx + 0] & 0xF8) >> 3);
               ret.push_back(to_base32[tmp]);
               tmp = ((buf[idx + 0] & 0x07) << 2);
               ret.push_back(to_base32[tmp]);
           }

           // Add padding if needed
           switch (c) {
           case 1:
               ret.append(6, '=');
               break;
           case 2:
               ret.append(4, '=');
               break;
           case 3:
               ret.append(3, '=');
               break;
           case 4:
               ret.append(1, '=');
               break;
           }

           idx += c;
       }
    }


    template <class Out>
    inline void base32_decode_any( Out & ret, std::string const& in) {
        typedef typename Out::value_type T;

       ret.clear();
       ret.reserve(in.size() * 5 / 8); // Reserve estimated space

       size_t idx = 0;
       size_t c;
       uint8_t tmp;
       while (idx < in.size()) {
           c = (in.size() - idx < 8) ? in.size() - idx : 8;

           // Skip padding characters
           if (in[idx] == '=') break;

           // Decode 8 characters at a time (or fewer for the last group)
           uint8_t b[8];
           for (size_t i = 0; i < c; ++i) {
               if (idx + i >= in.size() || in[idx + i] == '=') {
                   b[i] = 0; // Padding character
               } else {
                   b[i] = (in[idx + i] > 122) ? 0xFF : from_base32[static_cast<uint8_t>(in[idx + i])];
                   if (b[i] == 0xFF) b[i] = 0; // Invalid character, treat as padding
               }
           }

           // Process the 8 base32 characters into 5 bytes
           ret.push_back(static_cast<T>(((b[0] & 0x1F) << 3) | ((b[1] & 0x1C) >> 2)));
           if (c > 2 && b[2] != 0x1F) { // Check if we have at least 2 more chars and not padding
               ret.push_back(static_cast<T>(((b[1] & 0x03) << 6) | ((b[2] & 0x1F) << 1) | ((b[3] & 0x10) >> 4)));
           }
           if (c > 4 && b[4] != 0x1F) {
               ret.push_back(static_cast<T>(((b[3] & 0x0F) << 4) | ((b[4] & 0x1E) >> 1)));
           }
           if (c > 5 && b[5] != 0x1F) {
               ret.push_back(static_cast<T>(((b[4] & 0x01) << 7) | ((b[5] & 0x1F) << 2) | ((b[6] & 0x18) >> 3)));
           }
           if (c > 7 && b[7] != 0x1F) {
               ret.push_back(static_cast<T>(((b[6] & 0x07) << 5) | (b[7] & 0x1F)));
           }

           idx += c;
       }
    }

    inline void base32_decode(std::vector<uint8_t> & out, std::string const& encoded_string) {
       base32_decode_any(out, encoded_string);
    }

    inline void base32_decode(std::string & out, std::string const& encoded_string) {
       base32_decode_any(out, encoded_string);
    }

} // namespace b2t


#endif // BIN2TEXT_BASE32_H