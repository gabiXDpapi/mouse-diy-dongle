"""Validate a stopped-only device timing frame; quantiles are histogram bounds."""
import argparse
import json
import math
from pathlib import Path
import re

UPPER = [50, 100, 250, 500, 1000, 2000, 4000, 8000]
NAMES = {"candidate_age", "rf_submit", "submit_usb", "rf_usb", "ack_ready", "ack_end"}


def analyze_histogram(count, total, maximum, bins):
    if len(bins) != 9 or sum(bins) != count or not 0 < count <= 1000000:
        raise ValueError("Invalid histogram coverage")
    if not 0 <= maximum <= 0xffffffff or total > count * maximum:
        raise ValueError("Invalid maximum or sum")
    last_bin = next((i for i, upper in enumerate(UPPER) if maximum <= upper), 8)
    if last_bin != max(i for i, n in enumerate(bins) if n):
        raise ValueError("Maximum contradicts histogram")
    lower = [0] + [x + 1 for x in UPPER]
    if total < sum(n * lo for n, lo in zip(bins, lower)):
        raise ValueError("Sum below histogram minimum")
    if total > sum(n * hi for n, hi in zip(bins, UPPER + [maximum])):
        raise ValueError("Sum above histogram maximum")
    result = dict(count=count, sum_us=total, mean_us=total / count, max_us=maximum, bins=bins)
    for label, p in [('p50_upper_us', .50), ('p95_upper_us', .95), ('p99_upper_us', .99)]:
        rank, cumulative = math.ceil(count * p), 0
        for i, n in enumerate(bins):
            cumulative += n
            if cumulative >= rank:
                result[label] = min((UPPER + [maximum])[i], maximum)
                break
    return result


def analyze(text):
    frames = re.findall(r"LATENCY_V1 [^\n]+\n.*?LATENCY_END [^\n]+", text, re.S)
    if not frames:
        raise ValueError("No complete device timing frame")
    lines = frames[-1].splitlines()
    if lines[0] != "LATENCY_V1 cap=1000000 upper_us=50,100,250,500,1000,2000,4000,8000,overflow":
        raise ValueError("Unknown frame schema")
    footer = re.fullmatch(r"LATENCY_END failed=(\d+) capped=(\d+) tracking_fault=(\d+) reports=(\d+) retries=(\d+) busy=(\d+)", lines[-1])
    if not footer or any(int(x) for x in footer.groups()[:3]):
        raise ValueError("Failed, capped, corrupt or incomplete timing collection")
    reports, retries, busy = map(int, footer.groups()[3:])
    metrics = {}
    for line in lines[1:-1]:
        m = re.fullmatch(r"LATENCY metric=(\w+) count=(\d+) sum_us_hex=([0-9a-f]{16}) max_us=(\d+) bins=([\d,]+)", line)
        if not m:
            raise ValueError("Malformed metric")
        name, count, total, maximum, bins = m.groups()
        count, total, maximum = int(count), int(total, 16), int(maximum)
        bins = list(map(int, bins.split(',')))
        if name not in NAMES or name in metrics or len(bins) != 9 or sum(bins) != count or not 0 < count <= 1000000:
            raise ValueError("Missing, duplicate, empty or inconsistent metric")
        result = analyze_histogram(count, total, maximum, bins)
        metrics[name] = result
    if set(metrics) != NAMES:
        raise ValueError("Incomplete metric set")
    if any(metrics[n]['count'] != reports for n in ['rf_submit', 'submit_usb', 'rf_usb']):
        raise ValueError("USB sample coverage does not match accepted input")
    if metrics['rf_usb']['sum_us'] != metrics['rf_submit']['sum_us'] + metrics['submit_usb']['sum_us']:
        raise ValueError("Radio/USB time components do not reconcile")
    acks = metrics['ack_end']['count']
    if metrics['ack_ready']['count'] != acks or metrics['candidate_age']['count'] != acks + busy or reports + retries > acks:
        raise ValueError("RF candidate/ACK/retry/busy coverage does not reconcile")
    if metrics['ack_end']['sum_us'] < metrics['ack_ready']['sum_us']:
        raise ValueError("ACK end precedes ready")
    return dict(metrics=metrics, reports=reports, retries=retries, busy=busy,
                scope="Instrumented RF END to submission/USB completion callback; excludes sensor, prior retries/queueing, OS/game/display. Quantiles are upper bounds, not exact percentiles.")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('serial_file', type=Path)
    args = parser.parse_args()
    print(json.dumps(analyze(args.serial_file.read_text()), indent=2))
