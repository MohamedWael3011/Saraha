#include "main.h"
#include <chrono>
#include <ctime>

#define CONFIG_FILE "data.ini"

Config cfg(CONFIG_FILE);

void TestAccountToConfig()
{
	UserAccount acc(50, "muhab", "12345666");
	cfg.UserAccounts[acc.ID()] = acc;

	// Save configuration on exit (not finished yet)
	cfg.Save();
}

int main()
{
	// Load configuration on startup
	cfg.Load();

	// Testing configuration saving
	//TestAccountToConfig();

	// Save configuration on exit (not finished yet)
	cfg.Save();
	return 0;
}
