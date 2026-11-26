#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifndef ETIMEDOUT
#define ETIMEDOUT 110
#endif
#ifndef ENOTSUP
#define ENOTSUP 95
#endif
#define assert(c) do { if (!(c)) { \
    fprintf(stderr, "Assertion failed: %s at line %d\n", #c, __LINE__); exit(1); \
} } while (0)
#define BUILD_ASSERT(c) _Static_assert(c, #c)
#define ARG_UNUSED(x) ((void)(x))
#define K_MSEC(ms) (ms)
#define K_SEM_DEFINE(name, initial, limit) struct k_sem name = {initial}
#define UDC_STATIC_BUF_DEFINE(name, size) static uint8_t name[((size + 3) / 4) * 4]
typedef int atomic_t;
static int atomic_get(const atomic_t *v) { return *v; }
static void atomic_set(atomic_t *v, int n) { *v = n; }
static void atomic_clear(atomic_t *v) { *v = 0; }
struct k_sem { int count; };
struct device { int id; };
struct usb_setup_packet { uint16_t wValue, wLength; };
typedef int (*hid_cb_t)(const struct device *, struct usb_setup_packet *, int32_t *, uint8_t **);
struct hid_ops {
    void (*int_in_ready)(const struct device *);
    hid_cb_t get_report, set_report;
};
enum usb_dc_status_code { USB_DC_CONFIGURED, USB_DC_RESET, USB_DC_DISCONNECTED, USB_DC_ERROR };
#define SYS_REBOOT_COLD 0
static void sys_reboot(int mode) { assert(mode == SYS_REBOOT_COLD); }

static struct device mouse_dev = {0}, aux_dev = {1};
static const struct hid_ops *mouse_ops, *aux_ops;
static bool aux_available = true, aux_ready = true, auto_complete = true;
static int aux_init_result, mouse_write_result, aux_write_result;
static unsigned int mouse_writes, aux_writes;
static uint8_t last_mouse[6];
static uint8_t aux_log[16][9];
static uint32_t aux_log_len[16];
static unsigned int aux_log_count;

static int k_sem_take(struct k_sem *s, int timeout)
{
    if (!s->count && timeout > 0 && auto_complete) s->count = 1;
    if (!s->count) return -EAGAIN;
    s->count--;
    return 0;
}
static void k_sem_give(struct k_sem *s) { s->count = 1; }
static const struct device *device_get_binding(const char *name)
{
    if (!strcmp(name, "HID_0")) return &mouse_dev;
    if (!strcmp(name, "HID_1")) return aux_available ? &aux_dev : NULL;
    return NULL;
}
static bool device_is_ready(const struct device *dev)
{
    if (dev == &mouse_dev) return true;
    if (dev == &aux_dev) return aux_ready;
    return false;
}

/* Verify balanced collections and required usages on our 5-collection aux descriptor. */
static bool aux_descriptor_valid(const uint8_t *desc, size_t size)
{
    uint32_t page = 0, report_id = 0;
    unsigned int depth = 0, collections = 0, inputs = 0, outputs = 0;
    uint32_t seen_ids = 0;
    bool have_usage = false;
    for (size_t at = 0; at < size;) {
        uint8_t item = desc[at++];
        unsigned int bytes = item & 3u;
        if (bytes == 3) bytes = 4;
        if (item == 0xfe || at + bytes > size) return false;
        uint32_t value = 0;
        for (unsigned int i = 0; i < bytes; i++) value |= (uint32_t)desc[at++] << (8 * i);
        unsigned int type = (item >> 2) & 3u, tag = item >> 4;
        if (type == 1) {
            if (tag == 0) page = value;
            else if (tag == 8) { report_id = value; seen_ids |= (1u << report_id); }
        } else if (type == 2 && (tag == 0 || tag == 1 || tag == 2)) {
            have_usage = true;
        } else if (type == 0) {
            if (tag == 10) {
                if (depth || !have_usage || value != 1 || !page) return false;
                depth++; collections++;
            } else if (tag == 8) {
                if (depth != 1 || !report_id) return false;
                if (!(value & 1u) && !have_usage) return false; /* Data items need local usage. */
                inputs++;
            } else if (tag == 9) {
                if (depth != 1 || !report_id) return false;
                if (!(value & 1u) && !have_usage) return false;
                outputs++;
            } else if (tag == 12) {
                if (!depth) return false;
                depth--;
            } else return false;
            have_usage = false;
        } else return false;
    }
    /* Report IDs 1..5 must all be present across 5 Application Collections. */
    return !depth && collections == 5 && seen_ids == 0x3eu && inputs >= 7 && outputs == 2;
}

