"""Compare accepted device totals with receiver Raw Input; aggregate equality is not ordering proof."""
import argparse
import csv
import json
from pathlib import Path
import re
from analyze_latency import analyze


def signed_hex(value):
    number = int(value, 16)
    return number - (1 << 64) if number & (1 << 63) else number


def analyze_input(text, rows):
    timing = analyze(text)
    frames = re.findall(r'INPUT_V1 [^\n]+\nINPUT_SUM [^\n]+\nINPUT_EDGES [^\n]+\nINPUT_END capped=\d+', text)
    if not frames:
        raise ValueError('No complete input-total frame')
    lines = frames[-1].splitlines()
    header = re.fullmatch(r'INPUT_V1 reports=(\d+) unchanged=(\d+) buttons=(\d+) neutral=(\d+)', lines[0])
    sums = re.fullmatch(r'INPUT_SUM dx=([0-9a-f]{16}) dy=([0-9a-f]{16}) wheel=([0-9a-f]{16})', lines[1])
    edges = re.fullmatch(r'INPUT_EDGES down=([\d,]+) up=([\d,]+) neutral_up=([\d,]+)', lines[2])
    if not header or not sums or not edges or lines[3] != 'INPUT_END capped=0':
        raise ValueError('Malformed or capped input totals')
    reports, unchanged, buttons, neutral = map(int, header.groups())
    if reports != timing['reports'] or buttons != 0 or not 0 <= unchanged <= reports or not 0 <= neutral <= reports:
        raise ValueError('Input coverage/final button state contradicts stopped timing frame')
    dx, dy, wheel = map(signed_hex, sums.groups())
    if any(not -32768 * reports <= v <= 32767 * reports for v in [dx, dy]) or not -128 * reports <= wheel <= 127 * reports:
        raise ValueError('Input sum exceeds report range')
    down, up, neutral_up = [list(map(int, group.split(','))) for group in edges.groups()]
    if any(len(group) != 5 for group in [down, up, neutral_up]):
        raise ValueError('Expected five buttons')
    if any(d > reports or u > reports or n > neutral or d != u + n for d, u, n in zip(down, up, neutral_up)):
        raise ValueError('Button edges do not reconcile with neutral stopped state')
    host = dict(records=0, dx=0, dy=0, wheel_raw=0, down=[0]*5, up=[0]*5, delivery={})
    for row in rows:
        if row.get('source') != 'receiver':
            continue
        try:
            x, y, flags, data, mouse_flags = [int(row[name]) for name in ['dx','dy','button_flags','button_data','mouse_flags']]
            delivery = row['input_code']
        except (KeyError, TypeError, ValueError) as error:
            raise ValueError('Malformed receiver record') from error
        if not (-2**31 <= x < 2**31 and -2**31 <= y < 2**31 and 0 <= flags <= 0x7ff and
                0 <= data <= 65535 and 0 <= mouse_flags <= 65535 and not mouse_flags & 1 and delivery in ['0','1']):
            raise ValueError('Unsupported/invalid receiver input record')
        host['records'] += 1
        host['delivery'][delivery] = host['delivery'].get(delivery, 0) + 1
        host['dx'] += x
        host['dy'] += y
        if flags & 0x400:
            host['wheel_raw'] += data - 65536 if data >= 32768 else data
        for i in range(5):
            host['down'][i] += bool(flags & (1 << (2*i)))
            host['up'][i] += bool(flags & (2 << (2*i)))
    if not host['records']:
        raise ValueError('No receiver records')
    expected = dict(dx=dx, dy=dy, wheel_raw=wheel*120, down=down, up=[u+n for u,n in zip(up, neutral_up)])
    differences = {key: host[key]-expected[key] for key in ['dx','dy','wheel_raw']}
    differences.update({key:[h-e for h,e in zip(host[key],expected[key])] for key in ['down','up']})
    match = not any(differences[k] for k in ['dx','dy','wheel_raw']) and not any(differences['down']+differences['up'])
    return dict(aggregate_match=match, device=dict(reports=reports, unchanged=unchanged, neutral=neutral, **expected),
                host=host, differences=differences, expected_usb_reports=reports+neutral,
                device_minus_host_records=reports+neutral-host['records'],
                scope='Both foreground/background receiver records included; wheel120 units per HID step. Equal aggregate totals do not prove event ordering, timing, individual-event preservation or the cause of any count difference.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('serial_file', type=Path)
    parser.add_argument('raw_file', type=Path)
    args = parser.parse_args()
    with args.raw_file.open(newline='') as stream:
        result = analyze_input(args.serial_file.read_text(), csv.DictReader(stream))
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result['aggregate_match'] else 2)
