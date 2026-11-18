"""Exercise the actual fixed-memory latency collector with deterministic clocks."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


class LatencyTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gcc'), 'Native GCC required')
    def test_latency_collector(self):
        with tempfile.TemporaryDirectory() as directory:
            for conservation in (False, True):
                with self.subTest(conservation=conservation):
                    executable = Path(directory) / f'latency-{conservation}.exe'
                    flags = ['-DINPUT_CONSERVATION=1'] if conservation else []
                    result = subprocess.run(['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                                             *flags, str(Path(__file__).with_suffix('.c')), '-o', str(executable)],
                                            capture_output=True, text=True)
                    self.assertEqual(result.returncode, 0, result.stderr)
                    subprocess.run([str(executable)], check=True, timeout=5)
