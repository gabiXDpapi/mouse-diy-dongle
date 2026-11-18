"""Exercise the actual fixed-memory latency collector with deterministic clocks."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "research"))
from analyze_usb_phase import analyze_phase


class LatencyTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gcc'), 'Native GCC required')
    def test_latency_collector(self):
        with tempfile.TemporaryDirectory() as directory:
            for conservation, phase in ((False, False), (True, False), (True, True)):
                with self.subTest(conservation=conservation, phase=phase):
                    executable = Path(directory) / f'latency-{conservation}-{phase}.exe'
                    flags = ['-DINPUT_CONSERVATION=1'] if conservation else []
                    if phase: flags += ['-DUSB_PHASE_DIAGNOSTICS=1']
                    result = subprocess.run(['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                                             *flags, str(Path(__file__).with_suffix('.c')), '-o', str(executable)],
                                            capture_output=True, text=True)
                    self.assertEqual(result.returncode, 0, result.stderr)
                    native = subprocess.run([str(executable)], check=True, timeout=5, capture_output=True, text=True)
                    if phase:
                        rows = [dict(source='receiver',dx='7',dy='-3',button_flags='1024',button_data='120',mouse_flags='0',input_code='0')]
                        parsed = analyze_phase(native.stdout, rows)
                        self.assertTrue(parsed['input']['aggregate_match'])
                        self.assertEqual(parsed['phase'][1]['mean_rf_usb_us'],750)
                        self.assertEqual(parsed['invalid'],0)
