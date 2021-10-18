#include <stdio.h>
#include <karos/userspace.h>
#include <karos/display/display-core.h>
#include <karos/display/display-device.h>
#include <pico/stdlib.h>

#include "temperature_monitor.h"

int app_main(int argc, char **argv)
{
	struct display_device *display;
	int16_t bitmap_x, bitmap_y;

	display = get_display_device_by_name("ssd1306");
	if (!display)
		printf("Unable to find attached display device\n");

	bitmap_x = (128 - splash_data[1].width) / 2;
	bitmap_y = (32 - splash_data[1].height) / 2;

	display_init(display, 128, 32);
	display_clear(display);
	display_print_buffer(display);
	display_draw_bitmap(display, bitmap_x, bitmap_y, splash_data[1].data,
			    splash_data[1].width, splash_data[1].height, 1);
	display_print_buffer(display);

	printf("Splash Screen Set\n");
	while(true) {
		sleep_ms(1000);
	}
}
