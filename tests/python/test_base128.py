import pytest
from bin2text import Base128, base128_encode, base128_decode


def test_base128_class_encode():
    """Test Base128 class encode method."""
    b128 = Base128()

    # Test encoding string
    result = b128.encode("Hello, World!")
    # For Base128, we can't compare with a standard library, so we test roundtrip
    decoded_result = b128.decode(result)
    assert decoded_result == "Hello, World!"

    # Test encoding bytes
    result = b128.encode_bytes(b"Hello, World!")
    decoded_result = b128.decode(result)
    assert decoded_result == "Hello, World!"

    # Test encoding empty string
    result = b128.encode("")
    decoded_result = b128.decode(result)
    assert decoded_result == ""


def test_base128_class_decode():
    """Test Base128 class decode method."""
    b128 = Base128()

    # Test decoding base128 string (roundtrip test)
    original = "Hello, World!"
    encoded = b128.encode(original)
    result = b128.decode(encoded)
    assert result == original

    # Test decoding to bytes
    result_bytes = b128.decode_to_bytes(encoded)
    assert result_bytes == b"Hello, World!"

    # Test decoding empty string
    result = b128.decode("")
    assert result == ""


def test_base128_function_encode():
    """Test base128_encode function."""
    result = base128_encode("Hello, World!")
    decoded_result = base128_decode(result)
    assert decoded_result == "Hello, World!"


def test_base128_function_decode():
    """Test base128_decode function."""
    encoded = base128_encode("Hello, World!")
    result = base128_decode(encoded)
    assert result == "Hello, World!"


def test_base128_roundtrip():
    """Test base128 encoding and decoding roundtrip."""
    original = "The quick brown fox jumps over the lazy dog."

    # Encode then decode
    encoded = base128_encode(original)
    decoded = base128_decode(encoded)

    assert decoded == original


def test_base128_various_inputs():
    """Test base128 with various input types."""
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
        # Test roundtrip
        encoded = base128_encode(test_case)
        decoded = base128_decode(encoded)
        assert decoded == test_case, f"Roundtrip failed for: {repr(test_case)}"


def test_base128_class_vs_function():
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

    b128 = Base128()

    for test_case in test_cases:
        # Compare encode methods
        class_result = b128.encode(test_case)
        func_result = base128_encode(test_case)
        decoded_class = b128.decode(class_result)
        decoded_func = base128_decode(func_result)
        assert decoded_class == decoded_func
        assert decoded_class == test_case

        # Compare decode methods
        encoded = base128_encode(test_case)
        class_decoded = b128.decode(encoded)
        func_decoded = base128_decode(encoded)
        assert class_decoded == func_decoded
        assert class_decoded == test_case


def test_large_data():
    """Test with large data to ensure performance and correctness."""
    # Create a large string
    large_data = "A" * 10000  # 10,000 characters

    # Encode then decode
    encoded = base128_encode(large_data)
    decoded = base128_decode(encoded)

    # Verify roundtrip
    assert decoded == large_data


def test_binary_data():
    """Test with binary data."""
    binary_data = bytes(range(256))  # All possible byte values

    # Encode then decode
    encoded = base128_encode(binary_data)
    decoded = base128_decode(encoded)

    # Verify roundtrip
    assert decoded.encode('latin1') == binary_data  # Convert back to bytes for comparison