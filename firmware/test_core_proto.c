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

static struct device mouse_dev = {0}, cfg_dev = {1}, aux_dev = {2};
static const struct hid_ops *mouse_ops, *cfg_ops, *aux_ops;
static unsigned int irq_depth, mouse_writes, aux_writes;
static int64_t fake_ms;
static int64_t k_uptime_get(void) { return fake_ms; }
static unsigned int irq_lock(void) { irq_depth++; return 0; }
static void irq_unlock(unsigned int key) { assert(key == 0 && irq_depth == 1); irq_depth--; }
static int k_sem_take(struct k_sem *s, int timeout)
{
    ARG_UNUSED(timeout);
    if (!s->count) return -EAGAIN;
    s->count--;
    return 0;
}
static void k_sem_give(struct k_sem *s) { s->count = 1; }
static const struct device *device_get_binding(const char *name)
{
    if (!strcmp(name, "HID_0")) return &mouse_dev;
    if (!strcmp(name, "HID_1")) return &cfg_dev;
    if (!strcmp(name, "HID_2")) return &aux_dev;
    return NULL;
}
static bool device_is_ready(const struct device *dev) { return dev != NULL; }
static void usb_hid_register_device(const struct device *dev, const uint8_t *desc,
                                    size_t size, const struct hid_ops *ops)
{
    assert(dev && desc && size > 0 && ops);
    if (dev == &mouse_dev) mouse_ops = ops;
    else if (dev == &cfg_dev) cfg_ops = ops;
    else if (dev == &aux_dev) aux_ops = ops;
}
static int usb_hid_init(const struct device *dev) { assert(dev); return 0; }
static int hid_int_ep_write(const struct device *dev, const uint8_t *data,
                            uint32_t size, uint32_t *written)
{
    ARG_UNUSED(data);
    *written = size;
    if (dev == &mouse_dev) { mouse_writes++; mouse_ops->int_in_ready(dev); }
    else if (dev == &aux_dev) { aux_writes++; aux_ops->int_in_ready(dev); }
    return 0;
}

#define RF_CONFIG_READ_DIAGNOSTICS 1
#define RF_CONFIG_HID_DIAGNOSTICS 1
#define AUX_HID_FORWARDING 1
#define CORE_PROTO_FULL 1
#include "radio_logger/src/config_read.h"
static struct config_read config_read_state;
#include "radio_logger/src/wireless_hid.h"
#include "radio_logger/src/config_hid.h"

