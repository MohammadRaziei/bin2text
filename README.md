# bin2text

A Cython project with scikit-build for binary to text conversion using Base64 encoding

## Overview

This project provides efficient Python bindings for Base64 encoding and decoding operations using Cython for optimal performance. It enables converting binary data to text representations and vice versa, with both Python and C++ interfaces available.

## Features

- Fast Base64 encoding and decoding using Cython and C++
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
from bin2text import base64_encode, base64_decode, Base64

# Encode data to Base64
encoded = base64_encode("Hello, World!")
print(f"Encoded: {encoded}")

# Decode Base64 string
decoded = base64_decode(encoded)
print(f"Decoded: {decoded}")

# Using the Base64 class
b64 = Base64()
encoded = b64.encode("Hello, World!")
decoded = b64.decode(encoded)
print(f"Class result: {decoded}")
```

### Command-line Interface

```bash
# Encode a string
python -m bin2text --encode "Hello, World!"

# Decode a Base64 string
python -m bin2text --decode "SGVsbG8sIFdvcmxkIQ=="
```

## Development

Run tests:
```bash
pytest -n auto
```

## License

MIT License