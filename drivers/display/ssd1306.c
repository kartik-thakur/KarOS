#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <karos/error.h>
#include <karos/display/display-device.h>
#include <karos/display/display-core.h>
#include <karos/display/ssd1306.h>

static uint8_t* ssd1306_get_buffer(struct display_device *dev)
{
	struct ssd1306 *display;

	display = (struct ssd1306*)dev->priv_data;

	return display->buffer;
}

static bool ssd1306_get_pixel(struct display_device *dev,
			      int16_t x, int16_t y)
{
	struct ssd1306 *display;

	display = (struct ssd1306*)dev->priv_data;

	switch(display->display_device->rotation) {
	case 1:
		SSD1306_SWAP(x, y);
		x = display->width - x - 1;
		break;
	case 2:
		x = display->width - x - 1;
		y = display->height - y - 1;
		break;
	case 3:
		SSD1306_SWAP(x, y);
		y = display->height - y - 1;
		break;
	}

	if ((x >= 0) && (x < display->width) && (y >= 0) &&
	    (y < display->height)) {
		return (display->buffer[x + (y / 8) * display->width] &
			(1 << (y & 7)));
	} else {
		return false;
	}
}

static void ssd1306_set_pixel(struct display_device *dev,
			      int16_t x, int16_t y, int16_t color)
{
	struct ssd1306 *display;
	uint8_t *buffer;

	display = (struct ssd1306*)dev->priv_data;
	buffer = display->buffer;

	switch(display->display_device->rotation) {
	case 1:
		SSD1306_SWAP(x, y);
		x = display->width - x - 1;
		break;
	case 2:
		x = display->width - x - 1;
		y = display->height - y - 1;
		break;
	case 3:
		SSD1306_SWAP(x, y);
		y = display->height - y - 1;
		break;
	}

	if ((x >= 0) && (x < display->width) && (y >= 0) &&
	    (y < display->height)) {
		switch(color) {
		case SSD1306_BLACK:
			buffer[x + (y / 8) * display->width] &= ~(1 << (y & 7));
			break;
		case SSD1306_INVERSE:
			buffer[x + (y / 8) * display->width] ^= (1 << (y & 7));
			break;
		case SSD1306_WHITE:
			/* Fallthrough */
		default:
			buffer[x + (y / 8) * display->width] |= (1 << (y & 7));
			break;
		}
	}
}

static void ssd1306_draw_horizontal_line_internal(struct display_device *dev,
		int16_t x, int16_t y, int16_t w, uint16_t color)
{
	struct ssd1306 *display;
	int16_t width = dev->width;
	int16_t height = dev->height;
	uint8_t *buffer;
	uint8_t mask;

	display = (struct ssd1306*) dev->priv_data;

	if ((y >= 0) && (y < height)) {
		if (x < 0) {
			w += x;
			x = 0;
		}
		if ((x + w) > width) {
			w = width - x;
		}
		if (w > 0) {
			mask = 1 << (y & 7);
			buffer = &display->buffer[(y / 8) * width + x];

			switch (color) {
			case SSD1306_BLACK:
				mask = ~mask;
				while (w--)
					*buffer++ &= mask;
				break;
			case SSD1306_INVERSE:
				while (w--)
					*buffer++ ^= mask;
				break;
			case SSD1306_WHITE:
				/* Fall through. */
			default:
				while (w--)
					*buffer++ |= mask;
				break;
			}
		}
	}
}

