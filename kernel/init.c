#include <stdio.h>
#include <karos/drivers.h>
#include <karos/userspace.h>
#include <pico/stdlib.h>

void karos_subsystem_init()
{
	drivers_init();
}

int main()
{
	/*
	 * TODO: power init sequence.
	 * Pull GPIO8 HIGH.
	 */
	gpio_init(9);
	gpio_set_dir(9, GPIO_OUT);
	gpio_put(9, 1);

	gpio_init(25);
	gpio_set_dir(25, GPIO_OUT);
	gpio_put(25, 1);

	stdio_init_all();
	sleep_ms(5000);

	karos_subsystem_init();
	app_main(0, NULL);

	return 0;
}
