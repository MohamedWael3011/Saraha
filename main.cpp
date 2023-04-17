#include "main.h"
#include <chrono>
#include <ctime>

#define CONFIG_FILE "data.ini"

Config cfg(CONFIG_FILE);

int main()
{
	// Load configuration on startup
	cfg.Load();

	// Save configuration on exit (not finished yet)
	//cfg.Save();
	return 0;
}
