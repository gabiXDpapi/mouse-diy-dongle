import struct
import unittest

from check_uf2 import check


def block(address=0x26000, family=0xADA52840):
    data = bytearray(512)
    struct.pack_into("<8I", data, 0, 0x0A324655, 0x9E5D5157,
                     0x2000, address, 256, 0, 1, family)
    struct.pack_into("<I", data, 508, 0x0AB16F30)
    return data


class Uf2Checks(unittest.TestCase):
    def test_accepts_application(self):
        self.assertEqual(check(block()), (0x26000, 0x26100))

    def test_rejects_reserved_regions_and_wrong_chip(self):
        for data in (block(0), block(0x25000), block(0xEBF80),
                     block(0xF4000), block(family=0), block()[:-1], b""):
            with self.subTest(data=data[:32]):
                with self.assertRaises(ValueError):
                    check(data)

    def test_rejects_corruption_and_missing_blocks(self):
        for offset, value in ((0, 0), (8, 0), (16, 477), (20, 1), (24, 2), (508, 0)):
            data = block()
            struct.pack_into("<I", data, offset, value)
            with self.subTest(offset=offset):
                with self.assertRaises(ValueError):
                    check(data)


if __name__ == "__main__":
    unittest.main()
