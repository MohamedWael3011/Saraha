#include "Config.h"
#include "Misc.h"

#include <fstream>
#include <sstream>

Config::Config(const char* filename) : m_loaded(false), m_file(filename), m_nextaccountid(1)
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
		LoadAccounts(cfg);

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

void Config::LoadAccounts(IniFile& cfg)
{
	m_useraccounts.clear();

	UserAccount acc;
	int accscnt = cfg.ReadKeyInt("Accounts_Count");

	for (int i = 1; i <= accscnt; ++i)
	{
		// load account data
		acc.Set(
			cfg.ReadKeyInt("Account_ID_" + to_string(i)),
			cfg.ReadKey("Account_Username_" + to_string(i)),
			cfg.ReadKey("Account_Password_" + to_string(i))
		);

		LoadContacts(acc, i, cfg);
		LoadMessages(acc, i, cfg);
		LoadFavoriteMessages(acc, i, cfg);

		if (i == accscnt)
			m_nextaccountid = acc.ID() + 1;
		m_useraccounts[acc.ID()] = acc;
	}
}

void Config::LoadContacts(UserAccount& acc, int idx, IniFile& cfg)
{
	// clear
	acc.Contacts.clear();

	// load
	int contactscnt = cfg.ReadKeyInt("Account_Contacts_Count_" + to_string(idx));
	for (int i = 1; i <= contactscnt; ++i)
		acc.Contacts.insert(cfg.ReadKeyInt("Account_Contact_" + to_string(idx) + '_' + to_string(i)));
}

void Config::LoadMessages(UserAccount& acc, int idx, IniFile& cfg)
{
	// clear
	acc.Messages.clear();

	// load
	Message msg;
	int msgcnt = cfg.ReadKeyInt("Account_Messages_Count_" + to_string(idx));

	for (int i = 1; i <= msgcnt; ++i)
	{
		msg.Index = cfg.ReadKeyInt("Account_Message_Index_" + to_string(idx) + '_' + to_string(i));
		msg.Content = cfg.ReadKey("Account_Message_Content_" + to_string(idx) + '_' + to_string(i));
		msg.SentDate = StringToDate(cfg.ReadKey("Account_Message_Date_" + to_string(idx) + '_' + to_string(i)));
		msg.IsFavorite = cfg.ReadKeyInt("Account_Message_Favorite_" + to_string(idx) + '_' + to_string(i));

		AppendMessage(acc, cfg.ReadKeyInt("Account_Message_Sender_" + to_string(idx) + '_' + to_string(i)), msg);
	}
}

void Config::LoadFavoriteMessages(UserAccount& acc, int idx, IniFile& cfg)
{
	// clear
	queue<pair<int, Message>>().swap(acc.Favorites);

	// load
	Message msg;
	msg.IsFavorite = true;

	int msgcnt = cfg.ReadKeyInt("Account_Favorites_Count_" + to_string(idx));

	for (int i = 1; i <= msgcnt; ++i)
	{
		msg.Index = cfg.ReadKeyInt("Account_Favorite_MsgIndex_" + to_string(idx) + '_' + to_string(i));
		msg.Content = cfg.ReadKey("Account_Favorite_Content_" + to_string(idx) + '_' + to_string(i));
		msg.SentDate = StringToDate(cfg.ReadKey("Account_Favorite_Date_" + to_string(idx) + '_' + to_string(i)));

		acc.Favorites.push(pair<int, Message>(cfg.ReadKeyInt("Account_Favorite_Sender_" + to_string(idx) + '_' + to_string(i)), msg));
	}
}

bool Config::AccountExists(const string& username)
{
	for (auto it = m_useraccounts.begin(); it != m_useraccounts.end(); ++it)
	{
		if (it->second.Username().compare(username) == 0)
			return true;
	}
	return false;
}

bool Config::AccountExists(const string& username, const string& pw)
{
	for (auto it = m_useraccounts.begin(); it != m_useraccounts.end(); ++it)
	{
		if (it->second.Username().compare(username) == 0 && it->second.Password().compare(pw) == 0)
			return true;
	}
	return false;
}

UserAccount* Config::GetUserAccount(int id)
{
	auto it = m_useraccounts.find(id);
	return it != m_useraccounts.end() ? &it->second : NULL;
}

int Config::PopNextAccountID(void)
{
	return m_nextaccountid++;
}

void Config::AppendMessage(UserAccount& acc, int senderid, Message& msg)
{
	auto it = acc.Messages.find(senderid);
	if (it != acc.Messages.end())
		it->second.push(msg);
	else
	{
		stack<Message> msgs;
		msgs.push(msg);

		acc.Messages[senderid] = msgs;
	}
}
