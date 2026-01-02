# bin2text

A Cython project with scikit-build for binary to text conversion using Base64, Base32, Base16 (Hex), and Base128 encoding

## Overview

This project provides efficient Python bindings for multiple encoding formats (Base64, Base32, Base16/Hex, and Base128) using Cython for optimal performance. It enables converting binary data to text representations and vice versa, with both Python and C++ interfaces available.

## Features

- Fast Base64, Base32, Base16 (Hex), and Base128 encoding and decoding using Cython and C++
- Python bindings for easy integration
- Command-line interface for quick conversions
- C++ header-only library for direct integration
- Built with Cython and CMake via scikit-build

## Installation

```bash
pip install bin2text
```

For the latest development version:
```bash
pip install git+https://github.com/mohammadraziei/bin2text.git
```

## Quick Start

### Python API

```python
from bin2text import (
    base64_encode, base64_decode, Base64,
    base32_encode, base32_decode, Base32,
    base16_encode, base16_decode, Base16,
    base128_encode, base128_decode, Base128
)

# Base64 encoding
encoded_b64 = base64_encode("Hello, World!")
decoded_b64 = base64_decode(encoded_b64)
print(f"Base64: {encoded_b64} -> {decoded_b64}")

# Base32 encoding
encoded_b32 = base32_encode("Hello, World!")
decoded_b32 = base32_decode(encoded_b32)
print(f"Base32: {encoded_b32} -> {decoded_b32}")

# Base16 (Hex) encoding
encoded_b16 = base16_encode("Hello, World!")
decoded_b16 = base16_decode(encoded_b16)
print(f"Base16: {encoded_b16} -> {decoded_b16}")

# Base128 encoding
encoded_b128 = base128_encode("Hello, World!")
decoded_b128 = base128_decode(encoded_b128)
print(f"Base128: {encoded_b128} -> {decoded_b128}")

# Using the classes
b64 = Base64()
b32 = Base32()
b16 = Base16()
b128 = Base128()

text = "Hello, World!"
print(f"Original: {text}")
print(f"Base64: {b64.encode(text)} -> {b64.decode(b64.encode(text))}")
print(f"Base32: {b32.encode(text)} -> {b32.decode(b32.encode(text))}")
print(f"Base16: {b16.encode(text)} -> {b16.decode(b16.encode(text))}")
print(f"Base128: {b128.encode(text)} -> {b128.decode(b128.encode(text))}")
```

### Command-line Interface

```bash
# Encode a string using Base64 (default)
python -m bin2text --encode "Hello, World!"

# Decode a Base64 string
python -m bin2text --decode "SGVsbG8sIFdvcmxkIQ=="

# Encode using specific encoding (if implemented in CLI)
python -m bin2text --encode --format base32 "Hello, World!"
```

## Development

Run tests:
```bash
pytest -n auto
```

## License

MIT License