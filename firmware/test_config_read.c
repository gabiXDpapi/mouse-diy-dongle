#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define assert(c) do { if (!(c)) { fprintf(stderr, "Failed line %d: %s\n", __LINE__, #c); exit(1); } } while (0)
#include "radio_logger/src/config_read.h"

int main(void)
{
    struct config_read state = {0};
    uint8_t idle[65] = {2, 10, 61}, ack[13], body[64] = {0};
    for (unsigned i = 0; i < 8; i++) {
        idle[2] = (uint8_t)((61 + i) % 63);
        assert(config_read_ack(&state, idle, ack) == 13);
        assert(ack[0] == 12 && ack[1] == 10 && ack[2] == idle[2]);
        assert(ack[3] == (i == 7 ? 0x4f : 0xcf) && ack[4] == i * 8);
        memcpy(body + i * 8, ack + 5, 8);
        uint8_t retry[13];
        assert(config_read_ack(&state, idle, retry) == 13 && !memcmp(ack, retry, 13));
    }
    /* Only the exact version read is ever serialized; no arbitrary host command. */
    uint8_t expected[64] = {0, 0, 2, 3, 0, 0x81};
    assert(!memcmp(body, expected, sizeof(body)));
    idle[2] = 6;
    assert(config_read_ack(&state, idle, ack) == 3 && state.offset == 64);
    uint8_t reply[65] = {11, 20, 6, 0xc7, 0xa1, 0, 2, 3, 0, 0x81, 1, 0};
    assert(config_read_frame_valid(reply));
    config_read_receive(&state, reply);
    assert(state.received == 8 && !state.complete && !state.failed);
    config_read_receive(&state, reply); /* Retransmitted report is not appended. */
    assert(state.received == 8);
    reply[1] = 21; reply[3] = 0x47; memset(reply + 4, 0, 8);
    config_read_receive(&state, reply);
    assert(state.complete && !state.failed && state.received == 16);
    assert(config_read_ack(&state, idle, ack) == 3);

    memset(&state, 0, sizeof(state));
    idle[2] = 0;
    config_read_ack(&state, idle, ack);
    idle[2] = 2;
    config_read_ack(&state, idle, ack);
    assert(state.failed); /* A lost/unexpected command sequence cannot skip data. */
    memset(&state, 0, sizeof(state));
    idle[2] = 0;
    for (unsigned i = 0; i < 513; i++) config_read_ack(&state, idle, ack);
    assert(state.failed && state.attempts == 512);

    memset(&state, 0, sizeof(state));
    state.offset = 64;
    reply[1] = 255; reply[3] = 0xc7;
    config_read_receive(&state, reply);
    reply[1] = 1; /* Skipped queue sequence, not a valid wrap. */
    config_read_receive(&state, reply);
    assert(state.failed);
    memset(&state, 0, sizeof(state)); state.offset = 64;
    reply[1] = 0; reply[3] = 0x47;
    config_read_receive(&state, reply);
    assert(state.failed && !state.complete); /* No fabricated success from zeros. */
    reply[0] = 64; assert(!config_read_frame_valid(reply));
    reply[0] = 11; reply[3] = 0x7f; assert(!config_read_frame_valid(reply));
    reply[3] = 0x42; assert(!config_read_frame_valid(reply));
    puts("Read-only RF framing, retry, wrap, malformed and failed-response checks passed.");
    return 0;
}
