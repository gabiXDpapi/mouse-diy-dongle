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
    assert(!state.reply_released);
    config_read_receive(&state, reply);
    assert(!state.reply_released); /* Repeated final packet still needs its ACK. */
    uint8_t released[] = {2, 22, 6};
    config_read_receive(&state, released);
    assert(state.reply_released);
    assert(config_read_ack(&state, idle, ack) == 3);

    uint8_t request[64] = {0,0,2,1,1,0x82,1};
    assert(config_request_valid(request) && config_read_begin(&state,request));
    request[63]=1; assert(!config_request_valid(request)); request[63]=0;
    request[6]=2; assert(config_request_valid(request));
    request[6]=4; assert(!config_request_valid(request)); request[6]=1;
    request[3]=2; request[5]=2; request[7]=4;
    assert(config_request_valid(request));
    request[7]=5; assert(!config_request_valid(request));
    request[3]=1; request[5]=0x80; request[6]=request[7]=0;
    assert(config_read_begin(&state,request));
    for (unsigned i=0;i<8;i++) { idle[2]=(uint8_t)i; assert(config_read_ack(&state,idle,ack)==13); }
    idle[2]=8; config_read_ack(&state,idle,ack);
    uint8_t poll_reply[65] = {11,255,8,0x47,0xa1,0,2,1,1,0x80,1,0};
    config_read_receive(&state,poll_reply);
    assert(state.complete && !state.failed && state.received==8);
    released[1]=0; config_read_receive(&state,released);
    assert(state.reply_released); /* Data queue wrap is independent of command sequence. */
    uint8_t select[64]={0,0,2,2,1,2,1,3};
    assert(config_read_begin(&state,select));
    for (unsigned i=0;i<8;i++) { idle[2]=(uint8_t)i; config_read_ack(&state,idle,ack); }
    idle[2]=8; config_read_ack(&state,idle,ack);
    uint8_t selected_reply[65]={11,42,8,0x47,0xa1,0,2,2,1,2,1,3};
    config_read_receive(&state,selected_reply);
    assert(state.complete && !state.failed && state.response[7]==3);
    assert(config_read_begin(&state,select)); state.offset=64;
    selected_reply[11]=2; config_read_receive(&state,selected_reply);
    assert(state.failed && !state.complete); /* An old/other selection is not an acknowledgement. */
    const uint8_t buttons[]={1,3,2,5,4,0x14,0x10,0x11};
    for (unsigned profile=1;profile<=3;profile++) {
        uint8_t fields[][64]={{0,0,2,1,1,0x81},{0,0,2,1,1,0x82},
                             {0,0,2,19,2,0x81},{0,0,2,2,0,0x88}};
        for (unsigned i=0;i<4;i++) { fields[i][6]=(uint8_t)profile; assert(config_request_valid(fields[i])); }
        for (unsigned i=0;i<sizeof(buttons);i++) {
            uint8_t binding[64]={0,0,2,10,3,0x80};binding[6]=(uint8_t)profile;binding[7]=buttons[i];
            assert(config_request_valid(binding));
            binding[8]=1; assert(!config_request_valid(binding)); /* Only the startup-touched slot0. */
        }
    }
    uint8_t bad_binding[64]={0,0,2,10,3,0x80,1,6};
    assert(!config_request_valid(bad_binding));
    uint8_t lod[64]={0,0,2,1,1,0x88};assert(config_request_valid(lod));
    select[6]=2;assert(!config_request_valid(select)); /* No new profile write authority. */

    uint8_t values[64]={0,0,2,18,1,1,1,4};
    for (unsigned i=0;i<8;i++) { values[8+2*i]=3; values[9+2*i]=0x20; } /* 800 DPI. */
    assert(config_read_begin(&state,values));
    values[6]=2;assert(!config_request_valid(values));values[6]=1;
    values[7]=3;assert(!config_request_valid(values));values[7]=4;
    values[24]=1;assert(!config_request_valid(values));values[24]=0;
    values[8]=0;values[9]=49;assert(!config_request_valid(values));
    values[9]=51;assert(!config_request_valid(values));
    values[8]=0x4a;values[9]=0x6a;assert(!config_request_valid(values)); /* 19050 */
    values[8]=0;values[9]=50;assert(config_request_valid(values));
    values[8]=0x4a;values[9]=0x38;assert(config_request_valid(values)); /* 19000 */
    values[8]=3;values[9]=0x20;
    assert(config_read_begin(&state,values));state.offset=64;
    uint8_t values_response[24];memcpy(values_response,values,24);values_response[0]=0xa1;
    for(unsigned i=0;i<3;i++) {
        uint8_t frame[12]={11,(uint8_t)(60+i),0, i==2 ? 0x47 : 0xc7};
        memcpy(frame+4,values_response+8*i,8);config_read_receive(&state,frame);
    }
    assert(state.complete && !state.failed);
    assert(config_read_begin(&state,values));state.offset=64;values_response[23]^=1;
    for(unsigned i=0;i<3;i++) {
        uint8_t frame[12]={11,(uint8_t)(60+i),0, i==2 ? 0x47 : 0xc7};
        memcpy(frame+4,values_response+8*i,8);config_read_receive(&state,frame);
    }
    assert(state.failed && !state.complete); /* Every returned value must match the write. */

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
    puts("Allowlisted RF framing, retry, wrap, malformed and failed-response checks passed.");
    return 0;
}
