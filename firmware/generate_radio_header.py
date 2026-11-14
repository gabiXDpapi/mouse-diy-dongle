"""Build the logger's private parameters from the inspected firmware and USB observation."""

import hashlib
import json
from pathlib import Path
import re
import struct
import sys

IMAGE_SHA256 = "965c8f8cc17e912d0ede5f0ed82b4c12a15d21fae931aeaf59921c94794e1738"


def mouse_base(observation):
    if observation.get("device") != "258A:2018" or observation.get("interface") != "MI_02":
        raise ValueError("Expected the recorded Series One PRO vendor interface")
    address = observation.get("address_bytes", "")
    if not isinstance(address, str) or not re.fullmatch(r"[0-9A-Fa-f]{2}(?::[0-9A-Fa-f]{2}){3}", address):
        raise ValueError("Expected exactly four address bytes")
    value = int.from_bytes(bytes.fromhex(address.replace(":", "")), "little")
    if value in (0, 0xFFFFFFFF):
        raise ValueError("Refusing an empty/default address")
    return value


def make_header(image, observation):
    if hashlib.sha256(image).hexdigest() != IMAGE_SHA256:
        raise ValueError("Firmware differs from the inspected snapshot; re-analyze before building")
    def word(address):
        return struct.unpack_from("<I", image, address - 0x6000)[0]
    # State 4's observed 0x20 packet uses a separate three-channel discovery table.
    channels = (image[0x1A950 - 0x6000:0x1A953 - 0x6000]
                + image[0x1A966 - 0x6000:0x1A976 - 0x6000])
    if len(set(channels)) != 19 or any(channel > 100 for channel in channels):
        raise ValueError("Invalid observed channel table")
    return (
        "/* Generated from locally inspected evidence; do not commit device-specific values. */\n"
        f"#define MOUSE_BASE 0x{mouse_base(observation):08X}u\n"
        f"#define OBSERVED_BASE0 0x{word(0x15B64):08X}u\n"
        f"#define OBSERVED_PREFIX0 0x{word(0x15B5C):08X}u\n"
        f"#define OBSERVED_PCNF1 0x{word(0x15B68):08X}u\n"
        f"#define OBSERVED_CRCPOLY 0x{word(0x15B6C):08X}u\n"
        "static const uint8_t channels[] = {" + ", ".join(map(str, channels)) + "};\n"
    )


if __name__ == "__main__":
    evidence = Path(sys.argv[1])
    header = make_header(
        (evidence / "mouse-app-6000-1cc9f.bin").read_bytes(),
        json.loads((evidence / "mouse-address-observation.json").read_text(encoding="utf-8-sig")),
    )
    Path(sys.argv[2]).write_text(header, encoding="ascii")
    print("Radio evidence hash and private address validated; header generated.")
