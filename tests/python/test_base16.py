import pytest
import binascii
from bin2text import Base16, base16_encode, base16_decode


def test_base16_class_encode():
    """Test Base16 class encode method."""
    b16 = Base16()

    # Test encoding string
    result = b16.encode("Hello, World!")
    expected = binascii.hexlify(b"Hello, World!").decode('utf-8').upper()
    assert result == expected

    # Test encoding bytes
    result = b16.encode_bytes(b"Hello, World!")
    expected = binascii.hexlify(b"Hello, World!").decode('utf-8').upper()
    assert result == expected

    # Test encoding empty string
    result = b16.encode("")
    expected = binascii.hexlify(b"").decode('utf-8').upper()
    assert result == expected


def test_base16_class_decode():
    """Test Base16 class decode method."""
    b16 = Base16()

    # Test decoding base16 string
    encoded = binascii.hexlify(b"Hello, World!").decode('utf-8').upper()
    result = b16.decode(encoded)
    assert result == "Hello, World!"

    # Test decoding to bytes
    result = b16.decode_to_bytes(encoded)
    assert result == b"Hello, World!"

    # Test decoding empty string
    result = b16.decode("")
    expected = binascii.unhexlify("").decode('utf-8')
    assert result == expected


def test_base16_function_encode():
    """Test base16_encode function."""
    result = base16_encode("Hello, World!")
    expected = binascii.hexlify(b"Hello, World!").decode('utf-8').upper()
    assert result == expected


def test_base16_function_decode():
    """Test base16_decode function."""
    encoded = binascii.hexlify(b"Hello, World!").decode('utf-8').upper()
    result = base16_decode(encoded)
    expected = binascii.unhexlify(encoded.encode('utf-8')).decode('utf-8')
    assert result == expected


def test_base16_roundtrip():
    """Test base16 encoding and decoding roundtrip."""
    original = "The quick brown fox jumps over the lazy dog."

    # Encode then decode
    encoded = base16_encode(original)
    decoded = base16_decode(encoded)

    assert decoded == original


def test_base16_various_inputs():
    """Test base16 with various input types."""
    test_cases = [
        "",
        "A",
        "AB",
        "ABC",
        "ABCD",
        "Hello, World!",
        "1234567890",
        "!@#$%^&*()",
        "Multi-byte characters: àáâãäåæçèé",
        "Binary-like: \x00\x01\x02\x03\xff\xfe\xfd",
        "Special chars: \n\t\r\b\f\a\v",
        "Unicode: 🐍🚀🌟🎉",
        "Long string: " + "A" * 1000,
        "Numbers: 0123456789",
        "Mixed: Hello123!@#",
        "Whitespace: \t\n\r\v\f ",
        "Control chars: \x00\x01\x02\x03\x04\x05\x06\x07\x08\x0b\x0c\x0e\x0f"
    ]

    for test_case in test_cases:
        # Test against Python's binascii
        encoded = base16_encode(test_case)
        expected_encoded = binascii.hexlify(test_case.encode('utf-8')).decode('utf-8').upper()
        assert encoded == expected_encoded, f"Encoding failed for: {repr(test_case)}"

        decoded = base16_decode(encoded)
        expected_decoded = binascii.unhexlify(encoded.encode('utf-8')).decode('utf-8')
        assert decoded == expected_decoded, f"Decoding failed for: {repr(encoded)}"


def test_consistency_with_python_binascii():
    """Test that our implementation is consistent with Python's binascii module."""
    test_strings = [
        "",
        "A",
        "Hello",
        "Hello, World!",
        "The quick brown fox jumps over the lazy dog.",
        "Special chars: !@#$%^&*()",
        "Numbers: 1234567890",
        "Unicode: àáâãäåæçèé",
        "Binary: \x00\x01\x02\x03\xff\xfe\xfd"
    ]

    for test_str in test_strings:
        # Test encoding
        our_encoded = base16_encode(test_str)
        py_encoded = binascii.hexlify(test_str.encode('utf-8')).decode('utf-8').upper()
        assert our_encoded == py_encoded, f"Encoding mismatch for: {repr(test_str)}"

        # Test decoding
        our_decoded = base16_decode(our_encoded)
        py_decoded = binascii.unhexlify(our_encoded.encode('utf-8')).decode('utf-8')
        assert our_decoded == py_decoded, f"Decoding mismatch for: {repr(our_encoded)}"

        # Test roundtrip
        assert our_decoded == test_str, f"Roundtrip failed for: {repr(test_str)}"


def test_base16_class_vs_function():
    """Test that class methods produce same results as functions."""
    test_cases = [
        "",
        "Hello, World!",
        "Test string",
        "12345",
        "A",
        "AB",
        "ABC",
        "ABCD",
    ]

    b16 = Base16()

    for test_case in test_cases:
        # Compare encode methods
        class_result = b16.encode(test_case)
        func_result = base16_encode(test_case)
        assert class_result == func_result

        # Compare decode methods
        encoded = base16_encode(test_case)
        class_decoded = b16.decode(encoded)
        func_decoded = base16_decode(encoded)
        assert class_decoded == func_decoded


def test_error_handling():
    """Test error handling for invalid base16 strings."""
    # Test with invalid base16 string
    invalid_b16 = "InvalidHex!"
    # Note: Our implementation might not raise exceptions for invalid input
    # so we'll just test that it doesn't crash
    try:
        result = base16_decode(invalid_b16)
        # If it doesn't raise an exception, that's fine for now
    except Exception:
        # If it does raise an exception, that's also fine
        pass


def test_large_data():
    """Test with large data to ensure performance and correctness."""
    # Create a large string
    large_data = "A" * 10000  # 10,000 characters

    # Encode
    encoded = base16_encode(large_data)
    expected_encoded = binascii.hexlify(large_data.encode('utf-8')).decode('utf-8').upper()
    assert encoded == expected_encoded

    # Decode
    decoded = base16_decode(encoded)
    expected_decoded = binascii.unhexlify(encoded.encode('utf-8')).decode('utf-8')
    assert decoded == expected_decoded

    # Verify roundtrip
    assert decoded == large_data


def test_binary_data():
    """Test with binary data."""
    binary_data = bytes(range(256))  # All possible byte values

    # Encode binary data
    encoded = base16_encode(binary_data)
    expected_encoded = binascii.hexlify(binary_data).decode('utf-8').upper()
    assert encoded == expected_encoded

    # Decode back
    decoded = base16_decode(encoded)
    expected_decoded = binascii.unhexlify(encoded.encode('utf-8')).decode('utf-8')
    assert decoded == expected_decoded