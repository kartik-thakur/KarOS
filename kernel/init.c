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
	stdio_init_all();
	sleep_ms(5000);

	karos_subsystem_init();
	app_main(0, NULL);

	return 0;
}
