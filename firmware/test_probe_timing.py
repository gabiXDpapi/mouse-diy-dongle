"""Verify actual timer/PPI initialization, cadence and cleanup with fake hardware."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest

class ProbeTimingTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gcc'), 'Native GCC required')
    def test_phase_hardware_resources(self):
        with tempfile.TemporaryDirectory() as directory:
            Path(directory,'nrfx_ppi.h').write_text('')
            exe=Path(directory,'probe-timing.exe')
            result=subprocess.run(['gcc','-std=c11','-Wall','-Wextra','-Werror','-Wno-unused-variable','-Wno-unused-parameter','-DUSB_PHASE_DIAGNOSTICS=1','-I',directory,str(Path(__file__).with_suffix('.c')),'-o',str(exe)],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            subprocess.run([str(exe)],check=True,timeout=5)

            for forbidden in ['CONFIG_NRFX_TIMER3','CONFIG_USB_DEVICE_SOF','FAKE_TIMER_OWNED']:
                rejected=subprocess.run(['gcc','-std=c11','-DUSB_PHASE_DIAGNOSTICS=1','-D'+forbidden+'=1','-I',directory,str(Path(__file__).with_suffix('.c')),'-o',str(exe)],capture_output=True,text=True)
                self.assertNotEqual(rejected.returncode,0,forbidden)
                self.assertIn('static assertion failed',rejected.stderr)
