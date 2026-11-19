"""Exercise the actual C probe guards and timeout path with fake registers."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


class ProbeTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gcc'), 'Native GCC required for probe harness')
    def test_probe_guards_and_timeout(self):
        with tempfile.TemporaryDirectory() as directory:
            for mode in ('probe', 'receiver', 'dongle', 'config-read', 'config-hid'):
                with self.subTest(mode=mode):
                    executable = Path(directory) / f'{mode}-test.exe'
                    flags = ['-DWIRELESS_HID=1'] if mode != 'probe' else []
                    if mode in ('dongle', 'config-read', 'config-hid'):
                        flags += ['-DNORMAL_DONGLE=1']
                    if mode in ('config-read', 'config-hid'):
                        flags += ['-DRF_CONFIG_READ_DIAGNOSTICS=1']
                    if mode == 'config-hid':
                        flags += ['-DRF_CONFIG_HID_DIAGNOSTICS=1']
                    result = subprocess.run(['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                                    *flags, str(Path(__file__).with_suffix('.c')), '-o', str(executable)],
                                            capture_output=True, text=True)
                    self.assertEqual(result.returncode, 0, result.stderr)
                    subprocess.run([str(executable)], check=True, timeout=5)
