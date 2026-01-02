"""Command-line interface for bin2text."""

import argparse
from bin2text import (
    base64_encode, base64_decode,
    base32_encode, base32_decode,
    base16_encode, base16_decode,
    base128_encode, base128_decode
)


def main():
    """Command-line tool for bin2text with multiple encoding formats."""
    parser = argparse.ArgumentParser(description='Binary to text encoding/decoding tool')
    parser.add_argument('--encode', '-e', type=str, help='Encode a string')
    parser.add_argument('--decode', '-d', type=str, help='Decode an encoded string')
    parser.add_argument('--format', '-f', type=str, choices=['base64', 'base32', 'base16', 'base128'],
                        default='base64', help='Encoding format (default: base64)')

    args = parser.parse_args()

    if args.encode:
        if args.format == 'base64':
            result = base64_encode(args.encode)
        elif args.format == 'base32':
            result = base32_encode(args.encode)
        elif args.format == 'base16':
            result = base16_encode(args.encode)
        elif args.format == 'base128':
            result = base128_encode(args.encode)
        print(f"Encoded ({args.format}): {result}")
    elif args.decode:
        if args.format == 'base64':
            result = base64_decode(args.decode)
        elif args.format == 'base32':
            result = base32_decode(args.decode)
        elif args.format == 'base16':
            result = base16_decode(args.decode)
        elif args.format == 'base128':
            result = base128_decode(args.decode)
        print(f"Decoded ({args.format}): {result}")
    else:
        parser.print_help()


if __name__ == "__main__":
    main()