#include <stdio.h>
#include <karos/userspace.h>
#include <karos/thermal/thermal-core.h>
#include <karos/thermal/thermal-device.h>
#include <lib/display/lib-display.h>
#include <pico/stdlib.h>

#include "temperature_monitor.h"

int app_main(int argc, char **argv)
{
	struct display_device *display;
	struct thermal_device *thermal_device;
	int16_t bitmap_x, bitmap_y;
	int32_t temperature = 0;

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

	sleep_ms(2000);
	display_clear(display);
	display_draw_horizontal_line(display, 64, 16, 20, 1);
	display_print_buffer(display);

	sleep_ms(2000);
	display_clear(display);
	display_draw_vertical_line(display, 64, 16, 20, 1);
	display_print_buffer(display);

	sleep_ms(2000);
	display_clear(display);
	display_draw_circle(display, 64, 16, 10, 1);
	display_print_buffer(display);

	sleep_ms(2000);
	display_clear(display);
	display_fill_rectangle(display, 10, 10, 40, 10, 1);
	display_print_buffer(display);

	sleep_ms(2000);
	display_clear(display);
	display->textsize_x = 3;
	display->textsize_y = 4;
	display->textcolor = 1;
	display->textbgcolor = 1;
	display->wrap = true;
	display->_cp437 = false;
	display->font = NULL;
	display_write(display, 'k');
	display_write(display, 'a');
	display_write(display, 'r');
	display_write(display, 't');
	display_write(display, 'i');
	display_write(display, 'k');
	display_print_buffer(display);

	thermal_device = get_thermal_device_by_name("bmp280");
	if (!thermal_device)
		printf("Unable to find bmp280 device\n");

	printf("Default Splash Screen Set\n");
	while(true) {
		if (thermal_device) {
			printf("Getting temperature\n");
			thermal_device->ops->get_temperature(
					thermal_device->priv_data,
					&temperature);
			printf("Temperature: %d\n", temperature);
		}
		sleep_ms(1000);
	}
}
