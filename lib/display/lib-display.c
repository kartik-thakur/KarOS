#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <karos/error.h>
#include <karos/display/display-device.h>
#include <lib/display/lib-display.h>

#define __SWAP_INT16_T(a, b)			\
	{					\
		int16_t t = a;			\
						\
		a = b;				\
		b = t;				\
	}

#define PGM_READ_BYTE(addr) (*(const unsigned char *)(addr))
#define PGM_READ_WORD(addr) (*(const unsigned short *)(addr))
#define PGM_READ_DWORD(addr) (*(const unsigned long *)(addr))

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define PGM_READ_POINTER(addr) ((void *)PGM_READ_DWORD(addr))
#else
#define PGM_READ_POINTER(addr) ((void *)PGM_READ_WORD(addr))
#endif

inline gfx_glyph *pgm_read_glyph_ptr(const gfx_font *gfxfont, uint8_t c)
{
#ifdef __AVR__
	return &(((gfx_glyph *)pgm_read_pointer(&gfxfont->glyph))[c]);
#else
	return gfxfont->glyph + c;
#endif //__AVR__
}

inline uint8_t *pgm_read_bitmap_ptr(const gfx_font *gfxFont)
{
#ifdef __AVR__
	return (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
#else
	return gfxFont->bitmap;
#endif //__AVR__
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

void display_write_line(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, uint16_t color)
{
	int16_t steep;
	int16_t dx, dy, ystep;
	int16_t err;

	if (!dev)
		return;

	steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep) {
		__SWAP_INT16_T(x0, y0);
		__SWAP_INT16_T(x1, y1);
	}

	if (x0 > x1) {
		__SWAP_INT16_T(x0, x1);
		__SWAP_INT16_T(y0, y1);
	}

	dx = x1 - x0;
	dy = abs(y1 - y0);
	err = dx / 2;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for(; x0 <= x1; x0++) {
		if (steep) {
			display_write_pixel(dev, y0, x0, color);
		} else {
			display_write_pixel(dev, x0, y0, color);
		}

		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void display_draw_line(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, uint16_t color)
{
	if (!dev)
		return;

	if (x0 == x1) {
		if (y0 > y1)
			__SWAP_INT16_T(y0, y1);
		display_draw_vertical_line(dev, x0, y0, y1 - y0 + 1, color);
	} else if (y0 == y1) {
		if (x0 > x1)
			__SWAP_INT16_T(x0, x1);
		display_draw_horizontal_line(dev, x0, y0, x1 - x0 + 1, color);
	} else {
		display_write_line(dev, x0, y0, x1, y1, color);
	}
}

void display_draw_triangle(struct display_device *dev, int16_t x0,  int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	if (!dev)
		return;

	display_draw_line(dev, x0, y0, x1, y1, color);
	display_draw_line(dev, x1, y1, x2, y2, color);
	display_draw_line(dev, x2, y2, x0, y0, color);
}

void display_fill_triangle(struct display_device *dev, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y;
	int16_t dx01, dy01, dx02, dy02, dx12, dy12;
	int32_t sa = 0, sb = 0;
	int16_t	last;

	if (!dev)
		return;

	if (y0 > y1) {
		__SWAP_INT16_T(y0, y1);
		__SWAP_INT16_T(x0, x1);
	}

	if (y1 > y2) {
		__SWAP_INT16_T(y2, y1);
		__SWAP_INT16_T(x2, x1);
	}

	if (y0 > y1) {
		__SWAP_INT16_T(y0, y1);
		__SWAP_INT16_T(x0, x1);
	}

	if (y0 == y2) {
		a = b = x0;

		if (x1 < a) {
			a = x1;
		} else if (x1 > b) {
			b = x1;
		}

		if (x2 < a) {
			a = x2;
		} else if (x2 > b) {
			b = x2;
		}

		display_draw_horizontal_line(dev, a, y0, b - a + 1, color);
		return;
	}

	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;

	if (y1 == y2) {
		last = y1;
	} else {
		last = y1 - 1;
	}

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if (a > b)
			__SWAP_INT16_T(a, b);

		display_draw_horizontal_line(dev, a, y, b - a + 1, color);
	}

	sa = (int32_t)dx12 * (y - y1);
	sb = (int32_t)dx02 * (y - y0);

	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if (a > b)
			__SWAP_INT16_T(a, b);

		display_draw_horizontal_line(dev, a, y, b - a + 1, color);
	}
}

void display_draw_char(struct display_device *dev, int16_t x, int16_t y,
		unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
	display_draw_char_x_y(dev, x, y, c, color, bg, size, size);
}

void display_draw_char_x_y(struct display_device *dev, int16_t x, int16_t y,
		unsigned char c, uint16_t color, uint16_t bg,
		uint8_t size_x, uint8_t size_y)
{
	int8_t i, j;
	uint8_t line;
	gfx_glyph *glyph;
	uint8_t *bitmap;
	uint16_t bo;
	uint8_t w, h;
	int8_t xo, yo;
	uint8_t xx, yy, bits = 0, bit = 0;
	int16_t xo16 = 0, yo16 = 0;
	gfx_font *gfxfont;

	if (!dev)
		return;

	if (!dev->font) {
		if (!dev->_cp437 && (c >= 176))
			c++;

		for (i = 0; i < 5; i++) {
			line = PGM_READ_BYTE(&font[c * 5 + i]);
			for (j = 0; j < 8; j++, line >>= 1) {
				if (line & 1) {
					if (size_x == 1 && size_y == 1) {
						display_write_pixel(dev, x + i,
							y + j, color);
					} else {
						display_fill_rectangle(dev,
							x + i * size_x,
							y + j * size_y,
							size_x, size_y, color);
					}
				} else if (bg != color) {
					if (size_x == 1 && size_y == 1) {
						display_write_pixel(dev, x + i,
							y + j, bg);
					} else {
						display_fill_rectangle(dev,
							x + i * size_x,
							y + j * size_y, size_x,
							size_y, bg);
					}
				}
			}
			if (bg != color) {
				if (size_x == 1 && size_y == 1) {
					display_draw_vertical_line(dev, x + 5,
							y, 8, bg);
				} else {
					display_fill_rectangle(dev,
							x + 5 * size_x, y,
							size_x, 8 * size_y, bg);
				}
			}
		}
	} else {
		gfxfont = dev->font;
		c -= (uint8_t) PGM_READ_BYTE(&gfxfont->first);
		glyph = pgm_read_glyph_ptr(gfxfont, c);
		bitmap = pgm_read_bitmap_ptr(gfxfont);
		bo = PGM_READ_WORD(&glyph->bitmap_offset);
		w = PGM_READ_BYTE(&glyph->width);
		h = PGM_READ_BYTE(&glyph->height);
		xo = PGM_READ_BYTE(&glyph->x_offset);
		yo = PGM_READ_BYTE(&glyph->y_offset);

		if (size_x > 1 || size_y > 1) {
			xo16 = xo;
			yo16 = yo;
		}

		for (yy = 0; yy < h; yy++) {
			for (xx = 0; xx < w; xx++) {
				if (!(bit++ & 7)) {
					bits = PGM_READ_BYTE(&bitmap[bo++]);
				}

				if (bits & 0x80) {
					if (size_x == 1 && size_y == 1) {
						display_write_pixel(dev,
							x + xo + xx,
							y + yo + yy, color);
					} else {
						display_fill_rectangle(dev,
						    x + (xo16 + xx) * size_x,
						    y + (yo16 + yy) * size_y,
						    size_x, size_y, color);
					}
				}

				bits <<= 1;
			}
		}
	}
}


size_t display_write(struct display_device *dev, uint8_t c)
{
	uint8_t first, w, h;
	int16_t xo;
	gfx_glyph *glyph;

	if (!dev)
		return -EINVAL;

	if (!dev->font) {
		if (c == '\n') {
			dev->cursor_x = 0;
			dev->cursor_y += dev->textsize_y * 8;
		} else if (c != '\r') {
			if (dev->wrap &&
			    ((dev->cursor_x + dev->textsize_x * 6) >
			     dev->width)) {
				dev->cursor_x = 0;
				dev->cursor_y += dev->textsize_y * 8;
			}
			display_draw_char_x_y(dev, dev->cursor_x, dev->cursor_y, c,
					  dev->textcolor, dev->textbgcolor,
					  dev->textsize_x, dev->textsize_y);
			dev->cursor_x += dev->textsize_x * 6;
		}
	} else {
		if (c == '\n') {
			dev->cursor_x = 0;
			dev->cursor_y += (int16_t)dev->textsize_y *
				(uint8_t)PGM_READ_BYTE(&dev->font->y_advance);
		} else if (c != '\r') {
			first = PGM_READ_BYTE(&dev->font->first);
			glyph = pgm_read_glyph_ptr(dev->font, c - first);
			w = PGM_READ_BYTE(&glyph->width);
			h = PGM_READ_BYTE(&glyph->height);

			if ((w > 0) && (h > 0)) {
				xo = (int8_t)PGM_READ_BYTE(&glyph->x_offset);

				if (dev->wrap && ((dev->cursor_x +
				    dev->textsize_x * (xo + w)) > dev->width)) {
					dev->cursor_x = 0;
					dev->cursor_y +=
						(int16_t) dev->textsize_y *
						(uint8_t) PGM_READ_BYTE(
							&dev->font->y_advance);
				}
				display_draw_char_x_y(dev, dev->cursor_x,
					dev->cursor_y, c, dev->textcolor,
					dev->textbgcolor, dev->textsize_x,
					dev->textsize_y);
			}
			dev->cursor_x +=
				(uint8_t)PGM_READ_BYTE(&glyph->x_advance) *
					(int16_t) dev->textsize_x;
		}
	}

	return 1;
}
