#ifndef __DISPLAY_DEVICE_H__
#define __DISPLAY_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>

struct display_device;

struct display_device_ops {
	int		(*init)(struct display_device *dev,
				int16_t width, int16_t height);
	void		(*display)(struct display_device *dev);
	void		(*clear)(struct display_device *dev);
	void		(*set_pixel)(struct display_device *dev,
				int16_t x, int16_t y, int16_t color);
	bool		(*get_pixel)(struct display_device *dev,
				int16_t x, int16_t y);
	uint8_t*	(*get_buffer)(struct display_device);
};

struct display_device {
	char				*name;
	void				*priv_data;
	struct display_device_ops	*ops;

	int16_t				height;
	int16_t				width;
	int16_t				cursor_x;
	int16_t				cursor_y;
	uint8_t				textsize_x;
	uint8_t				textsize_y;
	uint16_t			textcolor;
	uint16_t			textbgcolor;
	uint8_t				rotation;
	bool				wrap;
};

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
void display(struct display_device *dev);
#endif