static void usb_hid_register_device(const struct device *dev, const uint8_t *desc,
                                    size_t size, const struct hid_ops *ops)
{
    assert(dev && desc && size > 0 && ops);
    if (dev == &mouse_dev) mouse_ops = ops;
    else {
        assert(dev == &aux_dev && aux_descriptor_valid(desc, size));
        aux_ops = ops;
    }
}
static int usb_hid_init(const struct device *dev)
{
    assert(dev);
    return dev == &aux_dev ? aux_init_result : 0;
}
static int hid_int_ep_write(const struct device *dev, const uint8_t *data,
                            uint32_t size, uint32_t *written)
{
    if (dev == &mouse_dev) {
        assert(size == 6);
        if (mouse_write_result) return mouse_write_result;
        memcpy(last_mouse, data, 6);
        *written = 6;
        mouse_writes++;
        if (auto_complete) mouse_ops->int_in_ready(dev);
        return 0;
    }
    assert(dev == &aux_dev && size >= 2 && size <= 9);
    if (aux_write_result) return aux_write_result;
    if (aux_log_count < 16) {
        memset(aux_log[aux_log_count], 0, 9);
        memcpy(aux_log[aux_log_count], data, size);
        aux_log_len[aux_log_count] = size;
        aux_log_count++;
    }
    *written = size;
    aux_writes++;
    if (auto_complete) aux_ops->int_in_ready(dev);
    return 0;
}

#define AUX_HID_FORWARDING 1
#include "radio_logger/src/wireless_hid.h"

