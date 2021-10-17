#include <stdio.h>
#include <karos/thermal/thermal-core.h>

/*
 * Include thermal drivers header to call their init functions.
 */
#include <karos/thermal/bmp280.h>

void thermal_drivers_init(void)
{
	int err;

	err = bmp280_driver_init();
	if (err)
		printf("BMP280 driver init failed\n");
}
