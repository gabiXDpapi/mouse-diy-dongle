#include <stdio.h>
#include <stdlib.h>
#define assert(c) do { if (!(c)) { fprintf(stderr,"Assertion failed: %s line%d\n",#c,__LINE__); exit(1); } } while (0)
#include "radio_logger/src/input_totals.h"

int main(void)
{
    input_totals_reset();
    const uint8_t extremes[] = {31,0,0x80,0xff,0x7f,0x80};
    input_totals_record(extremes);
    assert(input_totals.reports == 1 && input_totals.unchanged == 0);
    assert(input_totals.dx == -32768 && input_totals.dy == 32767 && input_totals.wheel == -128);
    const uint8_t held[] = {31,0,0,0,0,0};
    input_totals_record(held);
    assert(input_totals.unchanged == 1);
    const uint8_t reverse[] = {10,0xff,0x7f,0,0x80,0x7f};
    input_totals_record(reverse);
    assert(input_totals.dx == -1 && input_totals.dy == -1 && input_totals.wheel == -1);
    input_totals_neutral();
    assert(input_totals.neutral_reports == 1 && input_totals.buttons == 0);
    for (unsigned int i = 0; i < 5; i++) {
        assert(input_totals.down[i] == 1);
        assert(input_totals.up[i] + input_totals.neutral_up[i] == 1);
        assert(input_totals.neutral_up[i] == (unsigned int)((10u >> i) & 1));
    }
    input_totals_reset();
    const uint8_t zero[6] = {0};
    input_totals_record(zero);
    assert(input_totals.unchanged == 1 && !input_totals_capped);
    const uint8_t motion[] = {0,0xff,0x7f,0,0x80,0x7f};
    for (unsigned int i = 0; i < 70000; i++) input_totals_record(motion);
    assert(input_totals.dx == (int64_t)70000 * 32767);
    assert(input_totals.dy == (int64_t)70000 * -32768);
    int64_t before = input_totals.dx;
    input_totals.reports = INPUT_TOTALS_CAP;
    input_totals_record(motion);
    assert(input_totals_capped && input_totals.reports == INPUT_TOTALS_CAP && input_totals.dx == before);
    input_totals_reset();
    assert(!input_totals_capped && input_totals.reports == 0 && input_totals.down[0] == 0);
    return 0;
}
