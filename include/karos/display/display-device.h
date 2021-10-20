#ifndef __DISPLAY_DEVICE_H__
#define __DISPLAY_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include <lib/display/gfx-font.h>

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
	void		(*draw_horizontal_line)(struct display_device *dev,
				int16_t x, int16_t y, int16_t w,
				uint16_t color);
	void		(*draw_vertical_line)(struct display_device *dev,
				int16_t x, int16_t y, int16_t h,
				uint16_t color);
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
	bool				_cp437;
	gfx_font			*font;
};

#endif
