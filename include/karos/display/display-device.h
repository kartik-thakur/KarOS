#ifndef __DISPLAY_DEVICE_H__
#define __DISPLAY_DEVICE_H__

struct display_device_ops {
	int		(*init)(void* priv_data);
};

struct display_device {
	char				*name;
	void				*priv_data;

	struct display_device_ops	*ops;
};

#endif
