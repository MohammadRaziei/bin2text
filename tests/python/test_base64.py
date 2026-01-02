import pytest
from bin2text import Base64, base64_encode, base64_decode


def test_base64_class_encode():
    """Test Base64 class encode method."""
    b64 = Base64()
    
    # Test encoding string
    result = b64.encode("Hello, World!")
    assert result == "SGVsbG8sIFdvcmxkIQ=="
    
    # Test encoding bytes
    result = b64.encode_bytes(b"Hello, World!")
    assert result == "SGVsbG8sIFdvcmxkIQ=="
    
    # Test encoding empty string
    result = b64.encode("")
    assert result == ""


def test_base64_class_decode():
    """Test Base64 class decode method."""
    b64 = Base64()
    
    # Test decoding base64 string
    result = b64.decode("SGVsbG8sIFdvcmxkIQ==")
    assert result == "Hello, World!"
    
    # Test decoding to bytes
    result = b64.decode_to_bytes("SGVsbG8sIFdvcmxkIQ==")
    assert result == b"Hello, World!"
    
    # Test decoding empty string
    result = b64.decode("")
    assert result == ""


def test_base64_function_encode():
    """Test base64_encode function."""
    result = base64_encode("Hello, World!")
    assert result == "SGVsbG8sIFdvcmxkIQ=="


def test_base64_function_decode():
    """Test base64_decode function."""
    result = base64_decode("SGVsbG8sIFdvcmxkIQ==")
    assert result == "Hello, World!"


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
        "Multi-byte characters: àáâãäåæçèé"
    ]
    
    for test_case in test_cases:
        encoded = base64_encode(test_case)
        decoded = base64_decode(encoded)
        assert decoded == test_case


def test_base64_padding():
    """Test base64 padding scenarios."""
    # Single character (needs 2 padding characters)
    encoded = base64_encode("A")
    assert encoded == "QQ=="
    assert base64_decode(encoded) == "A"
    
    # Two characters (needs 1 padding character)
    encoded = base64_encode("AB")
    assert encoded == "QUI="
    assert base64_decode(encoded) == "AB"
    
    # Three characters (no padding needed)
    encoded = base64_encode("ABC")
    assert encoded == "QUJD"
    assert base64_decode(encoded) == "ABC"
    
    # Four characters (needs 2 padding characters)
    encoded = base64_encode("ABCD")
    assert encoded == "QUJDRA=="
    assert base64_decode(encoded) == "ABCD"