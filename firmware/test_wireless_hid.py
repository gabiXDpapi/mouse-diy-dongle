"""Exercise the actual USB bridge with asynchronous endpoint/backpressure stubs."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


class WirelessHidTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gcc'), 'Native GCC required for HID bridge harness')
    def test_usb_bridge(self):
        with tempfile.TemporaryDirectory() as directory:
            for diagnostics in (False, True):
                with self.subTest(diagnostics=diagnostics):
                    executable = Path(directory) / f'hid-bridge-{diagnostics}.exe'
                    flags = ['-DLATENCY_DIAGNOSTICS=1'] if diagnostics else []
                    result = subprocess.run(['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                                             *flags, str(Path(__file__).with_suffix('.c')), '-o', str(executable)],
                                            capture_output=True, text=True)
                    self.assertEqual(result.returncode, 0, result.stderr)
                    subprocess.run([str(executable)], check=True, timeout=5)
