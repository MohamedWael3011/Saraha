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
	unordered_map<int, pair<stack<Message>, stack<Message>>> Messages;
	queue<pair<int, Message>> Favorites;

public:
	UserAccount();
	UserAccount(int id, string username, string pw);

	void Set(int id, string username, string pw);

	bool AddContact(int User_ID);
	bool RemoveContact(int User_ID);
	bool SendUserMessage(UserAccount* recipient, string content);
	bool PopMessage(UserAccount* user);
	void ViewContacts(); //ordered by most messages
	void ViewMessages();
	bool ViewMessages(int User_ID);
	bool PutFavorite(int User_ID, int Msg_Index);
	bool RemoveOldestFavorite(int User_ID);
	bool ViewFavorites();
	int GetMessagesFromUser(UserAccount* user);
	bool GetContact(int User_ID);
	int ID() const;
	string Username() const;
	string Password() const;
};