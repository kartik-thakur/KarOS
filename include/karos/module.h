#ifndef __MODULE_H__
#define __MODULE_H__

struct karos_module {
	char		*name;
	int		(*init)(void);
	void		(*exit)(void);
};

#endif
