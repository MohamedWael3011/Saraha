#include "Config.h"

#include <fstream>
#include <sstream>

Config::Config(const char* filename) : m_loaded(false), m_file(filename)
{
}

Config::~Config(void)
{
}

bool Config::Load(void)
{
	try
	{
		// Load ini file
		IniFile cfg;
		if (!GetConfigFile(cfg))
			return false;

		// Load config
		int accscnt = cfg.ReadKeyInt("Accounts_Count");

		for (int i = 0; i < accscnt; ++i)
		{

		}

		m_loaded = true;
		return true;
	}
	catch (exception& ex)
	{
		printf("[Config] Failed to load: %s\n", ex.what());
	}
	return false;
}

bool Config::Save(void)
{
	try
	{
		// Load ini file
		IniFile cfg(false);
		if (!GetConfigFile(cfg))
			return false;

		printf("Loaded: [%s]\n", cfg["Accounts_Count"].c_str());

		// Save new config
		cfg.WriteKey("Accounts_Count", "15");
		cfg.WriteKey("Account_Name", "Ahmed");
		cfg.WriteKey("Account_ID2", "5010");

		// Commit
		SaveConfigFile(cfg);
		return true;
	}
	catch (exception& ex)
	{
		printf("[Config] Failed to save: %s\n", ex.what());
	}
	return false;
}

bool Config::GetConfigFile(IniFile& ini) const
{
	ifstream cfg(m_file);
	string data;

	if (cfg)
	{
		stringstream buffer;
		buffer << cfg.rdbuf();
		ini.SetData(buffer.str());
		return true;
	}

	return false;
}

bool Config::SaveConfigFile(IniFile& ini)
{
	ofstream cfg(m_file);
	if (cfg)
	{
		if (const char* data = ini.Data())
		{
			cfg.write(data, strlen(data));
			return true;
		}
	}
	return false;
}

bool Config::AccountExists(const string& username)
{
	for (auto it = m_useraccounts.begin(); it != m_useraccounts.end(); ++it)
	{
		if (it->second.Username.compare(username) == 0)
			return true;
	}
	return false;
}

bool Config::AccountExists(const string& username, const string& pw)
{
	for (auto it = m_useraccounts.begin(); it != m_useraccounts.end(); ++it)
	{
		if (it->second.Username.compare(username) == 0 && it->second.Password.compare(pw) == 0)
			return true;
	}
	return false;
}

UserAccount* Config::GetUserAccount(int id)
{
	auto it = m_useraccounts.find(id);
	return it != m_useraccounts.end() ? &it->second : NULL;
}
