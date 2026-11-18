import unittest
from analyze_input_totals import analyze_input


def timing(n):
    lines=['LATENCY_V1 cap=1000000 upper_us=50,100,250,500,1000,2000,4000,8000,overflow']
    for name,value,index in [('candidate_age',50,0),('rf_submit',100,1),('submit_usb',650,4),('rf_usb',750,4),('ack_ready',200,2),('ack_end',250,2)]:
        bins=[0]*9;bins[index]=n
        lines.append(f'LATENCY metric={name} count={n} sum_us_hex={n*value:016x} max_us={value} bins='+','.join(map(str,bins)))
    return '\n'.join(lines+[f'LATENCY_END failed=0 capped=0 tracking_fault=0 reports={n} retries=0 busy=0'])+'\n'


def sample(neutral=False):
    if neutral:
        return timing(1)+'INPUT_V1 reports=1 unchanged=0 buttons=0 neutral=1\nINPUT_SUM dx=0000000000000000 dy=0000000000000000 wheel=0000000000000000\nINPUT_EDGES down=1,0,0,0,0 up=0,0,0,0,0 neutral_up=1,0,0,0,0\nINPUT_END capped=0\n'
    return timing(3)+'INPUT_V1 reports=3 unchanged=1 buttons=0 neutral=0\nINPUT_SUM dx=0000000000000007 dy=fffffffffffffffd wheel=0000000000000001\nINPUT_EDGES down=1,0,0,0,0 up=1,0,0,0,0 neutral_up=0,0,0,0,0\nINPUT_END capped=0\n'


def raw(x=0,y=0,flags=0,data=0,code='0'):
    return dict(source='receiver',dx=str(x),dy=str(y),button_flags=str(flags),button_data=str(data),mouse_flags='0',input_code=code)


class InputAnalysisTests(unittest.TestCase):
    def test_missing_unchanged_record_with_preserved_totals(self):
        result=analyze_input(sample(),[raw(10,-5,0x401,120,'1'),raw(-3,2,2)])
        self.assertTrue(result['aggregate_match'])
        self.assertEqual(result['device_minus_host_records'],1)
        self.assertEqual(result['host']['delivery'],{'1':1,'0':1})

    def test_forced_neutral_is_not_missing_input(self):
        result=analyze_input(sample(True),[raw(flags=1),raw(flags=2)])
        self.assertTrue(result['aggregate_match'])
        self.assertEqual(result['device_minus_host_records'],0)

    def test_motion_or_edge_mismatch_is_reported(self):
        result=analyze_input(sample(),[raw(9,-5,0x401,120),raw(-3,2)])
        self.assertFalse(result['aggregate_match'])
        self.assertEqual(result['differences']['dx'],-1)
        self.assertEqual(result['differences']['up'][0],-1)

    def test_negative_wheel_and_other_device_are_handled(self):
        text=sample().replace('wheel=0000000000000001','wheel=ffffffffffffffff')
        rows=[raw(10,-5,0x401,65416),raw(-3,2,2),dict(source='wired')]
        result=analyze_input(text,rows)
        self.assertTrue(result['aggregate_match'])
        self.assertEqual(result['host']['wheel_raw'],-120)
        self.assertEqual(result['host']['records'],2)

    def test_refuse_bad_or_incomplete_data(self):
        for text in [sample().replace('INPUT_END','PARTIAL'),sample().replace('capped=0\n','capped=1\n'),
                     sample().replace('INPUT_V1 reports=3','INPUT_V1 reports=2'),sample().replace('buttons=0','buttons=1')]:
            with self.subTest(text=text):
                with self.assertRaises(ValueError): analyze_input(text,[raw()])
        bad=raw();bad['mouse_flags']='1'
        with self.assertRaises(ValueError): analyze_input(sample(),[bad])


if __name__ == '__main__':
    unittest.main()
