# distutils: language = c++
# cython: language_level=3

"""
A Cython project with scikit-build that includes base16 (hex) functionality
"""

from libc.stdlib cimport malloc, free
from libc.string cimport strcpy, strlen
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool
import sys


# Import the base16 functionality from the header-only library
cdef extern from "base16.h" namespace "b2t":
    void base16_encode(string& out, string const& buf) except +
    void base16_encode(string& out, const unsigned char* buf, size_t bufLen) except +
    void base16_encode(string& out, vector[unsigned char] const& buf) except +
    void base16_decode(vector[unsigned char]& out, string const& encoded_string) except +
    void base16_decode(string& out, string const& encoded_string) except +


cdef class Base16:
    """A base16 (hex) encoding/decoding class implemented in Cython."""

    def encode(self, data):
        """Encode data to base16 (hex) string."""
        cdef string result
        cdef string input_str

        if isinstance(data, str):
            # Convert Python string to bytes if needed
            input_str = data.encode('utf-8')
        elif isinstance(data, bytes):
            input_str = data
        else:
            # Convert other types to string then to bytes
            input_str = str(data).encode('utf-8')

        base16_encode(result, input_str)
        return result.decode('utf-8')

    def encode_bytes(self, bytes data):
        """Encode bytes to base16 (hex) string."""
        cdef string result
        cdef string input_str = data

        base16_encode(result, input_str)
        return result.decode('utf-8')

    def decode(self, str encoded_str):
        """Decode base16 (hex) string to bytes."""
        cdef string result
        cdef string input_str = encoded_str.encode('utf-8')

        base16_decode(result, input_str)
        return result.decode('utf-8')

    def decode_to_bytes(self, str encoded_str):
        """Decode base16 (hex) string to bytes."""
        cdef vector[unsigned char] result
        cdef string input_str = encoded_str.encode('utf-8')

        base16_decode(result, input_str)

        # Convert vector to bytes
        cdef bytes py_bytes = b""
        for i in range(result.size()):
            py_bytes += bytes([<unsigned char>result[i]])
        return py_bytes


def base16_encode(data):
    """Encode data to base16 (hex) string (convenience function)."""
    cdef Base16 b16 = Base16()
    return b16.encode(data)


def base16_decode(encoded_str):
    """Decode base16 (hex) string to bytes (convenience function)."""
    cdef Base16 b16 = Base16()
    return b16.decode(encoded_str)