#pragma once
#include "essentials.h"
#include "Message.h"

class UserAccount
{
private:
	int m_id;
	string m_username;
	string m_password;
public:
	set<int> Contacts;
	unordered_map<int, stack<Message>> Messages;
	queue<pair<int, Message>> Favorites;

public:
	UserAccount();
	UserAccount(int id, string username, string pw);

	void Set(int id, string username, string pw);

	int ID() const;
	string Username() const;
	string Password() const;
};