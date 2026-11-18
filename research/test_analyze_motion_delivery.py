import unittest
from analyze_motion_delivery import analyze_motion


def row(t, source='receiver', flags=0, code=0, mouse_flags=0, x=1, y=0):
    return {k:str(v) for k,v in dict(time_us=t,source=source,dx=x,dy=y,button_flags=flags,button_data=0,mouse_flags=mouse_flags,input_code=code).items()}


def markers(n):
    return ('RAW_INPUT_READY start_utc=2026-11-18T10:00:00.000Z; test\n'
            'TIMED_MOTION_READY countdown_s=5 move_s=30 rest_s=5\n'
            'MOTION_COUNTDOWN time_us=0\nMOTION_START time_us=5000000\n'
            'MOTION_STOP time_us=35000000\nMOTION_DONE time_us=40000000\n'
            f'RAW_INPUT_DONE records={n} read_errors=0 failed=0\n')


class MotionDeliveryTests(unittest.TestCase):
    def test_exact_1khz_epoch_and_fixed_boundaries(self):
        rows=[row(10000000+i*1000) for i in range(20000)]+[row(30000000),row(9000000,flags=1),row(12000000,source='wired')]
        rows.sort(key=lambda r:int(r['time_us']))
        result=analyze_motion(markers(len(rows)),rows)
        self.assertEqual(result['movement_hz'],1000)
        self.assertEqual(result['events_per_second'],[1000]*20)
        self.assertEqual(result['gaps_us']['p99'],1000)
        self.assertEqual(result['epoch_us'],[10000000,30000000])
    def test_slow_or_zero_motion_reports_are_not_falsely_1khz(self):
        rows=[row(10000000+i*5000) for i in range(4000)]
        result=analyze_motion(markers(len(rows)),rows)
        self.assertEqual(result['movement_hz'],200)
        self.assertEqual(result['gaps_us']['over_2000'],3999)
        rows=[row(10000000+i*1000,x=0) for i in range(100)]
        with self.assertRaises(ValueError):analyze_motion(markers(len(rows)),rows)
    def test_reject_bad_markers_capture_and_record_count(self):
        text=markers(2);rows=[row(10000000),row(10001000)]
        for changed in [text.replace('MOTION_DONE','PARTIAL'),text.replace('read_errors=0','read_errors=1'),
                        text.replace('failed=0','failed=1'),text.replace('records=2','records=3'),
                        text+'MOTION_ABORT time_us=10000001\n',text.replace('35000000','30000000'),text.replace('RAW_INPUT_READY ','NO_READY '),
                        text.replace('MOTION_START time_us=5000000','MOTION_START time_us=5000000\nMOTION_START time_us=5000001')]:
            with self.subTest(text=changed):
                with self.assertRaises(ValueError):analyze_motion(changed,rows)
    def test_reject_epoch_background_absolute_and_controls(self):
        for bad in [row(10000000,code=1),row(10000000,mouse_flags=1),row(10000000,flags=1),row(10000000,flags=1024)]:
            with self.subTest(row=bad):
                with self.assertRaises(ValueError):analyze_motion(markers(2),[bad,row(10001000)])
    def test_reject_partial_or_backwards_records(self):
        bad=row(10000000);bad['dy']=''
        for rows in [[bad,row(10001000)],[row(10001000),row(10000000)],[row(-1),row(10000000)]]:
            with self.assertRaises(ValueError):analyze_motion(markers(2),rows)
    def test_zero_gaps_and_no_coalesce_flags_are_reported(self):
        rows=[row(10000000,mouse_flags=8),row(10000000),row(10002001)]
        result=analyze_motion(markers(3),rows)
        self.assertEqual(result['gaps_us']['zero'],1)
        self.assertEqual(result['no_coalesce_records'],1)

    def test_wired_target_does_not_mix_receiver_events(self):
        rows=[row(10000000,source='wired'),row(10000000),row(10001000,source='wired'),row(10001000),row(10002000,source='wired')]
        result=analyze_motion(markers(len(rows)),rows,'wired')
        self.assertEqual(result['source'],'wired')
        self.assertEqual(result['movement_records'],3)
        self.assertEqual(result['movement_hz'],.15)
