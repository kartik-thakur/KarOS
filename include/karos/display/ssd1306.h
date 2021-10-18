#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <karos/i2c/i2c-dev.h>
#include <karos/i2c/i2c-core.h>

#define SSD1306_ADDR		0x3C

struct ssd1306 {
	struct i2c_dev	*i2c_dev;
};

int ssd1306_driver_init(void);

#endif
