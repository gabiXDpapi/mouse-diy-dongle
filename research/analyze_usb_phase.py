"""Validate hardware SOF phase accounting; this is not physical end-to-end latency."""
import argparse
import csv
import json
from pathlib import Path
import re
from analyze_latency import analyze, analyze_histogram
from analyze_input_totals import analyze_input


def analyze_phase(text, rows):
    timing = analyze(text)
    content = analyze_input(text, rows)
    frames = re.findall(r'PHASE_V1 [^\n]+\n(?:PHASE bin=[^\n]+\n){11}PHASE_END [^\n]+', text)
    if not frames:
        raise ValueError('No complete phase frame')
    lines = frames[-1].splitlines()
    if lines[0] != 'PHASE_V1 width_us=100 bins=10 calibrated=1' or len(lines) != 13:
        raise ValueError('Unknown, uncalibrated or incomplete phase schema')
    footer = re.fullmatch(r'PHASE_END reports=(\d+) invalid=(\d+)', lines[-1])
    if not footer or int(footer[1]) != timing['reports']:
        raise ValueError('Phase report coverage mismatch')
    phase = []
    for index, line in enumerate(lines[1:-1]):
        match = re.fullmatch(r'PHASE bin=(\d+) count=(\d+) rf_submit_hex=([0-9a-f]{16}) rf_usb_hex=([0-9a-f]{16}) max_us=(\d+) hist=([\d,]+)', line)
        if not match or int(match[1]) != index:
            raise ValueError('Malformed, duplicate or unordered phase bin')
        count, submit, total, maximum = int(match[2]), int(match[3], 16), int(match[4], 16), int(match[5])
        bins = list(map(int, match[6].split(',')))
        if submit > total or len(bins) != 9:
            raise ValueError('Invalid phase timing components')
        if count:
            histogram = analyze_histogram(count, total, maximum, bins)
        else:
            if submit or total or maximum or any(bins):
                raise ValueError('Nonzero empty phase bucket')
            histogram = dict(count=0, sum_us=0, max_us=0, bins=bins)
        phase.append(dict(bin=index, phase_us=[index*100,index*100+99] if index<10 else None,
                          **histogram, rf_submit_sum_us=submit,
                          mean_rf_submit_us=submit/count if count else None,
                          mean_rf_usb_us=total/count if count else None,
                          mean_submit_usb_us=(total-submit)/count if count else None))
    rf_usb = timing['metrics']['rf_usb']
    if (sum(p['count'] for p in phase) != timing['reports'] or
        phase[10]['count'] != int(footer[2]) or
        sum(p['sum_us'] for p in phase) != rf_usb['sum_us'] or
        sum(p['rf_submit_sum_us'] for p in phase) != timing['metrics']['rf_submit']['sum_us'] or
        max(p['max_us'] for p in phase) != rf_usb['max_us'] or
        [sum(p['bins'][i] for p in phase) for i in range(9)] != rf_usb['bins']):
        raise ValueError('Phase totals contradict complete timing frame')
    return dict(input=content, timing=timing, phase=phase, invalid=int(footer[2]),
                scope='Hardware SOF phase at software submission timestamp; invalid/stale/boundary samples are separate. USB callbacks include driver work-queue dispatch. One instrumented run does not prove an optimization, sustained polling or physical end-to-end latency.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('serial_file', type=Path)
    parser.add_argument('raw_file', type=Path)
    args = parser.parse_args()
    with args.raw_file.open(newline='') as stream:
        result = analyze_phase(args.serial_file.read_text(), csv.DictReader(stream))
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result['input']['aggregate_match'] else 2)
