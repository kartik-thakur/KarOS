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

void display_draw_circle(struct display_device *dev, int16_t x0,
		int16_t y0, int16_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddf_x = 1;
	int16_t ddf_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	if (!dev)
		return;

	display_write_pixel(dev, x0, y0 + r, color);
	display_write_pixel(dev, x0, y0 - r, color);
	display_write_pixel(dev, x0 + r, y0, color);
	display_write_pixel(dev, x0 - r, y0, color);

	while(x < y) {
		if (f >= 0) {
			y--;
			ddf_y += 2;
			f += ddf_y;
		}

		x++;
		ddf_x += 2;
		f += ddf_x;

		display_write_pixel(dev, x0 + x, y0 + y, color);
		display_write_pixel(dev, x0 - x, y0 + y, color);
		display_write_pixel(dev, x0 + x, y0 - y, color);
		display_write_pixel(dev, x0 - x, y0 - y, color);
		display_write_pixel(dev, x0 + y, y0 + x, color);
		display_write_pixel(dev, x0 - y, y0 + x, color);
		display_write_pixel(dev, x0 + y, y0 - x, color);
		display_write_pixel(dev, x0 - y, y0 - x, color);
	}
}

void display_draw_circle_helper(struct display_device *dev, int16_t x0,
		int16_t y0, int16_t r,uint8_t cornername, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddf_x  = 1;
	int16_t ddf_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	if (!dev)
		return;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddf_y += 2;
			f += ddf_y;
		}

		x++;
		ddf_x += 2;
		f += ddf_x;

		if (cornername & 0x4) {
			display_write_pixel(dev, x0 + x, y0 + y, color);
			display_write_pixel(dev, x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			display_write_pixel(dev, x0 + x, y0 - y, color);
			display_write_pixel(dev, x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			display_write_pixel(dev, x0 - y, y0 + x, color);
			display_write_pixel(dev, x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			display_write_pixel(dev, x0 - y, y0 - x, color);
			display_write_pixel(dev, x0 - x, y0 - y, color);
		}
	}
}

void display_fill_circle_helper(struct display_device *dev, int16_t x0,
		int16_t y0, int16_t r, uint8_t corners, int16_t delta,
		uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddf_x = 1;
	int16_t ddf_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	int16_t px = x;
	int16_t py =  y;

	if (!dev)
		return;

	delta++;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddf_y += 2;
			f += ddf_y;
		}

		x++;
		ddf_x += 2;
		f +=ddf_y;

		if (x < (y + 1)) {
			if (corners & 1)
				display_draw_vertical_line(dev, x0 + x,
						y0 - y, 2 * y + delta, color);
			if (corners & 2)
				display_draw_vertical_line(dev, x0 - x,
						y0 - y, 2 * y + delta, color);
		}

		if ( y != py) {
			if (corners & 1)
				display_draw_vertical_line(dev, x0 + py,
						y0 - px, 2 * px + delta, color);
			if (corners & 2)
				display_draw_vertical_line(dev, x0 - py,
						y0 - px, 2 * px + delta, color);
			py = y;
		}
		px = x;
	}
}

void display_fill_circle(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t r, uint16_t color)
{
	if (!dev)
		return;

	display_draw_vertical_line(dev, x0, y0 - r, 2 * r + 1, color);
	display_fill_circle_helper(dev, x0, y0, r, 3, 0, color);
}

void display_draw_rectangle(struct display_device *dev, int16_t x, int16_t y,
		int16_t w, int16_t h, uint16_t color)
{
	if (!dev)
		return ;

	display_draw_horizontal_line(dev, x, y, w, color);
	display_draw_horizontal_line(dev, x, y + h - 1, w, color);
	display_draw_vertical_line(dev, x, y, h, color);
	display_draw_vertical_line(dev, x + w - 1, y, h, color);
}

void display_draw_round_rectangle(struct display_device *dev, int16_t x,
		int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
	int16_t max_radius = ((w < h) ? w : h) / 2;

	if (!dev)
		return;

	if (r > max_radius)
		r = max_radius;

	display_draw_horizontal_line(dev, x + r, y, w - 2 * r, color);
	display_draw_horizontal_line(dev, x + r, y + h - 1, w - 2 * r, color);
	display_draw_vertical_line(dev, x, y + r, h - 2 * r, color);
	display_draw_vertical_line(dev, x + w - 1, y + r, h - 2 * r, color);

	display_draw_circle_helper(dev, x + r, y + r, r, 1, color);
	display_draw_circle_helper(dev, x + w - r - 1, y + r, r, 2, color);
	display_draw_circle_helper(dev, x + w - r - 1, y + h - r - 1,
				   r, 4, color);
	display_draw_circle_helper(dev, x + r, y + h - r - 1, r, 8, color);
}

void display_fill_rectangle(struct display_device *dev, int16_t x, int16_t y,
		int16_t w, int16_t h, uint16_t color)
{
	int16_t i;

	if (!dev)
		return;

	for (i = x; i < x + w; i++)
		display_draw_vertical_line(dev, i, y, h, color);
}

void display_fill_round_rectangle(struct display_device *dev, int16_t x,
		int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
	int16_t max_radius = ((w < h) ? w : h) / 2;

	if (!dev)
		return;

	if (r > max_radius)
		r = max_radius;

	display_fill_rectangle(dev, x + r, y, w - 2 * r, h, color);

	display_fill_circle_helper(dev, x + w - r - 1, y + r, r, 1,
			h - 2 * r - 1, color);
	display_fill_circle_helper(dev, x + r, y + r, r, 2, h - 2 * r - 1,
			color);
}


void display_fill_screen(struct display_device *dev, uint16_t color)
{
	if (!dev)
		return;

	display_fill_rectangle(dev, 0, 0, dev->width, dev->height, color);
}

void display_draw_line(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, uint16_t color)
{
}

void display_draw_triangle(struct display_device *dev, int16_t x0,  int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

}

