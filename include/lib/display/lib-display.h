#ifndef __LIB_DISPLAY_H__
#define __LIB_DISPLAY_H__

#include <karos/display/display-core.h>
#include <karos/display/display-device.h>

int display_init(struct display_device *dev, int16_t width, int16_t height);
void display_clear(struct display_device *dev);
void display_write_pixel(struct display_device *dev, int16_t x, int16_t y,
		uint16_t color);
void display_set_rotation(struct display_device *dev, uint8_t rotation);
void display_draw_bitmap(struct display_device *dev, int16_t x, int16_t y,
		uint8_t *bitmap, int16_t width, int16_t height,
		uint16_t color);
void display_draw_char(struct display_device *dev, int16_t x, int16_t y,
		unsigned char c, uint16_t color, uint16_t bg_color,
		uint8_t size);
void display_set_text_size(struct display_device *dev, uint8_t size);
void display_print_buffer(struct display_device *dev);
void display_draw_vertical_line(struct display_device *dev, int16_t x,
		int16_t y, int16_t h, uint16_t color);
void display_draw_horizontal_line(struct display_device *dev, int16_t x,
		int16_t y, int16_t w, uint16_t color);
void display_draw_circle(struct display_device *dev, int16_t x0,
		int16_t y0, int16_t r, uint16_t color);

#endif
