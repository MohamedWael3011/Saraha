#include "main.h"

#define CONFIG_FILE "data.ini"

Config cfg(CONFIG_FILE);

int main()
{
	cfg.Load();
	cfg.Save();
	return 0;
}
