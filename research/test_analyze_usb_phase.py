import unittest
from test_analyze_input_totals import sample,raw
from analyze_usb_phase import analyze_phase

def phase_frame(invalid=False):
    lines=['PHASE_V1 width_us=100 bins=10 calibrated=1']
    for i in range(11):
        n=3 if i==(10 if invalid else 2) else 0
        bins=[0]*9;bins[4]=n
        lines.append(f'PHASE bin={i} count={n} rf_submit_hex={n*100:016x} rf_usb_hex={n*750:016x} max_us={750 if n else 0} hist='+','.join(map(str,bins)))
    return '\n'.join(lines+[f'PHASE_END reports=3 invalid={3 if invalid else 0}'])+'\n'

def rows():return [raw(10,-5,0x401,120),raw(-3,2,2)]

class PhaseAnalysisTests(unittest.TestCase):
    def test_valid_phase_and_invalid_coverage(self):
        for invalid in [False,True]:
            result=analyze_phase(sample()+phase_frame(invalid),rows())
            self.assertTrue(result['input']['aggregate_match'])
            bucket=result['phase'][10 if invalid else 2]
            self.assertEqual(bucket['count'],3)
            self.assertEqual(bucket['mean_rf_usb_us'],750)
            self.assertEqual(bucket['mean_submit_usb_us'],650)
            self.assertEqual(result['invalid'],3 if invalid else 0)
    def test_reject_incomplete_or_inconsistent_phase(self):
        original=sample()+phase_frame()
        for text in [original.replace('PHASE_END','PARTIAL'),original.replace('calibrated=1','calibrated=0'),
                     original.replace('bin=3 count=0','bin=2 count=0'),original.replace('PHASE_END reports=3','PHASE_END reports=4'),
                     original.replace('invalid=0','invalid=1'),original.replace('rf_usb_hex=00000000000008ca','rf_usb_hex=00000000000008cb'),
                     original.replace('bin=0 count=0','bin=0 count=1'),original.replace('width_us=100','width_us=200'),
                     original.replace('rf_submit_hex=000000000000012c','rf_submit_hex=000000000000012d')]:
            with self.subTest(text=text):
                with self.assertRaises(ValueError):analyze_phase(text,rows())
    def test_input_mismatch_is_preserved(self):
        result=analyze_phase(sample()+phase_frame(),[raw()])
        self.assertFalse(result['input']['aggregate_match'])

    def test_partial_dump_then_complete_retry(self):
        text=sample()+'PHASE_V1 width_us=100 bins=10 calibrated=1\nPHASE bin=0 partial\n'+sample()+phase_frame()
        self.assertTrue(analyze_phase(text,rows())['input']['aggregate_match'])
