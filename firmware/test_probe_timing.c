#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define assert(c) do { if (!(c)) { fprintf(stderr,"assert %s line%d\n",#c,__LINE__); exit(1); } } while(0)
#define BUILD_ASSERT(c) _Static_assert(c,#c)
#define IS_ENABLED(x) x
#ifndef CONFIG_NRFX_TIMER3
#define CONFIG_NRFX_TIMER3 0
#endif
#ifndef CONFIG_USB_DEVICE_SOF
#define CONFIG_USB_DEVICE_SOF 0
#endif
#ifndef FAKE_TIMER_OWNED
#define FAKE_TIMER_OWNED 0
#endif
#define DT_NODE_HAS_STATUS(node,status) FAKE_TIMER_OWNED
#define DT_NODELABEL(node) 0
#define TIMER_MODE_MODE_Timer 0
#define TIMER_BITMODE_BITMODE_32Bit 3
#define NRFX_SUCCESS 0
typedef unsigned int nrf_ppi_channel_t;
struct timer { volatile uint32_t TASKS_STOP,INTENCLR,SHORTS,MODE,BITMODE,PRESCALER,TASKS_CLEAR,TASKS_START,TASKS_CAPTURE[6],CC[6]; };
static struct timer timer2,timer3;
#define NRF_TIMER2 (&timer2)
#define NRF_TIMER3 (&timer3)
static struct { uint32_t EVENTS_END,EVENTS_READY,MODECNF0; } radio;
#define NRF_RADIO (&radio)
static struct { uint32_t EVENTS_SOF; } usbd;
#define NRF_USBD (&usbd)
static unsigned int allocated,enabled,freed,calls,fail_at,now_us;
static bool stalled,absent,irregular;
static int nrfx_ppi_channel_alloc(nrf_ppi_channel_t *c) { if(++calls==fail_at)return -1;*c=allocated++;return 0; }
static int nrfx_ppi_channel_assign(nrf_ppi_channel_t c,uint32_t e,uint32_t t) { assert(c<allocated && e && t);return ++calls==fail_at?-1:0; }
static int nrfx_ppi_channel_enable(nrf_ppi_channel_t c) { assert(c<allocated);if(++calls==fail_at)return -1;enabled|=1u<<c;return 0; }
static int nrfx_ppi_channel_disable(nrf_ppi_channel_t c) { enabled &= ~(1u<<c);return 0; }
static int nrfx_ppi_channel_free(nrf_ppi_channel_t c) { assert(c<allocated && !(enabled&(1u<<c)));freed++;return 0; }
static void k_busy_wait(unsigned int us) {
    now_us+=us;
    if(!stalled){timer2.CC[2]=timer3.CC[2]=now_us;}
    if(!absent && (enabled&4)) timer3.CC[4]=(now_us/(irregular?1500:1000))*(irregular?1500:1000);
}
#define printk(...) ((void)0)
#include "radio_logger/src/probe_timing.h"
static void reset(void) {
 memset(&timer2,0,sizeof(timer2));memset(&timer3,0,sizeof(timer3));
 allocated=enabled=freed=calls=fail_at=now_us=0;stalled=absent=irregular=false;
 usb_phase_calibrated=false;
}
int main(void) {
 reset(); assert(init_probe_timing()==0 && usb_phase_calibrated && allocated==3 && enabled==7 && !freed);
 assert(timer3.PRESCALER==4 && timer3.TASKS_START==1 && !timer2.TASKS_START);
 probe_timing_before_tx();probe_timing_after_tx(true);
 for(unsigned int i=1;i<=9;i++) { reset();fail_at=i;assert(init_probe_timing()==-EIO);assert(freed==allocated && !enabled && timer3.TASKS_STOP && !usb_phase_calibrated); }
 reset();stalled=true;assert(init_probe_timing()==-EIO && freed==3 && !enabled);
 reset();absent=true;assert(init_probe_timing()==-EIO && freed==3 && !enabled && !usb_phase_calibrated);
 reset();irregular=true;assert(init_probe_timing()==-EIO && freed==3 && !enabled);
 return 0;
}
