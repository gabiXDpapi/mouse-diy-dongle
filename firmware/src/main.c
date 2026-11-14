#include <errno.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/usb/udc_buf.h>
#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const uint8_t descriptor[] = HID_MOUSE_REPORT_DESC(3);
static K_SEM_DEFINE(configured, 0, 1);
static K_SEM_DEFINE(sent, 0, 1);

static void usb_status(enum usb_dc_status_code status, const uint8_t *param)
{
	ARG_UNUSED(param);
	if (status == USB_DC_CONFIGURED) {
		k_sem_give(&configured);
	}
}

static void report_sent(const struct device *dev)
{
	ARG_UNUSED(dev);
	k_sem_give(&sent);
}

static const struct hid_ops ops = { .int_in_ready = report_sent };

int main(void)
{
	const struct device *hid = device_get_binding("HID_0");
	/* Buttons, relative X, relative Y, wheel: no clicks or scrolling. */
	static const int8_t movements[][4] = {{0, 40, 0, 0}, {0, -40, 0, 0}};
	UDC_STATIC_BUF_DEFINE(report, 4);
	int err;

	if (!gpio_is_ready_dt(&led)) {
		return -ENODEV;
	}
	err = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (err) {
		return err;
	}
	if (hid == NULL || !device_is_ready(hid)) {
		return -ENODEV;
	}
	usb_hid_register_device(hid, descriptor, sizeof(descriptor), &ops);
	err = usb_hid_init(hid);
	if (err) {
		return err;
	}
	err = usb_enable(usb_status);
	if (err) {
		return err;
	}

	k_sem_take(&configured, K_FOREVER);
	k_sleep(K_SECONDS(5));
	for (size_t i = 0; i < ARRAY_SIZE(movements); i++) {
		memcpy(report, movements[i], sizeof(report));
		err = hid_int_ep_write(hid, report, sizeof(report), NULL);
		if (err) {
			return err;
		}
		if (k_sem_take(&sent, K_SECONDS(1)) != 0) {
			return -ETIMEDOUT;
		}
		k_sleep(K_MSEC(250));
	}

	/* End the test once per boot; USB remains enumerated. */
	return gpio_pin_set_dt(&led, 0);
}
