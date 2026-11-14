import hashlib
import unittest
from unittest.mock import patch

from generate_radio_header import make_header, mouse_base


class RadioEvidenceTests(unittest.TestCase):
    def test_discovery_and_data_channels_are_both_scanned(self):
        image = bytearray(93344)
        image[0x1A950 - 0x6000:0x1A953 - 0x6000] = bytes([3, 49, 78])
        image[0x1A966 - 0x6000:0x1A976 - 0x6000] = bytes(
            [51, 27, 76, 4, 48, 24, 73, 8, 52, 26, 77, 2, 47, 22, 72, 7])
        with patch('generate_radio_header.IMAGE_SHA256', hashlib.sha256(image).hexdigest()):
            header = make_header(image, dict(device="258A:2018", interface="MI_02",
                                            address_bytes="12:34:56:78"))
        self.assertIn('channels[] = {3, 49, 78, 51, 27, 76, 4, 48, 24, 73, 8, 52, 26, 77, 2, 47, 22, 72, 7}', header)

    def test_byte_order(self):
        self.assertEqual(mouse_base(dict(device="258A:2018", interface="MI_02",
                                         address_bytes="12:34:56:78")), 0x78563412)

    def test_invalid_evidence(self):
        for address in ("", "12:34:56", "GG:34:56:78", "00:00:00:00", "FF:FF:FF:FF", 123):
            with self.subTest(address=address), self.assertRaises(ValueError):
                mouse_base(dict(device="258A:2018", interface="MI_02", address_bytes=address))
        with self.assertRaises(ValueError):
            mouse_base(dict(device="258A:2031", interface="MI_02", address_bytes="12:34:56:78"))
        with self.assertRaises(ValueError):
            make_header(bytes(93344), {})
