/* Prototype configuration allowlist; no address, updater or arbitrary writes. */
static const uint8_t config_read_version_request[64] = {0, 0, 2, 3, 0, 0x81};
#ifdef CORE_PROTO_FULL
static const uint8_t config_dongle_version_request[64] = {0, 0, 0, 3, 0, 0x81};
#endif
struct config_read {
    uint8_t offset, command_sequence, last_report, received;
    bool started, have_report, complete, failed, reply_released, configured;
    unsigned int attempts;
    uint8_t response[64];
    uint8_t request[64];
};

static inline bool config_request_valid(const uint8_t *p)
{
    if (!memcmp(p, config_read_version_request, 64)) return true;
#ifdef CORE_PROTO_FULL
    /* Receiver-local firmware version (target 0, group 0, op 0x81, any payload size <= 58). */
    if (p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] <= 58 && p[4] == 0 && p[5] == 0x81) {
        for (unsigned int i = 6; i < 64; i++) if (p[i] != 0) return false;
        return true;
    }
    /* Safe read-only getters (op >= 0x80) in groups 0..3 (excluding 00/82 RF address).
     * Allows flexible request size byte p[3] used by third-party WebHID tools like OpenMouse.
     */
    if (p[0] == 0 && p[1] == 0 && p[2] == 2 && p[3] <= 58 && p[4] <= 3 && (p[5] & 0x80u)) {
        if (p[4] == 0 && p[5] == 0x82) return false; /* Never expose RF address read. */
        return true;
    }
#endif
    uint8_t expected[64] = {0,0,2,1,0,0,0};
    expected[4] = p[4]; expected[5] = p[5];
    bool profile_read = false;
    switch (((unsigned int)p[4] << 8) | p[5]) {
    case 0x0085: case 0x0180: case 0x0188: break; /* Global profile/poll/LOD getters. */
#ifdef CORE_PROTO_FULL
    case 0x0083: expected[3] = 2; break; /* Battery status getter. */
    case 0x0005: /* Active profile setter (1..3). */
        if (p[6] < 1 || p[6] > 3) return false;
        expected[3] = 1; expected[6] = p[6];
        break;
    case 0x0008: /* Profile debounce setter (profile 1..3, 0..16 ms). */
        if (p[6] < 1 || p[6] > 3 || p[7] > 16) return false;
        expected[3] = 2; expected[6] = p[6]; expected[7] = p[7];
        break;
    case 0x0100: /* Polling divider setter (1/2/4/8 = 1000/500/250/125 Hz). */
        if (p[6] != 1 && p[6] != 2 && p[6] != 4 && p[6] != 8) return false;
        expected[3] = 1; expected[6] = p[6];
        break;
    case 0x0108: /* Lift-off distance setter (1 or 2 mm). */
        if (p[6] != 1 && p[6] != 2) return false;
        expected[3] = 1; expected[6] = p[6];
        break;
    case 0x0201: /* Six DPI-indicator RGB entries per profile (1..3). */
        if (p[6] < 1 || p[6] > 3) return false;
        expected[3] = 19; expected[6] = p[6];
        memcpy(expected + 7, p + 7, 18);
        break;
    case 0x0300: { /* Slot-0 button binding setter (profile 1..3, 8 known buttons). */
        static const uint8_t buttons[] = {1,3,2,5,4,0x14,0x10,0x11};
        bool found = false;
        for (unsigned int i = 0; i < sizeof(buttons); i++) if (p[7] == buttons[i]) found = true;
        if (!found || p[6] < 1 || p[6] > 3 || p[8] != 0) return false;
        expected[3] = 10; expected[6] = p[6]; expected[7] = p[7]; expected[8] = 0;
        memcpy(expected + 9, p + 9, 7);
        break;
    }
#endif
    case 0x0181: case 0x0182: profile_read = true; break;
    case 0x0281: expected[3] = 19; profile_read = true; break; /* Six DPI colours. */
    case 0x0088: expected[3] = 2; profile_read = true; break; /* Profile debounce. */
    case 0x0380: {
        static const uint8_t buttons[] = {1,3,2,5,4,0x14,0x10,0x11};
        bool found = false;
        for (unsigned int i = 0; i < sizeof(buttons); i++) if (p[7] == buttons[i]) found = true;
        if (!found) return false;
        expected[3] = 10; expected[7] = p[7]; profile_read = true;
        break; /* Slot0 only; remaining padding must be zero. */
    }
    case 0x0101:
#ifdef CORE_PROTO_FULL
        if (p[6] < 1 || p[6] > 3 || p[7] < 1 || p[7] > 6) return false;
        expected[3] = (uint8_t)(p[7] * 4u + 2u); expected[6] = p[6]; expected[7] = p[7];
        for (unsigned int at = 8; at < 8u + (unsigned int)p[7] * 4u; at += 2) {
            unsigned int dpi = (unsigned int)p[at] * 256u + p[at + 1];
            if (dpi < 50 || dpi > 19000 || dpi % 50) return false;
            expected[at] = p[at]; expected[at + 1] = p[at + 1];
        }
#else
        /* Qualify profile1 with its existing four stages before enabling CORE writes. */
        if (p[6] != 1 || p[7] != 4) return false;
        expected[3] = 18; expected[6] = 1; expected[7] = 4;
        for (unsigned int at = 8; at < 24; at += 2) {
            unsigned int dpi = (unsigned int)p[at] * 256u + p[at + 1];
            if (dpi < 50 || dpi > 19000 || dpi % 50) return false;
            expected[at] = p[at]; expected[at + 1] = p[at + 1];
        }
#endif
        break;
    case 0x0102:
#ifdef CORE_PROTO_FULL
        if (p[6] < 1 || p[6] > 3 || p[7] < 1 || p[7] > 6) return false;
        expected[3] = 2; expected[6] = p[6]; expected[7] = p[7];
#else
        /* shortcut: current profile has four stages; qualify other profiles separately. */
        if (p[7] < 1 || p[7] > 4) return false;
        expected[3] = 2; expected[6] = 1; expected[7] = p[7];
#endif
        break;
    default: return false;
    }
    if (profile_read) {
        if (p[6] < 1 || p[6] > 3) return false;
        expected[6] = p[6];
    }
    return !memcmp(p, expected, 64);
}

