/* Diagnostic only: one fixed firmware-version read, never a settings write. */
struct config_read {
    uint8_t offset, command_sequence, last_report, received;
    bool started, have_report, complete, failed;
    unsigned int attempts;
    uint8_t response[64];
};

static bool config_read_frame_valid(const uint8_t *p)
{
    if (p[0] < 2 || p[0] > 64) return false;
    unsigned int at = 3;
    while (at <= p[0]) {
        uint8_t header = p[at++];
        unsigned int length = (header >> 3) & 15u, type = header & 7u;
        if (length != 8 || (type != 1 && type != 5 && type != 7) ||
            at + length > (unsigned int)p[0] + 1) return false;
        at += length;
    }
    return at == (unsigned int)p[0] + 1;
}

static unsigned int config_read_ack(struct config_read *s, const uint8_t *p, uint8_t *ack)
{
    /* Data queue and command stream have independent acknowledgements. */
    ack[0] = 2;
    ack[1] = p[0] == 2 ? p[1] : (uint8_t)(p[1] + 1);
    ack[2] = p[2] & 63u;
    if (s->failed || s->complete) return 3;
    if (!s->started) {
        s->started = true;
        s->command_sequence = ack[2];
    } else if (ack[2] != s->command_sequence) {
        if (s->offset == 64 || ack[2] != (s->command_sequence + 1u) % 63u) {
            s->failed = true;
            return 3;
        }
        s->offset += 8;
        s->command_sequence = ack[2];
    }
    if (s->offset == 64) return 3;
    if (s->attempts == 512) {
        s->failed = true;
        return 3;
    }
    s->attempts++;
    static const uint8_t request[64] = {0, 0, 2, 3, 0, 0x81};
    ack[0] = 12;
    ack[3] = s->offset == 56 ? 0x4f : 0xcf; /* Type7, offset+8 data, continuation. */
    ack[4] = s->offset;
    memcpy(ack + 5, request + s->offset, 8);
    return 13;
}

static void config_read_receive(struct config_read *s, const uint8_t *p)
{
    if (s->failed || s->complete || p[0] == 2) return;
    if (!config_read_frame_valid(p)) { s->failed = true; return; }
    if (s->have_report) {
        if (p[1] == s->last_report) return;
        if (p[1] != (uint8_t)(s->last_report + 1)) { s->failed = true; return; }
    }
    s->last_report = p[1];
    s->have_report = true;
    for (unsigned int at = 3; at <= p[0]; at += 9) {
        uint8_t header = p[at];
        if ((header & 7u) != 7) continue;
        if (s->offset != 64 || s->received > 56) { s->failed = true; return; }
        memcpy(s->response + s->received, p + at + 1, 8);
        s->received += 8;
        if (!(header & 128u)) {
            if (s->received < 10 || s->response[0] != 0xa1 || s->response[2] != 2 ||
                s->response[4] != 0 || s->response[5] != 0x81) s->failed = true;
            else s->complete = true;
            return;
        }
    }
}
