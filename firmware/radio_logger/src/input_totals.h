#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define INPUT_TOTALS_CAP 1000000u
/* Main-thread only: accepted input, neutralization, reset and stopped-only dump. */
static struct {
    uint32_t reports, unchanged, neutral_reports;
    int64_t dx, dy, wheel;
    uint8_t buttons;
    uint32_t down[5], up[5], neutral_up[5];
} input_totals;
static bool input_totals_capped;

static void input_totals_reset(void)
{
    memset(&input_totals, 0, sizeof(input_totals));
    input_totals_capped = false;
}

static void input_totals_record(const uint8_t report[6])
{
    if (input_totals.reports == INPUT_TOTALS_CAP) { input_totals_capped = true; return; }
    uint32_t x = report[1] | ((uint32_t)report[2] << 8);
    uint32_t y = report[3] | ((uint32_t)report[4] << 8);
    int32_t dx = x >= 0x8000 ? (int32_t)x - 0x10000 : (int32_t)x;
    int32_t dy = y >= 0x8000 ? (int32_t)y - 0x10000 : (int32_t)y;
    int32_t wheel = report[5] >= 0x80 ? (int32_t)report[5] - 0x100 : report[5];
    uint8_t buttons = report[0] & 31;
    if (!dx && !dy && !wheel && buttons == input_totals.buttons) input_totals.unchanged++;
    for (unsigned int i = 0; i < 5; i++) {
        uint8_t bit = 1u << i;
        if ((buttons & bit) && !(input_totals.buttons & bit)) input_totals.down[i]++;
        if (!(buttons & bit) && (input_totals.buttons & bit)) input_totals.up[i]++;
    }
    input_totals.reports++;
    input_totals.dx += dx;
    input_totals.dy += dy;
    input_totals.wheel += wheel;
    input_totals.buttons = buttons;
}

static void input_totals_neutral(void)
{
    if (input_totals.neutral_reports == INPUT_TOTALS_CAP) { input_totals_capped = true; return; }
    input_totals.neutral_reports++;
    for (unsigned int i = 0; i < 5; i++) {
        if (input_totals.buttons & (1u << i)) input_totals.neutral_up[i]++;
    }
    input_totals.buttons = 0;
}
