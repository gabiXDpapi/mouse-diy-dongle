"""Native tests of actual vendor HID callbacks and the single-command mailbox."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


class ConfigHidTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gcc'), 'Native GCC required')
    def test_feature_mailbox(self):
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'config-hid.exe'
            result = subprocess.run(['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                                     str(Path(__file__).with_suffix('.c')), '-o', str(exe)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(exe)], check=True, timeout=5)