static inline bool config_read_begin(struct config_read *s, const uint8_t *request)
{
    if (!config_request_valid(request)) return false;
    memset(s, 0, sizeof(*s));
    memcpy(s->request, request, 64);
    s->configured = true;
    return true;
}

static bool config_read_frame_valid(const uint8_t *p)
{
    if (p[0] < 2 || p[0] > 64) return false;
    unsigned int at = 3;
    while (at <= p[0]) {
        uint8_t header = p[at++];
        unsigned int length = (header >> 3) & 15u, type = header & 7u;
#ifdef CORE_PROTO_FULL
        bool cont = (header & 0x80u) != 0;
        bool ok = (!cont && ((type == 1 && length == 8) || (type == 2 && length == 8) ||
                             (type == 3 && length == 2) || (type == 4 && length == 1) ||
                             (type == 5 && length == 8) || (type == 6 && length == 8))) ||
                  (type == 7 && length == 8);
        if (!ok || at + length > (unsigned int)p[0] + 1) return false;
#else
        if (length != 8 || (type != 1 && type != 5 && type != 7) ||
            at + length > (unsigned int)p[0] + 1) return false;
#endif
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
    ack[0] = 12;
    ack[3] = s->offset == 56 ? 0x4f : 0xcf; /* Type7, offset+8 data, continuation. */
    ack[4] = s->offset;
    const uint8_t *request = s->configured ? s->request : config_read_version_request;
    memcpy(ack + 5, request + s->offset, 8);
    return 13;
}

static void config_read_receive(struct config_read *s, const uint8_t *p)
{
    if (s->failed) return;
    if (!config_read_frame_valid(p)) { s->failed = true; return; }
    if (s->complete) {
        if (s->reply_released) return;
        if (p[1] == (uint8_t)(s->last_report + 1)) s->reply_released = true;
        else if (p[1] != s->last_report) s->failed = true;
        return;
    }
    if (p[0] == 2) return;
    if (s->have_report) {
        if (p[1] == s->last_report) return;
        if (p[1] != (uint8_t)(s->last_report + 1)) { s->failed = true; return; }
    }
    s->last_report = p[1];
    s->have_report = true;
    for (unsigned int at = 3; at <= p[0];) {
        uint8_t header = p[at++];
        unsigned int len = (header >> 3) & 15u;
        const uint8_t *chunk = p + at;
        at += len;
        if ((header & 7u) != 7) continue;
        if (s->offset != 64 || s->received > 56) { s->failed = true; return; }
        memcpy(s->response + s->received, chunk, 8);
        s->received += 8;
        if (!(header & 128u)) {
            const uint8_t *request = s->configured ? s->request : config_read_version_request;
            unsigned int payload = s->response[3];
            unsigned int padded = (6u + payload + 7u) & ~7u;
#ifdef CORE_PROTO_FULL
            bool ok_status = (s->response[0] == 0xa1 || s->response[0] == 0xa3);
            if (payload > 58 || s->received != padded || !ok_status ||
                s->response[2] != request[2] || s->response[4] != request[4] ||
                s->response[5] != request[5] ||
                (s->response[0] == 0xa1 && request[4] == 1 && request[5] == 2 &&
                 (payload != 2 || memcmp(s->response + 6, request + 6, 2))) ||
                (s->response[0] == 0xa1 && request[4] == 1 && request[5] == 1 &&
                 (payload != request[3] || memcmp(s->response + 6, request + 6, request[3])))) s->failed = true;
            else s->complete = true;
#else
            if (payload > 58 || s->received != padded || s->response[0] != 0xa1 ||
                s->response[2] != request[2] || s->response[4] != request[4] ||
                s->response[5] != request[5] ||
                (request[4] == 1 && request[5] == 2 &&
                 (payload != 2 || memcmp(s->response + 6, request + 6, 2))) ||
                (request[4] == 1 && request[5] == 1 &&
                 (payload != request[3] || memcmp(s->response + 6, request + 6, request[3])))) s->failed = true;
            else s->complete = true;
#endif
            return;
        }
    }
}
