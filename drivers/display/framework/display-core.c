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

int display_init(struct display_device *dev, int16_t width, int16_t height)
{
	if (dev && dev->ops->init)
		return dev->ops->init(dev, width, height);

	return -EINVAL;
}

void display_clear(struct display_device *dev)
{
	if (dev && dev->ops->clear)
		dev->ops->clear(dev);
}

void display_write_pixel(struct display_device *dev, int16_t x, int16_t y,
		uint16_t color)
{
	if (dev && dev->ops->set_pixel)
		dev->ops->set_pixel(dev, x, y, color);
}

void display_set_rotation(struct display_device *dev, uint8_t rotation)
{
	if (dev)
		dev->rotation = rotation;
}

void display_print_buffer(struct display_device *dev)
{
	if (dev && dev->ops->display)
		dev->ops->display(dev);
}

void display_draw_bitmap(struct display_device *dev, int16_t x, int16_t y,
			 uint8_t *bitmap, int16_t width, int16_t height,
			 uint16_t color)
{
	int16_t byte_width;
	int16_t i, j;
	uint8_t byte = 0;

	if (!dev)
		return;

	byte_width = (width  + 7) / 8;

	for (j = 0; j < height; j++, y++) {
		for (i = 0; i < width; i++) {
			if (i & 7) {
				byte <<= 1;
			} else {
				byte = (*(unsigned char *)(&bitmap[j *
							byte_width + i / 8]));
			}

			if (byte & 0x80)
				display_write_pixel(dev, x + i, y, color);
		}
	}
}