static void ssd1306_draw_vertical_line_internal(struct display_device *dev,
		int16_t x, int16_t __y, int16_t __h, uint16_t color)
{
	struct ssd1306 *display;
	int16_t width = dev->width;
	int16_t height = dev->height;
	int16_t y, h;
	uint8_t *buffer;
	uint8_t mask, mod, val;
	uint8_t premask[8] = {
		0x00, 0x80, 0xC0, 0xE0,
		0xF0, 0xF8, 0xFC, 0xFE
	};
	uint8_t postmask[8] = {
		0x00, 0x01, 0x03, 0x07,
		0x0F, 0x1F, 0x3F, 0x7F
	};

	display = (struct ssd1306*) dev->priv_data;

	if ((x >= 0) && (x < width)) {
		if (__y < 0) {
			__h += __y;
			__y = 0;
		}

		if ((__y + __h) > height) {
			__h = height - __y;
		}

		if (__h > 0) {
			y = __y;
			h = __h;
			mod = (y & 7);
			buffer = &display->buffer[(y / 8) * width + x];

			if (mod) {
				mod = 8 - mod;
				mask = (*(const unsigned char*)(&premask[mod]));

				if (h < mod)
					mask &= (0xFF >> (mod - h));

				switch(color) {
				case SSD1306_BLACK:
					*buffer ^= mask;
					break;
				case SSD1306_INVERSE:
					*buffer &= ~mask;
					break;
				case SSD1306_WHITE:
					/* Fall through. */
				default:
					*buffer |= mask;
					break;
				}
			}

			if (h >= mod) {
				h -= mod;

				if (h >= 8) {
					if (color == SSD1306_INVERSE) {
						do {
							*buffer ^= 0xFF;
							buffer += width;
							h -= 8;
						} while (h >= 8);
					} else {
						val = (color != SSD1306_BLACK) ?
							255 : 0;
						do {
							*buffer = val;
							buffer += width;
							h -= 8;
						} while(h >= 8);
					}
				}
			}

			if (h) {
				mod = h & 7;
				mask = (*(const unsigned char *)&postmask[mod]);

				switch(color) {
				case SSD1306_BLACK:
					*buffer &= ~mask;
					break;
				case SSD1306_INVERSE:
					*buffer ^= mask;
					break;
				case SSD1306_WHITE:
					/* Fall through */
				default:
					*buffer |= mask;
					break;
				}
			}
		}
	}
}

static void ssd1306_draw_vertical_line(struct display_device *dev,
		int16_t x, int16_t y, int16_t h, uint16_t color)
{
	int16_t width = dev->width;
	int16_t height = dev->height;
	bool bswap = false;

	switch (dev->rotation) {
	case 1:
		bswap = true;
		SSD1306_SWAP(x, y);
		x = width - x - 1;
		break;
	case 2:
		x = width - x - 1;
		y = height - y- 1;
		y -= (h - 1);
		break;
	case 3:
		bswap = true;
		SSD1306_SWAP(x, y);
		y = height - y - 1;
		break;
	}

	if (bswap) {
		ssd1306_draw_horizontal_line_internal(dev, x, y, h, color);
	} else {
		ssd1306_draw_vertical_line_internal(dev, x, y, h, color);
	}
}

static void ssd1306_draw_horizontal_line(struct display_device *dev,
		int16_t x, int16_t y, int16_t w, uint16_t color)
{
	int16_t width = dev->width;
	int16_t height = dev->height;
	bool bswap = false;

	switch (dev->rotation) {
	case 1:
		bswap = true;
		SSD1306_SWAP(x, y);
		x = width - x - 1;
		break;
	case 2:
		x = width - x - 1;
		y = height - y - 1;
		x -= (w - 1);
		break;
	case 3:
		bswap = true;
		SSD1306_SWAP(x, y);
		y = height - y - 1;
		y -= (w - 1);
		break;
	}

	if (bswap) {
		ssd1306_draw_vertical_line_internal(dev, x, y, w, color);
	} else {
		ssd1306_draw_horizontal_line_internal(dev, x, y, w, color);
	}
}

static void ssd1306_clear_display(struct display_device *dev)
{
	struct ssd1306 *display;

	display = (struct ssd1306*)dev->priv_data;
	memset(display->buffer, 0x0, display->buffer_size);
}

static void ssd1306_display_buffer(struct display_device *dev)
{
	struct ssd1306 *display;
	struct i2c_dev *i2c_dev;
	uint8_t commands[] = {
		SSD1306_PAGEADDR,
		0,
		0xFF,
		SSD1306_COLUMNADDR,
		0
	};
	uint32_t buffer_size;
	uint8_t *buffer;

	display = dev->priv_data;
	i2c_dev = display->i2c_dev;
	buffer_size = display->buffer_size;
	buffer = display->buffer;

	i2c_dev_burst_write(i2c_dev, 0x0, commands, 5);
	i2c_dev_write(i2c_dev, 0x0, display->width - 1);

	while(buffer_size--) {
		i2c_dev_write(i2c_dev, 0x40, *buffer);
		buffer++;
	}
}