int main(void)
{
    /* Init failure guards. */
    aux_available = false; assert(init_aux_hid() == -ENODEV); aux_available = true;
    aux_ready = false; assert(init_aux_hid() == -ENODEV); aux_ready = true;
    aux_init_result = -EIO; assert(init_aux_hid() == -EIO); aux_init_result = 0;
    assert(init_wireless_hid() == 0 && init_aux_hid() == 0 && aux_ops && aux_ops->set_report);

    /* Keyboard LED Output SET_REPORT handling. */
    uint8_t led_buf[2] = {0x03, 0x02};
    uint8_t *ptr = led_buf; int32_t slen = 2;
    struct usb_setup_packet setup = {.wValue = 0x0203, .wLength = 2};
    assert(aux_ops->set_report(&aux_dev, &setup, &slen, &ptr) == 0);
    setup.wLength = 1; slen = 1;
    assert(aux_ops->set_report(&aux_dev, &setup, &slen, &ptr) == 0);
    setup.wValue = 0x0300;
    assert(aux_ops->set_report(&aux_dev, &setup, &slen, &ptr) == -ENOTSUP);

    /* Frame validation checks. */
    uint8_t idle[3] = {2, 1, 0};
    assert(aux_hid_frame_valid(idle));
    uint8_t bad_short[2] = {1, 0}; assert(!aux_hid_frame_valid(bad_short));
    uint8_t bad_type[12] = {11, 0, 0, 0x47}; assert(!aux_hid_frame_valid(bad_type));
    uint8_t bad_cont[12] = {11, 0, 0, 0xc2}; assert(!aux_hid_frame_valid(bad_cont));
    uint8_t bad_len[12] = {11, 0, 0, 0x13, 0xe9, 0x00}; assert(!aux_hid_frame_valid(bad_len));

    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    wireless_hid_restart_if_needed();
    assert(wireless_hid_new_session());

    /* Forward Keyboard (type 2 -> ID 3, 9B). */
    uint8_t kbd[12] = {11, 1, 0, 0x42, 0x02, 0x00, 0x04, 0, 0, 0, 0, 0};
    assert(aux_hid_frame_valid(kbd) && wireless_hid_forward(kbd) == 0);
    assert(aux_writes == 1 && aux_log_len[0] == 9 && aux_log[0][0] == 0x03 &&
           aux_log[0][1] == 0x02 && aux_log[0][3] == 0x04 && aux_hid_kbd_active);

    /* Forward Consumer (type 3 -> ID 1, 3B). */
    uint8_t cons[6] = {5, 2, 0, 0x13, 0xe9, 0x00};
    assert(aux_hid_frame_valid(cons) && wireless_hid_forward(cons) == 0);
    assert(aux_writes == 2 && aux_log_len[1] == 3 && aux_log[1][0] == 0x01 &&
           aux_log[1][1] == 0xe9 && aux_log[1][2] == 0x00 && aux_hid_consumer_active);

    /* Forward System Control (type 4 -> ID 2, 2B, masked to 3 bits). */
    uint8_t sysp[5] = {4, 3, 0, 0x0c, 0xff};
    assert(aux_hid_frame_valid(sysp) && wireless_hid_forward(sysp) == 0);
    assert(aux_writes == 3 && aux_log_len[2] == 2 && aux_log[2][0] == 0x02 &&
           aux_log[2][1] == 0x07 && aux_hid_system_active);

    /* Forward Vendor FFA0 (type 5 -> ID 4, 9B) and FFFF (type 6 -> ID 5, 9B). */
    uint8_t v4[12] = {11, 4, 0, 0x45, 0x06, 0x01, 0, 0, 0, 0, 0, 0};
    uint8_t v5[12] = {11, 5, 0, 0x46, 0xaa, 0x55, 0, 0, 0, 0, 0, 0};
    assert(wireless_hid_forward(v4) == 0 && aux_writes == 4 && aux_log[3][0] == 0x04 && aux_log[3][1] == 0x06);
    assert(wireless_hid_forward(v5) == 0 && aux_writes == 5 && aux_log[4][0] == 0x05 && aux_log[4][1] == 0xaa);

    /* Compound Mouse (type 1) + Vendor FFA0 (type 5) with USB busy on record 1:
     * Must NOT duplicate Mouse movement on retransmission!
     */
    uint8_t comp[21] = {20, 6, 0,
                        0x41, 0x01, 0x00, 0x05, 0x00, 0xfb, 0xff, 0, 0,
                        0x45, 0x02, 0x02, 0, 0, 0, 0, 0, 0};
    assert(aux_hid_frame_valid(comp));
    aux_write_result = -EAGAIN;
    assert(wireless_hid_forward(comp) == -EAGAIN);
    assert(mouse_writes == 1 && aux_writes == 5 && aux_hid_busy == 1);
    aux_write_result = 0;
    assert(wireless_hid_forward(comp) == 0);
    assert(mouse_writes == 1 && aux_writes == 6); /* Mouse was NOT duplicated! */
    assert(aux_log[5][0] == 0x04 && aux_log[5][1] == 0x02);
    /* Full retransmission of seq 6 must not duplicate either record. */
    assert(wireless_hid_forward(comp) == 0);
    assert(mouse_writes == 1 && aux_writes == 6 && wireless_hid_retries == 1);

    /* Neutral release on link loss / stop: must release Mouse + Kbd + Consumer + System,
     * and NOT emit zero reports for stateless Vendor IDs 4/5.
     */
    aux_log_count = 0;
    assert(wireless_hid_finish() == 0);
    assert(mouse_writes == 2 && last_mouse[0] == 0);
    assert(aux_log_count == 3);
    assert(aux_log_len[0] == 9 && aux_log[0][0] == 0x03 && !aux_hid_nonzero(aux_log[0] + 1, 8));
    assert(aux_log_len[1] == 3 && aux_log[1][0] == 0x01 && !aux_hid_nonzero(aux_log[1] + 1, 2));
    assert(aux_log_len[2] == 2 && aux_log[2][0] == 0x02 && aux_log[2][1] == 0);
    assert(!aux_hid_kbd_active && !aux_hid_consumer_active && !aux_hid_system_active);
    /* Second finish is a no-op. */
    assert(wireless_hid_finish() == 0 && mouse_writes == 2 && aux_log_count == 3);

    /* Timeout during aux neutral release fails closed and keeps active flag set. */
    auto_complete = false;
    kbd[1] = 7;
    assert(wireless_hid_forward(kbd) == 0 && aux_hid_kbd_active);
    assert(wireless_hid_finish() == -ETIMEDOUT && aux_hid_kbd_active);
    auto_complete = true;
    assert(wireless_hid_finish() == 0 && !aux_hid_kbd_active);

    puts("Auxiliary HID descriptor, RF types 1..6, compound busy-retry, LED SET_REPORT and neutral release checks passed.");
    return 0;
}
