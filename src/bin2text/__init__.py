"""A Cython project with scikit-build"""

__version__ = "0.1.0"

# Import the compiled Cython modules
from .base64 import Base64, base64_encode, base64_decode
from .base32 import Base32, base32_encode, base32_decode
from .base16 import Base16, base16_encode, base16_decode
from .base128 import Base128, base128_encode, base128_decode

__all__ = [
    "Base64", "base64_encode", "base64_decode",
    "Base32", "base32_encode", "base32_decode",
    "Base16", "base16_encode", "base16_decode",
    "Base128", "base128_encode", "base128_decode"
]