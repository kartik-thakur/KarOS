#ifndef __LIB_DISPLAY_H__
#define __LIB_DISPLAY_H__

#include <karos/display/display-core.h>
#include <karos/display/display-device.h>
#include <lib/display/gfx-font.h>

struct display_config {
	int16_t				cursor_x;
	int16_t				cursor_y;
	uint8_t				textsize_x;
	uint8_t				textsize_y;
	uint16_t			textcolor;
	uint16_t			textbgcolor;
	uint8_t				rotation;
	bool				wrap;
	bool				_cp437;
	gfx_font			*font;
};

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
void display_draw_circle_helper(struct display_device *deev, int16_t x0,
		int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void display_fill_circle_helper(struct display_device *dev, int16_t x0,
		int16_t y0, int16_t r, uint8_t corners, int16_t delta,
		uint16_t color);
void display_fill_circle(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t r, uint16_t color);
void display_draw_rectangle(struct display_device *dev, int16_t x, int16_t y,
		int16_t w, int16_t h, uint16_t color);
void display_draw_round_rectangle(struct display_device *dev, int16_t x,
		int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void display_fill_rectangle(struct display_device *dev, int16_t x, int16_t y,
		int16_t w, int16_t h, uint16_t color);
void display_fill_screen(struct display_device *dev, uint16_t color);
void display_draw_triangle(struct display_device *dev, int16_t x0,  int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void display_draw_line(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, uint16_t color);
void display_write_line(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, uint16_t color);
void display_fill_triangle(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void display_draw_char(struct display_device *dev, int16_t x, int16_t y,
		unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void display_draw_char_x_y(struct display_device *dev, int16_t x, int16_t y,
		unsigned char c, uint16_t color, uint16_t bg,
		uint8_t size_x, uint8_t size_y);
size_t display_write(struct display_device *dev, uint8_t c);
#endif
