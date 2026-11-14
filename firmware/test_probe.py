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
            executable = Path(directory) / 'probe-test.exe'
            result = subprocess.run(['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                            str(Path(__file__).with_suffix('.c')), '-o', str(executable)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(executable)], check=True, timeout=5)
