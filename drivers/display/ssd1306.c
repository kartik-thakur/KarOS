#include <stdio.h>
#include <stdlib.h>
#include <karos/error.h>
#include <karos/display/display-device.h>
#include <karos/display/display-core.h>
#include <karos/display/ssd1306.h>

int ssd1306_driver_probe(struct i2c_dev *dev)
{
	struct display_device *display_device;
	int err = 0;

	printf("SSD1306 Driver Probe\n");

	display_device = calloc(1, sizeof(struct display_device));
	if (!display_device)
		return -ENOMEM;

	display_device->name = "ssd1306";
	display_device->priv_data = (void*) dev;
	display_device_register(display_device);
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
