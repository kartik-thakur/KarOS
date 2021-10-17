#include <stdio.h>
#include <karos/drivers.h>

void karos_subsystem_init()
{
	drivers_init();
}

int main()
{
	return 0;
}
