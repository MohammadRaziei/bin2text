"""Command-line interface for bin2text."""

import argparse
from bin2text import base64_encode, base64_decode


def main():
    """Simple command-line tool for bin2text."""
    parser = argparse.ArgumentParser(description='Base64 encoding/decoding tool')
    parser.add_argument('--encode', '-e', type=str, help='Encode a string to base64')
    parser.add_argument('--decode', '-d', type=str, help='Decode a base64 string')

    args = parser.parse_args()

    if args.encode:
        result = base64_encode(args.encode)
        print(f"Encoded: {result}")
    elif args.decode:
        result = base64_decode(args.decode)
        print(f"Decoded: {result}")
    else:
        parser.print_help()


if __name__ == "__main__":
    main()