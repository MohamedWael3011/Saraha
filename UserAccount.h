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
	bool AddContact(int User_ID);
	bool RemoveContact(int User_ID);
	void SendMessage(int User_ID, string content);
	bool PopMessage(int User_ID);
	void ViewContacts(); //ordered by most messages
	void ViewMessages();
	void ViewMessages(int User_ID);
	bool PutFavorite(int User_ID, int Msg_Index);
	bool RemoveOldestFavorite(int User_ID);
	void ViewFavorites();
	int GetMessagesFromUser(UserAccount* user);
	UserAccount* GetContact(int User_ID);
	int ID() const;
	string Username() const;
	string Password() const;
};