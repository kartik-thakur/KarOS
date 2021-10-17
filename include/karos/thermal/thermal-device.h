#ifndef __THERMAL_DEVICE_H__
#define __THERMAL_DEVICE_H__

struct thermal_device_ops {
	int (*get_temperature)(void* data, int32_t *value);
};

struct thermal_device {
	char				*name;
	void				*priv_data;
	struct thermal_device_ops	*ops;
};

#endif
