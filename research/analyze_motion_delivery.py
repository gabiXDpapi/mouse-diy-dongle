"""Predefined foreground host movement epoch; not bus/sensor/end-to-end timing."""
import argparse
import csv
import json
import math
from pathlib import Path
import re
import statistics

FIELDS={'time_us','source','dx','dy','button_flags','button_data','mouse_flags','input_code'}


def analyze_motion(text, rows, source='receiver'):
    if source not in ['receiver','wired']:
        raise ValueError('Unknown target source')
    if text.count('RAW_INPUT_READY ') != 1 or text.count('TIMED_MOTION_READY countdown_s=5 move_s=30 rest_s=5') != 1 or 'MOTION_ABORT' in text:
        raise ValueError('Missing timed mode or aborted foreground epoch')
    markers=re.findall(r'^MOTION_(COUNTDOWN|START|STOP|DONE) time_us=(\d+)\r?$',text,re.M)
    if [m[0] for m in markers] != ['COUNTDOWN','START','STOP','DONE']:
        raise ValueError('Missing, duplicate or unordered motion markers')
    countdown,start,stop,done=[int(m[1]) for m in markers]
    if not (countdown>=0 and start-countdown>=5000000 and stop-start>=30000000 and done-stop>=5000000 and done<=1200000000):
        raise ValueError('Motion phase durations/order invalid')
    footer=re.findall(r'^RAW_INPUT_DONE records=(\d+) read_errors=(\d+) failed=(\d+)\r?$',text,re.M)
    if len(footer)!=1 or footer[0][1:]!=('0','0'):
        raise ValueError('Raw recording incomplete or contains errors')
    rows=list(rows)
    if len(rows)!=int(footer[0][0]):
        raise ValueError('CSV/footer record-count mismatch')
    epoch_start,epoch_end=start+5000000,start+25000000
    times=[];counts=[0]*20;stationary=0;no_coalesce=0;previous=-1;selected=0
    for row in rows:
        if set(row)!=FIELDS or row['source'] not in ['receiver','wired']:
            raise ValueError('Malformed CSV fields/source')
        try:
            stamp,x,y,flags,data,mouse_flags,code=[int(row[k]) for k in ['time_us','dx','dy','button_flags','button_data','mouse_flags','input_code']]
        except (ValueError,TypeError) as error:
            raise ValueError('Malformed CSV values') from error
        if not (0<=stamp<=1200000000 and previous<=stamp and -2**31<=x<2**31 and -2**31<=y<2**31 and
                0<=flags<=65535 and 0<=data<=65535 and 0<=mouse_flags<=65535 and code in [0,1]):
            raise ValueError('Invalid timestamp/value range')
        previous=stamp
        if row['source']!=source or not epoch_start<=stamp<epoch_end:
            continue
        if code!=0 or mouse_flags&1 or flags:
            raise ValueError('Measured epoch has background, absolute or control input')
        selected+=1
        no_coalesce+=bool(mouse_flags&8)
        if not x and not y:
            stationary+=1
            continue
        times.append(stamp)
        counts[(stamp-epoch_start)//1000000]+=1
    if len(times)<2:
        raise ValueError('Insufficient movement in the predefined epoch')
    gaps=sorted(b-a for a,b in zip(times,times[1:]))
    return dict(source=source,epoch_us=[epoch_start,epoch_end],duration_seconds=20,
                movement_records=len(times),receiver_or_wired_records=selected,stationary_records=stationary,
                movement_hz=len(times)/20,events_per_second=counts,min_second_hz=min(counts),max_second_hz=max(counts),
                gaps_us=dict(median=statistics.median(gaps),p95=gaps[math.ceil(.95*len(gaps))-1],
                             p99=gaps[math.ceil(.99*len(gaps))-1],maximum=max(gaps),
                             over_2000=sum(g>2000 for g in gaps),zero=sum(g==0 for g in gaps)),
                no_coalesce_records=no_coalesce,
                scope='All central20s of a predeclared30s foreground MOVE period. Host WM_INPUT delivery only; queueing/coalescing, zero-motion suppression and hand motion affect gaps. Not measured USB token cadence, fresh sensor sampling, RF loss or physical gaming latency.')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('stdout_file',type=Path)
    parser.add_argument('csv_file',type=Path)
    parser.add_argument('--source',choices=['receiver','wired'],default='receiver')
    args=parser.parse_args()
    with args.csv_file.open(newline='') as stream:
        result=analyze_motion(args.stdout_file.read_text(),csv.DictReader(stream),args.source)
    print(json.dumps(result,indent=2))
