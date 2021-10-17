#include <stdio.h>
#include <karos/drivers.h>
#include <pico/stdlib.h>

#include <karos/thermal/thermal-device.h>
#include <karos/thermal/thermal-core.h>

void karos_subsystem_init()
{
	drivers_init();
}

int main()
{
	struct thermal_device *therm_dev;

	stdio_init_all();
	sleep_ms(5000);

	karos_subsystem_init();

	therm_dev = get_thermal_device_by_name("bmp280");
	if (!therm_dev) {
		printf("Thermal device not present\n");
		return 0;
	}

	while(true) {
		int32_t temperature = 0;

		therm_dev->ops->get_temperature(therm_dev->priv_data,
				&temperature);
		printf("Temperature %d\n", temperature);
		sleep_ms(1000);
	}

	return 0;
}
