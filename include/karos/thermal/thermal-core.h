#ifndef __THERMAL_CORE_H__
#define __THERMAL_CORE_H__

#include <karos/thermal/thermal-device.h>

void thermal_drivers_init(void);
int thermal_device_register(struct thermal_device *dev);
struct thermal_device* get_thermal_device_by_name(char *name);

#endif
