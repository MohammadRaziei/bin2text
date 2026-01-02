import pytest
import base64 as py_base64
from bin2text import Base64, base64_encode, base64_decode


def test_base64_class_encode():
    """Test Base64 class encode method."""
    b64 = Base64()

    # Test encoding string
    result = b64.encode("Hello, World!")
    expected = py_base64.b64encode(b"Hello, World!").decode('utf-8')
    assert result == expected

    # Test encoding bytes
    result = b64.encode_bytes(b"Hello, World!")
    expected = py_base64.b64encode(b"Hello, World!").decode('utf-8')
    assert result == expected

    # Test encoding empty string
    result = b64.encode("")
    expected = py_base64.b64encode(b"").decode('utf-8')
    assert result == expected


def test_base64_class_decode():
    """Test Base64 class decode method."""
    b64 = Base64()

    # Test decoding base64 string
    encoded = py_base64.b64encode(b"Hello, World!").decode('utf-8')
    result = b64.decode(encoded)
    assert result == "Hello, World!"

    # Test decoding to bytes
    result = b64.decode_to_bytes(encoded)
    assert result == b"Hello, World!"

    # Test decoding empty string
    result = b64.decode("")
    expected = py_base64.b64decode(b"").decode('utf-8')
    assert result == expected


def test_base64_function_encode():
    """Test base64_encode function."""
    result = base64_encode("Hello, World!")
    expected = py_base64.b64encode(b"Hello, World!").decode('utf-8')
    assert result == expected


def test_base64_function_decode():
    """Test base64_decode function."""
    encoded = py_base64.b64encode(b"Hello, World!").decode('utf-8')
    result = base64_decode(encoded)
    expected = py_base64.b64decode(encoded.encode('utf-8')).decode('utf-8')
    assert result == expected


def test_base64_roundtrip():
    """Test base64 encoding and decoding roundtrip."""
    original = "The quick brown fox jumps over the lazy dog."

    # Encode then decode
    encoded = base64_encode(original)
    decoded = base64_decode(encoded)

    assert decoded == original


def test_base64_various_inputs():
    """Test base64 with various input types."""
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
        # Test against Python's base64
        encoded = base64_encode(test_case)
        expected_encoded = py_base64.b64encode(test_case.encode('utf-8')).decode('utf-8')
        assert encoded == expected_encoded, f"Encoding failed for: {repr(test_case)}"

        decoded = base64_decode(encoded)
        expected_decoded = py_base64.b64decode(encoded.encode('utf-8')).decode('utf-8')
        assert decoded == expected_decoded, f"Decoding failed for: {repr(encoded)}"


def test_base64_padding():
    """Test base64 padding scenarios."""
    test_cases = [
        ("A", 1),      # Single character (needs 2 padding characters)
        ("AB", 2),     # Two characters (needs 1 padding character)
        ("ABC", 3),    # Three characters (no padding needed)
        ("ABCD", 4),   # Four characters (needs 2 padding characters)
        ("ABCDE", 5),  # Five characters (needs 3 padding characters)
        ("ABCDEF", 6), # Six characters (needs 2 padding characters)
    ]

    for original, length in test_cases:
        encoded = base64_encode(original)
        expected = py_base64.b64encode(original.encode('utf-8')).decode('utf-8')
        assert encoded == expected
        assert base64_decode(encoded) == original


def test_consistency_with_python_base64():
    """Test that our implementation is consistent with Python's base64 module."""
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
        our_encoded = base64_encode(test_str)
        py_encoded = py_base64.b64encode(test_str.encode('utf-8')).decode('utf-8')
        assert our_encoded == py_encoded, f"Encoding mismatch for: {repr(test_str)}"

        # Test decoding
        our_decoded = base64_decode(our_encoded)
        py_decoded = py_base64.b64decode(our_encoded.encode('utf-8')).decode('utf-8')
        assert our_decoded == py_decoded, f"Decoding mismatch for: {repr(our_encoded)}"

        # Test roundtrip
        assert our_decoded == test_str, f"Roundtrip failed for: {repr(test_str)}"


def test_base64_class_vs_function():
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

    b64 = Base64()

    for test_case in test_cases:
        # Compare encode methods
        class_result = b64.encode(test_case)
        func_result = base64_encode(test_case)
        assert class_result == func_result

        # Compare decode methods
        encoded = base64_encode(test_case)
        class_decoded = b64.decode(encoded)
        func_decoded = base64_decode(encoded)
        assert class_decoded == func_decoded


def test_error_handling():
    """Test error handling for invalid base64 strings."""
    # Test with invalid base64 string
    invalid_b64 = "InvalidBase64!"
    with pytest.raises(Exception):
        base64_decode(invalid_b64)


def test_large_data():
    """Test with large data to ensure performance and correctness."""
    # Create a large string
    large_data = "A" * 10000  # 10,000 characters

    # Encode
    encoded = base64_encode(large_data)
    expected_encoded = py_base64.b64encode(large_data.encode('utf-8')).decode('utf-8')
    assert encoded == expected_encoded

    # Decode
    decoded = base64_decode(encoded)
    expected_decoded = py_base64.b64decode(encoded.encode('utf-8')).decode('utf-8')
    assert decoded == expected_decoded

    # Verify roundtrip
    assert decoded == large_data


def test_binary_data():
    """Test with binary data."""
    binary_data = bytes(range(256))  # All possible byte values

    # Encode binary data
    encoded = base64_encode(binary_data)
    expected_encoded = py_base64.b64encode(binary_data).decode('utf-8')
    assert encoded == expected_encoded

    # Decode back
    decoded = base64_decode(encoded)
    expected_decoded = py_base64.b64decode(encoded.encode('utf-8')).decode('utf-8')
    assert decoded == expected_decoded


def test_edge_cases():
    """Test edge cases."""
    # Test with padding characters
    padded_b64 = "SGVsbG8="
    result = base64_decode(padded_b64)
    expected = py_base64.b64decode(padded_b64.encode('utf-8')).decode('utf-8')
    assert result == expected

    # Test with different padding lengths
    test_cases = [
        "QQ==",    # 2 padding chars
        "QUI=",    # 1 padding char
        "QUJD",    # no padding
    ]

    for b64_str in test_cases:
        our_result = base64_decode(b64_str)
        py_result = py_base64.b64decode(b64_str.encode('utf-8')).decode('utf-8')
        assert our_result == py_result