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
    return latency_write_line(console, line, deadline);
}