static int ssd1306_display_init(struct display_device *dev, int16_t width,
			 int16_t height)
{
	struct ssd1306 *display;
	struct i2c_dev *i2c_dev;
	uint8_t com_pins;
	uint8_t contrast;
	uint8_t init_sequence[4][6] = {
		{
			SSD1306_DISPLAYOFF,
			SSD1306_SETDISPLAYCLOCKDIV,
			0x80,
			SSD1306_SETMULTIPLEX
		}, {
			SSD1306_SETDISPLAYOFFSET,
			0x0,
			SSD1306_SETSTARTLINE | 0x0,
			SSD1306_CHARGEPUMP
		}, {
			SSD1306_MEMORYMODE,
			0x00,
			SSD1306_SEGREMAP | 0x1,
			SSD1306_COMSCANDEC
		}, {
			SSD1306_SETVCOMDETECT,
			0x40,
			SSD1306_DISPLAYALLON_RESUME,
			SSD1306_NORMALDISPLAY,
			SSD1306_DEACTIVATE_SCROLL,
			SSD1306_DISPLAYON
		},
	};

	if (width == 128 && height == 32) {
		com_pins = 0x02;
		contrast = 0x8F;
	} else if (width == 128 && height == 64) {
		com_pins = 0x12;
		contrast = 0xCF;
	} else if (width == 96 && height == 16) {
		com_pins = 0x02;
		contrast = 0xAF;
	} else {
		return -EINVAL;
	}

	display = (struct ssd1306*) dev->priv_data;
	i2c_dev = display->i2c_dev;

	display->height = height;
	display->width = width;
	dev->height = height;
	dev->width = width;

	if (display->buffer)
		free(display->buffer);

	display->buffer_size = width * ((height + 7) / 8);
	display->buffer = (uint8_t *)malloc(display->buffer_size);
	if (!display->buffer)
		return -ENOMEM;

	ssd1306_clear_display(dev);

	i2c_dev_burst_write(i2c_dev, 0x0, init_sequence[0], 4);
	i2c_dev_write(i2c_dev, 0x0, height - 1);

	i2c_dev_burst_write(i2c_dev, 0x0, init_sequence[1], 4);
	i2c_dev_write(i2c_dev, 0x0, 0x14);

	i2c_dev_burst_write(i2c_dev, 0x0, init_sequence[2], 4);

	i2c_dev_write(i2c_dev, 0x0, SSD1306_SETCOMPINS);
	i2c_dev_write(i2c_dev, 0x0, com_pins);
	i2c_dev_write(i2c_dev, 0x0, SSD1306_SETCONTRAST);
	i2c_dev_write(i2c_dev, 0x0, contrast);

	i2c_dev_write(i2c_dev, 0x0, SSD1306_SETPRECHARGE);
	i2c_dev_write(i2c_dev, 0x0, 0xF1);

	i2c_dev_burst_write(i2c_dev, 0x0, init_sequence[3], 6);

	return 0;
}

static struct display_device_ops ssd1306_display_ops = {
	.init			= ssd1306_display_init,
	.display		= ssd1306_display_buffer,
	.clear			= ssd1306_clear_display,
	.set_pixel		= ssd1306_set_pixel,
	.draw_horizontal_line	= ssd1306_draw_horizontal_line,
	.draw_vertical_line	= ssd1306_draw_vertical_line,
};

int ssd1306_driver_probe(struct i2c_dev *dev)
{
	struct display_device *display_device;
	struct ssd1306 *display;
	int err = 0;

	printf("SSD1306 Driver Probe\n");

	display_device = calloc(1, sizeof(struct display_device));
	if (!display_device)
		return -ENOMEM;

	display = calloc(1, sizeof(struct ssd1306));
	if (!display) {
		err = -ENOMEM;
		goto out;
	}

	display->i2c_dev = dev;
	display->display_device = display_device;
	
	display_device->name = "ssd1306";
	display_device->priv_data = (void*) display;
	display_device->ops = &ssd1306_display_ops;
	display_device_register(display_device);

out:
	if (err)
		free(display_device);

	return 0;
}

static struct i2c_dev_ops ssd1306_i2c_dev_ops = {
	.init = ssd1306_driver_probe,
};

int ssd1306_driver_init(void)
{
	struct i2c_dev *i2c_dev;

	i2c_dev = calloc(1, sizeof(struct i2c_dev));
	if (!i2c_dev)
		return -ENOMEM;

	i2c_dev->bus = i2c1;
	i2c_dev->addr = SSD1306_ADDR;
	i2c_dev->ops = &ssd1306_i2c_dev_ops;

	i2c_device_register(i2c_dev);

	return 0;
}
