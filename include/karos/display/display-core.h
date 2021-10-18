#ifndef __DISPLAY_CORE_H__
#define __DISPLAY_CORE_H__

#include <karos/display/display-device.h>

int display_device_register(struct display_device *dev);
int display_drivers_init(void);
struct display_device* get_display_device_by_name(char *name);

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

#endif