int main(void)
{
    /* Verify 3-HID binding: HID_0 mouse, HID_1 config, HID_2 aux. */
    assert(init_wireless_hid() == 0 && init_config_hid() == 0 && init_aux_hid() == 0);
    assert(mouse_ops && cfg_ops && aux_ops);
    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    wireless_hid_restart_if_needed();
    assert(wireless_hid_new_session());

    /* 1. Receiver-local firmware version (00/00/81) answers immediately without RF. */
    struct usb_setup_packet setup = {.wValue = 0x0300, .wLength = 64};
    uint8_t req[64]; memcpy(req, config_dongle_version_request, 64);
    uint8_t *ptr = req; int32_t len = 64;
    assert(cfg_ops->set_report(&cfg_dev, &setup, &len, &ptr) == 0);
    ptr = NULL; len = 0;
    assert(cfg_ops->get_report(&cfg_dev, &setup, &len, &ptr) == 0);
    assert(ptr[0] == 0xa1 && ptr[2] == 0 && ptr[3] == 3 && ptr[5] == 0x81 && ptr[6] == 1);

    /* 2. Offline mouse (mouse_linked=false) immediately returns 0xA4 without lockout. */
    uint8_t batt[64] = {0, 0, 2, 2, 0, 0x83};
    ptr = batt; len = 64;
    assert(cfg_ops->set_report(&cfg_dev, &setup, &len, &ptr) == 0);
    config_hid_poll_link(false);
    ptr = NULL; len = 0;
    assert(cfg_ops->get_report(&cfg_dev, &setup, &len, &ptr) == 0 && ptr[0] == 0xa4);

    /* 3. Allowlist coverage: all CORE startup setters + battery + strict rejections. */
    uint8_t active_prof[64] = {0, 0, 2, 1, 0, 5, 2};
    assert(config_request_valid(active_prof));
    active_prof[6] = 4; assert(!config_request_valid(active_prof));

    uint8_t debounce[64] = {0, 0, 2, 2, 0, 8, 3, 32};
    assert(config_request_valid(debounce));
    debounce[7] = 33; assert(!config_request_valid(debounce));

    uint8_t poll[64] = {0, 0, 2, 1, 1, 0, 4};
    assert(config_request_valid(poll));
    poll[6] = 3; assert(!config_request_valid(poll));

    uint8_t lod_om[64] = {0, 0, 2, 1, 1, 7, 1};
    assert(config_request_valid(lod_om));
    lod_om[6] = 2; assert(!config_request_valid(lod_om));

    uint8_t lod[64] = {0, 0, 2, 1, 1, 8, 2};
    assert(config_request_valid(lod));
    lod[6] = 3; assert(!config_request_valid(lod));

    uint8_t color[64] = {0, 0, 2, 19, 2, 1, 2, 0xff, 0, 0};
    assert(config_request_valid(color));
    color[6] = 0; assert(!config_request_valid(color));

    uint8_t bind[64] = {0, 0, 2, 10, 3, 0, 3, 0x14, 0, 1, 2, 3, 4, 5, 6, 7};
    assert(config_request_valid(bind));
    bind[8] = 1; assert(!config_request_valid(bind)); /* Non-zero slot rejected. */
    bind[8] = 0; bind[7] = 0x99; assert(!config_request_valid(bind)); /* Unknown button rejected. */

    /* Reject updater/bootloader/address/reset commands, including RF address getter 00/82. */
    uint8_t boot[64] = {0, 0, 2, 1, 0, 0x01};
    assert(!config_request_valid(boot));
    uint8_t addr[64] = {0, 0, 2, 4, 0, 0x02};
    assert(!config_request_valid(addr));
    uint8_t addr_read[64] = {0, 0, 2, 4, 0, 0x82};
    assert(!config_request_valid(addr_read));

    /* Flexible read-only getters (e.g. OpenMouse p[3]=0 or p[3]=18) are accepted. */
    uint8_t om_dpi_read[64] = {0, 0, 2, 0, 1, 0x81, 1};
    assert(config_request_valid(om_dpi_read));
    uint8_t om_poll_read[64] = {0, 0, 2, 0, 1, 0x80};
    assert(config_request_valid(om_poll_read));
    uint8_t om_ver_read[64] = {0, 0, 0, 0, 0, 0x81};
    assert(config_request_valid(om_ver_read));

    /* 4. Simultaneous Mouse + Aux + Config Reply on the same RF stream,
     * plus consecutive SET_REPORT without intermediate GET_REPORT once published.
     */
    uint8_t sel[64] = {0, 0, 2, 2, 1, 2, 2, 3};
    ptr = sel; len = 64;
    assert(cfg_ops->set_report(&cfg_dev, &setup, &len, &ptr) == 0);
    config_hid_poll_link(true);
    uint8_t idle[65] = {2, 1, 0}, ack[13];
    for (unsigned int i = 0; i < 8; i++) {
        idle[2] = (uint8_t)i;
        assert(config_read_ack(&config_read_state, idle, ack) == 13);
    }
    idle[2] = 8;
    assert(config_read_ack(&config_read_state, idle, ack) == 3);

    /* Mixed packet: Consumer (3B) + Mouse (9B) + Config Reply (9B) = 23B payload. */
    uint8_t mixed[65] = {
        23, 10, 8,
        0x13, 0xe9, 0x00,
        0x41, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x47, 0xa1, 0x00, 0x02, 0x02, 0x01, 0x02, 0x02, 0x03
    };
    assert(aux_hid_frame_valid(mixed) && config_read_frame_valid(mixed));
    assert(wireless_hid_forward(mixed) == 0 && mouse_writes == 1 && aux_writes == 1);
    config_read_receive(&config_read_state, mixed);
    assert(config_read_state.complete);
    idle[1] = 11;
    config_read_receive(&config_read_state, idle);
    config_hid_poll_link(true);
    assert(config_hid_published && !config_hid_query_active);

    /* Consecutive back-to-back SET_REPORT calls queue cleanly while in flight (OpenMouse setDpi). */
    ptr = batt; len = 64;
    assert(cfg_ops->set_report(&cfg_dev, &setup, &len, &ptr) == 0);
    ptr = sel; len = 64;
    assert(cfg_ops->set_report(&cfg_dev, &setup, &len, &ptr) == 0); /* Queued, NOT -EBUSY! */
    assert(config_hid_q_count == 1);
    config_hid_poll_link(false); /* First completes offline A4 and promotes queued sel. */
    assert(config_hid_q_count == 0 && config_hid_pending && !config_hid_published);
    config_hid_poll_link(false); /* Second completes offline A4. */
    assert(config_hid_published);
    assert(wireless_hid_finish() == 0);
    assert(!irq_depth);

    puts("CORE proto v27: 3-HID binding, local version, offline A4, startup allowlist, mixed RF stream and fire-and-forget SET checks passed.");
    return 0;
}
