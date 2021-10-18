#include <karos/drivers.h>
#include <karos/display/display-core.h>
#include <karos/i2c/i2c-core.h>
#include <karos/thermal/thermal-core.h>

void drivers_init(void)
{
	display_drivers_init();
	thermal_drivers_init();

	/*
	 * Probe devices connected to i2c and spi in the end.
	 * By that time devices are already registered with their
	 * native framework.
	 */
	i2c_drivers_init();
}
