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

}

bool UserAccount::RemoveContact(int User_ID) {

}
void UserAccount::SendMessage(int User_ID, string content) {

}
bool UserAccount::PopMessage(int User_ID) {

}
void UserAccount::ViewContacts() { //ordered by most messages
}
void UserAccount::ViewMessages() {

}
void UserAccount::ViewMessages(int User_ID) {

}
bool UserAccount::PutFavorite(int User_ID, int Msg_Index) {

}
bool UserAccount::RemoveOldestFavorite(int User_ID) {

}
void UserAccount::ViewFavorites() {

}
int UserAccount::GetMessagesFromUser(UserAccount* user) {

}
UserAccount* UserAccount::GetContact(int User_ID) {

}
