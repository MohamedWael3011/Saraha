#pragma once
#include "essentials.h"
#include "IniFile.h"
#include "UserAccount.h"

class Config
{
private:
	bool m_loaded;
	string m_file;

	int m_nextaccountid;
	map<int, UserAccount> m_useraccounts;

public:
	Config(const char* filename);
	~Config();

	bool Load();
	bool Save();

private:
	bool GetConfigFile(IniFile& ini) const;
	bool SaveConfigFile(IniFile& ini);

public:
	bool AccountExists(const string& username);
	bool AccountExists(const string& username, const string& pw);

	UserAccount* GetUserAccount(int id);
};