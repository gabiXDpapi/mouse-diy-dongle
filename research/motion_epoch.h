#pragma once
enum motion_phase { MOTION_WAIT, MOTION_COUNTDOWN, MOTION_MOVE, MOTION_REST, MOTION_DONE, MOTION_ABORT };
struct motion_epoch { enum motion_phase phase; long long since_us, start_us, stop_us; };
static void motion_tick(struct motion_epoch *state, long long now_us, int foreground)
{
    if(state->phase == MOTION_WAIT && foreground) {
        state->phase = MOTION_COUNTDOWN; state->since_us = now_us;
    } else if((state->phase == MOTION_COUNTDOWN || state->phase == MOTION_MOVE) && !foreground) {
        state->phase = MOTION_ABORT;
    } else if(state->phase == MOTION_COUNTDOWN && now_us-state->since_us >= 5000000) {
        state->phase = MOTION_MOVE; state->since_us = state->start_us = now_us;
    } else if(state->phase == MOTION_MOVE && now_us-state->since_us >= 30000000) {
        state->phase = MOTION_REST; state->since_us = state->stop_us = now_us;
    } else if(state->phase == MOTION_REST && now_us-state->since_us >= 5000000) {
        state->phase = MOTION_DONE;
    }
}
