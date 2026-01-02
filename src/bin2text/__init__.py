"""A Cython project with scikit-build"""

__version__ = "0.1.0"

# Import the compiled Cython module
from .base64 import Base64, base64_encode, base64_decode

__all__ = ["Base64", "base64_encode", "base64_decode"]