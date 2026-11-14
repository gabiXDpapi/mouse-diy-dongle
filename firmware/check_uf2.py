"""Check application-only UF2 blocks for the recorded S140-v6 bootloader layout."""

import hashlib
from pathlib import Path
import struct
import sys


def check(data):
    if not data or len(data) % 512:
        raise ValueError("UF2 must contain complete 512-byte blocks")
    ranges = []
    count = len(data) // 512
    for index in range(count):
        block = data[index * 512:(index + 1) * 512]
        m0, m1, flags, address, size, number, total, family = struct.unpack_from("<8I", block)
        if (m0, m1, struct.unpack_from("<I", block, 508)[0]) != (
            0x0A324655, 0x9E5D5157, 0x0AB16F30
        ):
            raise ValueError("Invalid UF2 magic")
        if flags != 0x2000 or family != 0xADA52840:
            raise ValueError("Expected plain nRF52840 application blocks")
        if number != index or total != count:
            raise ValueError("Missing, reordered, or duplicated UF2 blocks")
        if not (0 < size <= 476 and 0x26000 <= address < address + size <= 0xEC000):
            raise ValueError("Payload outside application partition or invalid size")
        if ranges and address < ranges[-1][1]:
            raise ValueError("Overlapping or unordered flash writes")
        ranges.append((address, address + size))
    if ranges[0][0] != 0x26000:
        raise ValueError("Application does not start at 0x26000")
    return ranges[0][0], ranges[-1][1]


if __name__ == "__main__":
    data = Path(sys.argv[1]).read_bytes()
    start, end = check(data)
    print(f"PASS: {len(data) // 512} blocks, 0x{start:X}..0x{end:X} (end exclusive)")
    print(f"SHA256: {hashlib.sha256(data).hexdigest()}")
