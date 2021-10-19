#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <karos/error.h>
#include <karos/display/display-core.h>
#include <karos/display/display-device.h>

/*
 * Include display drivers headers to call their init functions.
 */
#include <karos/display/ssd1306.h>

struct display_device_list {
	struct display_device		*dev;
	struct display_device_list	*next;
	struct display_device_list	*prev;
};

static struct display_device_list *device_list = NULL;

int display_device_register(struct display_device *dev)
{
	if (!device_list) {
		device_list = calloc(1, sizeof(struct display_device_list));
		if (!device_list)
			return -ENOMEM;

		device_list->dev = dev;
	} else {
		struct display_device_list *device;

		for (device = device_list; device != NULL;
		     device = device->next) {
			if (!device->next)
				return -ENOMEM;

			device->next->dev = dev;
			device->next->prev = device;
			break;
		}
	}

	return 0;
}

int display_drivers_init(void)
{
	int err;

	err = ssd1306_driver_init();
	if (err)
		printf("SSD1306 driver init failed\n");

	return 0;
}

struct display_device* get_display_device_by_name(char *name)
{
	struct display_device_list *device;

	if (!name)
		return NULL;

	for (device = device_list; device != NULL; device = device->next) {
		if (!strcmp(name, device->dev->name))
			return device->dev;
	}

	return NULL;
}

