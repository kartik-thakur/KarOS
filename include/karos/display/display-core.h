#ifndef __DISPLAY_CORE_H__
#define __DISPLAY_CORE_H__

#include <karos/display/display-device.h>

int display_device_register(struct display_device *dev);
int display_drivers_init(void);
struct display_device* get_display_device_by_name(char *name);

#endif
