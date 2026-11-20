#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef ENOTSUP
#define ENOTSUP 95
#endif
#define assert(c) do { if (!(c)) { fprintf(stderr, "Failed line %d: %s\n", __LINE__, #c); exit(1); } } while (0)
#define ARG_UNUSED(v) ((void)(v))
#define UDC_STATIC_BUF_DEFINE(name, size) static uint8_t name[size]
struct device { int unused; };
struct usb_setup_packet { uint16_t wValue, wLength; };
typedef int (*hid_cb_t)(const struct device *, struct usb_setup_packet *, int32_t *, uint8_t **);
struct hid_ops { hid_cb_t get_report, set_report; };
static struct device device;
static bool available = true, ready = true;
static int init_result;
static const struct hid_ops *registered;
static unsigned int irq_depth;
static unsigned int irq_lock(void) { irq_depth++; return 0; }
static void irq_unlock(unsigned int key) { assert(key == 0 && irq_depth == 1); irq_depth--; }
static const struct device *device_get_binding(const char *name)
{ assert(!strcmp(name,"HID_1")); return available ? &device : NULL; }
static bool device_is_ready(const struct device *d) { return d && ready; }
/* Narrow semantic check for our one-collection, ID-less vendor Feature channel. */
static bool feature_descriptor_valid(const uint8_t *desc, size_t size)
{
    uint32_t page = 0, usage = 0, bits = 0, count = 0;
    unsigned int depth = 0, features = 0;
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
            else if (tag == 7) bits = value;
            else if (tag == 8) return false;
            else if (tag == 9) count = value;
        } else if (type == 2 && tag == 0) {
            usage = value; have_usage = true;
        } else if (type == 0) {
            if (tag == 10) {
                if (depth || !have_usage || page != 0xffff || usage || value != 1) return false;
                depth++;
            } else if (tag == 11) {
                if (depth != 1 || !have_usage || page != 0xffff || usage || bits != 8 || count != 64 || value != 2) return false;
                features++;
            } else if (tag == 12) {
                if (!depth) return false;
                depth--;
            } else return false;
            /* Local usages are consumed by EVERY Main item, including Collection. */
            have_usage = false;
        } else return false;
    }
    return !depth && features == 1;
}
static void usb_hid_register_device(const struct device *d, const uint8_t *desc, size_t size, const struct hid_ops *ops)
{ assert(d && feature_descriptor_valid(desc,size)); registered = ops; }
static int usb_hid_init(const struct device *d) { assert(d); return init_result; }
#include "radio_logger/src/config_read.h"
static struct config_read config_read_state;
#include "radio_logger/src/config_hid.h"

int main(void)
{
    const uint8_t old_missing_usage[] = {6,255,255,9,0,161,1,21,0,38,255,0,117,8,149,64,177,2,192};
    assert(!feature_descriptor_valid(old_missing_usage,sizeof(old_missing_usage)));
    assert(feature_descriptor_valid(config_hid_descriptor,sizeof(config_hid_descriptor)));
    assert(!feature_descriptor_valid(config_hid_descriptor,sizeof(config_hid_descriptor)-1));
    uint8_t bad_descriptor[sizeof(config_hid_descriptor)];
    memcpy(bad_descriptor,config_hid_descriptor,sizeof(bad_descriptor));
    for (size_t i=0;i+1<sizeof(bad_descriptor);i++) if (bad_descriptor[i]==0x95) bad_descriptor[i+1]=63;
    assert(!feature_descriptor_valid(bad_descriptor,sizeof(bad_descriptor)));
    available = false; assert(init_config_hid() == -ENODEV); available = true;
    ready = false; assert(init_config_hid() == -ENODEV); ready = true;
    init_result = -EIO; assert(init_config_hid() == -EIO); init_result = 0;
    assert(init_config_hid() == 0 && registered);
    struct usb_setup_packet setup = {.wValue=0x0300, .wLength=64};
    uint8_t request[64]; memcpy(request, config_read_version_request,64);
    uint8_t *data = request; int32_t length = 64;
    assert(registered->get_report(&device,&setup,&length,&data) == 0 && data[0] == 0xa3);
    data = request; setup.wValue = 0x0200;
    assert(registered->set_report(&device,&setup,&length,&data) == -EINVAL);
    setup.wValue = 0x0300; length = 63;
    assert(registered->set_report(&device,&setup,&length,&data) == -EINVAL);
    length = 64; request[5] = 1;
    assert(registered->set_report(&device,&setup,&length,&data) == -ENOTSUP);
    request[5] = 0x81;
    assert(registered->set_report(&device,&setup,&length,&data) == 0);
    assert(!config_hid_query_active);
    data = NULL; length = 0;
    assert(registered->get_report(&device,&setup,&length,&data) == 0 && length == 64 && data[0] == 0xa0);
    config_hid_poll(); assert(config_hid_query_active && !config_read_state.started);
    /* Drive the real RF codec after the USB callback has queued the request. */
    uint8_t idle[65] = {2,1,0}, ack[13];
    for (unsigned i=0;i<8;i++) { idle[2]=(uint8_t)i; assert(config_read_ack(&config_read_state,idle,ack)==13); }
    idle[2]=8; assert(config_read_ack(&config_read_state,idle,ack)==3);
    uint8_t rf[65] = {11,20,8,0xc7,0xa1,0,2,3,0,0x81,1,0};
    config_read_receive(&config_read_state,rf);
    rf[1]=21; rf[3]=0x47; memset(rf+4,0,8); config_read_receive(&config_read_state,rf);
    assert(config_read_state.complete);
    config_hid_poll();
    data = NULL;
    assert(registered->get_report(&device,&setup,&length,&data)==0 && data[0]==0xa1 && data[6]==1);
    uint8_t snapshot[64]; memcpy(snapshot,data,64);
    config_read_state.response[0]=0xa2; config_hid_poll();
    assert(!memcmp(data,snapshot,64)); /* RF publication never mutates an active USB snapshot. */
    data=request; length=64;
    assert(registered->set_report(&device,&setup,&length,&data)==-EBUSY);
    config_hid_reset(); config_read_state.failed=true; config_hid_query_active=true;
    config_hid_poll(); data=NULL;
    assert(registered->get_report(&device,&setup,&length,&data)==0 && data[0]==0xa2);
    assert(!irq_depth);
    puts("Vendor Feature callbacks, real RF roundtrip, immutable allowlist and mailbox checks passed.");
    return 0;
}
