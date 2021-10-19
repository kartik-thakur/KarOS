#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <karos/error.h>
#include <lib/display/lib-display.h>
#include <karos/display/display-device.h>

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

void display_draw_vertical_line(struct display_device *dev, int16_t x,
		int16_t y, int16_t h, uint16_t color)
{
	if (dev && dev->ops->draw_vertical_line)
		dev->ops->draw_vertical_line(dev, x, y, h, color);
}

void display_draw_horizontal_line(struct display_device *dev, int16_t x,
		int16_t y, int16_t w, uint16_t color)
{
	if (dev && dev->ops->draw_horizontal_line)
		dev->ops->draw_horizontal_line(dev, x, y, w, color);
}
