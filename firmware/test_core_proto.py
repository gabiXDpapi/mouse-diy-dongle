"""Native tests of combined v27 CORE proto firmware (Mouse + Config Feature + Aux HID)."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


class CoreProtoTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gcc'), 'Native GCC required')
    def test_core_proto_combined(self):
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'core-proto.exe'
            result = subprocess.run(['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                                     str(Path(__file__).with_suffix('.c')), '-o', str(exe)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(exe)], check=True, timeout=5)
