#include <stdio.h>
#include <karos/error.h>
#include <karos/display/display-device.h>
#include <karos/display/display-core.h>
#include <karos/display/ssd1306.h>

int ssd1306_driver_init(void)
{
	struct display_device *display_device;
	int err = 0;

	display_device = calloc(1, sizeof(struct display_device));
	if (!display_device)
		return -ENOMEM;

	display_device->name = "ssd1306";
	display_device_register(display_device);

	return 0;
}
