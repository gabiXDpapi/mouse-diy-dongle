"""Exercise the production read-only RF codec without USB or radio hardware."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


class ConfigReadTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gcc'), 'Native GCC required')
    def test_read_query_transport(self):
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'config-read.exe'
            result = subprocess.run(
                ['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                 str(Path(__file__).with_suffix('.c')), '-o', str(exe)],
                capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(exe)], check=True, timeout=5)
