import unittest
from analyze_latency import analyze


def frame():
    lines = ['LATENCY_V1 cap=1000000 upper_us=50,100,250,500,1000,2000,4000,8000,overflow']
    for name, value, bin_index in [('candidate_age', 50, 0), ('rf_submit', 100, 1),
                                   ('submit_usb', 650, 4), ('rf_usb', 750, 4),
                                   ('ack_ready', 200, 2), ('ack_end', 250, 2)]:
        bins = [0] * 9
        bins[bin_index] = 1
        lines.append(f'LATENCY metric={name} count=1 sum_us_hex={value:016x} max_us={value} bins='+','.join(map(str,bins)))
    return '\n'.join(lines + ['LATENCY_END failed=0 capped=0 tracking_fault=0 reports=1 retries=0 busy=0'])


class AnalysisTests(unittest.TestCase):
    def test_valid_frame_and_quantile_bounds(self):
        result = analyze(frame())
        self.assertEqual(result['metrics']['rf_usb']['mean_us'], 750)
        self.assertEqual(result['metrics']['rf_usb']['p99_upper_us'], 750)

    def test_refuse_corruption_or_partial_collection(self):
        for changed in [frame().replace('LATENCY_END', 'PARTIAL'),
                        frame().replace('tracking_fault=0', 'tracking_fault=1'),
                        frame().replace('capped=0', 'capped=1'),
                        frame().replace('reports=1', 'reports=2'),
                        frame().replace('00000000000002ee', '00000000000002ef'),
                        frame().replace('bins=1,0,0,0,0,0,0,0,0', 'bins=2,0,0,0,0,0,0,0,0')]:
            with self.subTest(text=changed):
                with self.assertRaises(ValueError):
                    analyze(changed)


if __name__ == '__main__':
    unittest.main()
