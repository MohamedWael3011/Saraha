#include "UserAccount.h"

UserAccount::UserAccount(void)
{
}

UserAccount::UserAccount(int id, string username, string pw)
{
	Set(id, username, pw);
}

void UserAccount::Set(int id, string username, string pw)
{
	m_id = id;
	m_username = username;
	m_password = pw;
}

int UserAccount::ID(void) const
{
	return m_id;
}

string UserAccount::Username(void) const
{
	return m_username;
}

string UserAccount::Password(void) const
{
	return m_password;
}
bool UserAccount::AddContact(int User_ID) {

	return false;
}

bool UserAccount::RemoveContact(int User_ID) {
	return false;
}

void UserAccount::ReceiveMessage(int Sender_ID, string content)
{
	SYSTEMTIME time;
	GetSystemTime(&time);

	Message msg;
	msg.Content = content;
	msg.SentDate = time;

	auto it = Messages.find(Sender_ID);

	// if received msgs from this user before
	if (it != Messages.end())
	{
		msg.Index = (!it->second.empty() ? it->second.top().Index : 0) + 1;
		it->second.push(msg);
	}

	// first time to receive msg from this user
	else
	{
		msg.Index = 0;

		stack<Message> msgs;
		msgs.push(msg);

		Messages[Sender_ID] = msgs;
	}
}

bool UserAccount::PopMessage(int User_ID) {

	auto it = Messages.find(User_ID);
	if (it == Messages.end() || it->second.empty()) {
		
		return false;
	}
	else {
		it->second.pop();
		return true;
	}

}

void UserAccount::ViewContacts() { 
	set<int>::iterator itr;		   	
	vector<pair<int,int>>ContactTotalMessages;

	for (itr = Contacts.begin();itr != Contacts.end(); itr++) // Retrieving Contact's Sent Messages
	{
		int ContactID = *itr;
		int ContactMessages= Messages[ContactID].size();
		ContactTotalMessages.push_back(make_pair(ContactMessages, ContactID)); // Storing as (Messages,ID) for easier sort lol :')
	}
	if (ContactTotalMessages.empty()) {
		cout << "Oh no, you don't have any contacts. :(" << endl;
	}
	else {
		sort(ContactTotalMessages.begin(), ContactTotalMessages.end(), greater<>());
		for (auto i : ContactTotalMessages) {
			cout << "Contact with ID " << i.second << ": " << i.first << "  Message" << endl;
		}
	}
}

void UserAccount::ViewMessages() {
	
}

bool UserAccount::ViewMessages(int User_ID) {
	return false;

}
bool UserAccount::PutFavorite(int User_ID, int Msg_Index) {
	return false;

}
bool UserAccount::RemoveOldestFavorite(int User_ID) {
	return false;

}
void UserAccount::ViewFavorites() {

}
int UserAccount::GetMessagesFromUser(UserAccount* user) {
	return 0;

}
UserAccount* UserAccount::GetContact(int User_ID) {
	return NULL;
}
