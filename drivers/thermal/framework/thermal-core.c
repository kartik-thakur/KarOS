#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <karos/error.h>
#include <karos/thermal/thermal-core.h>
#include <karos/thermal/thermal-device.h>

/*
 * Include thermal drivers header to call their init functions.
 */
#include <karos/thermal/bmp280.h>

struct thermal_device_list {
	struct thermal_device		*dev;
	struct thermal_device_list	*next;
	struct thermal_device_list	*prev;
};

static struct thermal_device_list *device_list = NULL;

int thermal_device_register(struct thermal_device *dev)
{
	if (!device_list) {
		device_list = calloc(1, sizeof(struct thermal_device_list));
		if (!device_list)
			return -ENOMEM;
		device_list->dev = dev;
	} else{
		struct thermal_device_list *device;

		for (device = device_list; device != NULL;
		     device = device->next) {
			if (!device->next) {
				device->next = calloc(1,
					sizeof(struct thermal_device_list));
				if (!device->next)
					return -ENOMEM;

				device->next->dev = dev;
				device->next->prev = device;
				break;
			}
		}
	}

	return 0;
}

struct thermal_device* get_thermal_device_by_name(char *name)
{
	struct thermal_device_list *device;

	if (!name)
		return NULL;

	for (device = device_list; device != NULL; device = device->next) {
		if (!strcmp(name, device->dev->name))
			return device->dev;
	}

	return NULL;
}

void thermal_drivers_init(void)
{
	int err;

	err = bmp280_driver_init();
	if (err)
		printf("BMP280 driver init failed\n");
}
