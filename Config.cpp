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
		WriteAccounts(cfg);

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
	UserAccounts.clear();

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
		UserAccounts[acc.ID()] = acc;
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

void Config::WriteAccounts(IniFile& cfg)
{
	cfg.WriteKeyInt("Accounts_Count", UserAccounts.size());

	int i = 1;
	for (auto it = UserAccounts.begin(); it != UserAccounts.end(); ++it, ++i)
	{
		// write account data
		cfg.WriteKeyInt("Account_ID_" + to_string(i), it->first);
		cfg.WriteKey("Account_Username_" + to_string(i), it->second.Username());
		cfg.WriteKey("Account_Password_" + to_string(i), it->second.Password());

		WriteContacts(it->second, i, cfg);
		WriteMessages(it->second, i, cfg);
		WriteFavoriteMessages(it->second, i, cfg);
	}
}

void Config::WriteContacts(UserAccount& acc, int idx, IniFile& cfg)
{
	cfg.WriteKeyInt("Account_Contacts_Count_" + to_string(idx), acc.Contacts.size());

	int i = 1;
	for (auto it = acc.Contacts.begin(); it != acc.Contacts.end(); ++it, ++i)
		cfg.WriteKeyInt("Account_Contact_" + to_string(idx) + '_' + to_string(i), *it);
}

void Config::WriteMessages(UserAccount& acc, int idx, IniFile& cfg)
{
	size_t size = 0;
	int i = 1;
	Message msg;
	stack<Message> msgs;

	for (auto it = acc.Messages.begin(); it != acc.Messages.end(); ++it)
	{
		msgs = it->second;
		size += msgs.size();

		while (!msgs.empty())
		{
			msg = msgs.top();

			cfg.WriteKeyInt("Account_Message_Sender_" + to_string(idx) + '_' + to_string(i), it->first);

			cfg.WriteKeyInt("Account_Message_Index_" + to_string(idx) + '_' + to_string(i), msg.Index);
			cfg.WriteKey("Account_Message_Content_" + to_string(idx) + '_' + to_string(i), msg.Content);
			cfg.WriteKey("Account_Message_Date_" + to_string(idx) + '_' + to_string(i), DateToString(msg.SentDate));
			cfg.WriteKeyInt("Account_Message_Favorite_" + to_string(idx) + '_' + to_string(i), msg.IsFavorite);

			msgs.pop();
			++i;
		}
	}

	cfg.WriteKeyInt("Account_Messages_Count_" + to_string(idx), size);
}

void Config::WriteFavoriteMessages(UserAccount& acc, int idx, IniFile& cfg)
{
	int i = 1;
	pair<int, Message> favorite;
	queue<pair<int, Message>> favorites = acc.Favorites;

	cfg.WriteKeyInt("Account_Favorites_Count_" + to_string(idx), favorites.size());

	while (!favorites.empty())
	{
		favorite = favorites.front();

		cfg.WriteKeyInt("Account_Favorite_Sender_" + to_string(idx) + '_' + to_string(i), favorite.first);

		cfg.WriteKeyInt("Account_Favorite_MsgIndex_" + to_string(idx) + '_' + to_string(i), favorite.second.Index);
		cfg.WriteKey("Account_Favorite_Content_" + to_string(idx) + '_' + to_string(i), favorite.second.Content);
		cfg.WriteKey("Account_Favorite_Date_" + to_string(idx) + '_' + to_string(i), DateToString(favorite.second.SentDate));

		favorites.pop();
		++i;
	}
}

bool Config::AccountExists(const string& username)
{
	for (auto it = UserAccounts.begin(); it != UserAccounts.end(); ++it)
	{
		if (it->second.Username().compare(username) == 0)
			return true;
	}
	return false;
}

bool Config::AccountExists(const string& username, const string& pw)
{
	for (auto it = UserAccounts.begin(); it != UserAccounts.end(); ++it)
	{
		if (it->second.Username().compare(username) == 0 && it->second.Password().compare(pw) == 0)
			return true;
	}
	return false;
}

UserAccount* Config::GetUserAccount(int id)
{
	auto it = UserAccounts.find(id);
	return it != UserAccounts.end() ? &it->second : NULL;
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
