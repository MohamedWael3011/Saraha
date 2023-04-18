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

bool Register(string Username, string Password)
{
	if (cfg.AccountExists(Username))
		return false;
	else
	{
		int new_id = cfg.UserAccounts.size() + 1;
		UserAccount new_acc(new_id, Username, Password);
		cfg.UserAccounts[new_acc.ID()] = new_acc;
		return true;
	}
}

bool Login(string Username, string Password)
{
	if (cfg.AccountExists(Username, Password))
		return true;
	else
		return false;
}

int main()
{
	// Load configuration on startup
	cfg.Load();

	// Testing configuration saving
	//TestAccountToConfig();

	//Testing Register
	/*if (Register("Karim", "123123"))
		cout << "Yes\n";
	else
		cout << "No\n";*/

	//Testing Login
	/*if (Login("Karim", "123123"))
		cout << "Yes\n";
	else
		cout << "No\n";*/

	// Save configuration on exit (not finished yet)
	cfg.Save();
	return 0;
}


