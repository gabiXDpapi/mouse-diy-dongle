/* Only called with RF disarmed and no in-flight sample; never from an ISR. */
static int latency_write_line(const struct device *console, const char *line, int64_t deadline)
{
    size_t left = strlen(line);
    while (left) {
        uint32_t dtr = 0;
        if (uart_line_ctrl_get(console, UART_LINE_CTRL_DTR, &dtr) || !dtr) return -ENOTCONN;
        if (k_uptime_get() >= deadline) return -ETIMEDOUT;
        int sent = uart_fifo_fill(console, (const uint8_t *)line, (int)left);
        if (sent < 0) return sent;
        if ((size_t)sent > left) return -EIO;
        line += sent;
        left -= sent;
        if (!sent) k_sleep(K_MSEC(1));
    }
    return 0;
}

#ifdef INPUT_CONSERVATION
static int input_totals_dump(const struct device *console, int64_t deadline)
{
    char line[240];
    int length = snprintk(line, sizeof(line), "INPUT_V1 reports=%u unchanged=%u buttons=%u neutral=%u\n",
                          input_totals.reports, input_totals.unchanged, input_totals.buttons, input_totals.neutral_reports);
    if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
    int err = latency_write_line(console, line, deadline);
    if (err) return err;
    uint64_t dx = (uint64_t)input_totals.dx, dy = (uint64_t)input_totals.dy, wheel = (uint64_t)input_totals.wheel;
    length = snprintk(line, sizeof(line), "INPUT_SUM dx=%08x%08x dy=%08x%08x wheel=%08x%08x\n",
                     (uint32_t)(dx >> 32), (uint32_t)dx, (uint32_t)(dy >> 32), (uint32_t)dy,
                     (uint32_t)(wheel >> 32), (uint32_t)wheel);
    if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
    err = latency_write_line(console, line, deadline);
    if (err) return err;
    length = snprintk(line, sizeof(line), "INPUT_EDGES down=%u,%u,%u,%u,%u up=%u,%u,%u,%u,%u neutral_up=%u,%u,%u,%u,%u\n",
                     input_totals.down[0],input_totals.down[1],input_totals.down[2],input_totals.down[3],input_totals.down[4],
                     input_totals.up[0],input_totals.up[1],input_totals.up[2],input_totals.up[3],input_totals.up[4],
                     input_totals.neutral_up[0],input_totals.neutral_up[1],input_totals.neutral_up[2],input_totals.neutral_up[3],input_totals.neutral_up[4]);
    if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
    err = latency_write_line(console, line, deadline);
    if (err) return err;
    length = snprintk(line, sizeof(line), "INPUT_END capped=%u\n", input_totals_capped);
    if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
    return latency_write_line(console, line, deadline);
}
#endif

#ifdef USB_PHASE_DIAGNOSTICS
static int phase_dump(const struct device *console, unsigned int reports, int64_t deadline)
{
    char line[240];
    int length = snprintk(line, sizeof(line), "PHASE_V1 width_us=100 bins=10 calibrated=%u\n", usb_phase_calibrated);
    if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
    int err = latency_write_line(console, line, deadline);
    if (err) return err;
    for (unsigned int i = 0; i < 11; i++) {
        const struct latency_histogram *h = &phase_metrics[i];
        uint64_t submit = phase_rf_submit[i];
        length = snprintk(line, sizeof(line),
            "PHASE bin=%u count=%u rf_submit_hex=%08x%08x rf_usb_hex=%08x%08x max_us=%u hist=%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
            i, h->count, (uint32_t)(submit >> 32), (uint32_t)submit,
            (uint32_t)(h->sum >> 32), (uint32_t)h->sum, h->maximum,
            h->bins[0], h->bins[1], h->bins[2], h->bins[3], h->bins[4], h->bins[5], h->bins[6], h->bins[7], h->bins[8]);
        if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
        err = latency_write_line(console, line, deadline);
        if (err) return err;
    }
    length = snprintk(line, sizeof(line), "PHASE_END reports=%u invalid=%u\n", reports, phase_metrics[10].count);
    if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
    return latency_write_line(console, line, deadline);
}
#endif

static int latency_dump(const struct device *console, unsigned int reports,
                        unsigned int retries, unsigned int busy)
{
    static const char *const names[] = {"candidate_age", "rf_submit", "submit_usb",
                                       "rf_usb", "ack_ready", "ack_end"};
    if (!latency_dump_ready()) return -EAGAIN;
    int64_t deadline = k_uptime_get() + 1500;
    int err = latency_write_line(console,
        "LATENCY_V1 cap=1000000 upper_us=50,100,250,500,1000,2000,4000,8000,overflow\n", deadline);
    if (err) return err;
    char line[240];
    for (unsigned int i = 0; i < LAT_METRIC_COUNT; i++) {
        const struct latency_histogram *h = &latency_metrics[i];
        int length = snprintk(line, sizeof(line),
            "LATENCY metric=%s count=%u sum_us_hex=%08x%08x max_us=%u bins=%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
            names[i], h->count, (uint32_t)(h->sum >> 32), (uint32_t)h->sum, h->maximum,
            h->bins[0], h->bins[1], h->bins[2], h->bins[3], h->bins[4],
            h->bins[5], h->bins[6], h->bins[7], h->bins[8]);
        if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
        err = latency_write_line(console, line, deadline);
        if (err) return err;
    }
    int length = snprintk(line, sizeof(line), "LATENCY_END failed=%u capped=%u tracking_fault=%u reports=%u retries=%u busy=%u\n",
                          latency_failed_submissions, latency_capped, latency_tracking_fault,
                          reports, retries, busy);
    if (length < 0 || length >= (int)sizeof(line)) return -EOVERFLOW;
    err = latency_write_line(console, line, deadline);
#ifdef INPUT_CONSERVATION
    if (!err) err = input_totals_dump(console, deadline);
#endif
#ifdef USB_PHASE_DIAGNOSTICS
    if (!err) err = phase_dump(console, reports, deadline);
#endif
    return err;
}
