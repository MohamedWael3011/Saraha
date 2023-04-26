#include "UserAccount.h"
#include "Misc.h"

void PrintDate(SYSTEMTIME d)
{
	cout << d.wDay << "-" << d.wMonth << "-" << d.wYear << " " << d.wHour << ":" << d.wMinute << endl;
}
bool compareByTime(const pair<Message, int>& a, const pair<Message, int>& b) {
	FILETIME aFileTime, bFileTime;
	SystemTimeToFileTime(&a.first.SentDate, &aFileTime);
	SystemTimeToFileTime(&b.first.SentDate, &bFileTime);
	return CompareFileTime(&aFileTime, &bFileTime) < 0;
}
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

	if (Messages.find(User_ID) != Messages.end()) {

		if (Contacts.find(User_ID) != Contacts.end()) {

			cout << "this contact has already been added";
		}
		else {

			Contacts.insert(User_ID);
		}
	}
	else
	{
		cout << "this contact should send at least one message to be added to the Contacts";
	}

}


bool UserAccount::RemoveContact(int User_ID) {

	if (Contacts.find(User_ID) != Contacts.end()) {

		Contacts.erase((User_ID);
	}
	else {

		cout << "this contact is not exist";
	}
}

void UserAccount::ReceiveMessage(int Sender_ID, string content)
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	Message msg{ 0, content, time };

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
	if (Messages.empty() == true)
	{
		cout << "No messages to be displayed" << endl;
	}
	else
	{
		vector <pair<Message, int>> AllMessages;
		stack <Message> UserMessages;
		unordered_map<int, stack<Message>>::iterator it;

		for (it = Messages.begin(); it != Messages.end(); it++)
		{
			UserMessages = it->second;
			while (!UserMessages.empty())
			{
				AllMessages.emplace_back(UserMessages.top(), it->first);
				UserMessages.pop();
			}
		}
		std::sort(AllMessages.begin(), AllMessages.end(), compareByTime);

		for (int i = 0; i < AllMessages.size(); i++)
		{
			cout << AllMessages[i].first.Index << "| User ID: " << AllMessages[i].second << "| " << AllMessages[i].first.Content << "  ";
			PrintDate(AllMessages[i].first.SentDate);
		}
	}
}
bool UserAccount::ViewMessages(int User_ID) {
	stack <Message> UserMessages;

	if (Messages.find(User_ID) == Messages.end())
	{
		return false;
	}
	else
	{
		UserMessages = Messages[User_ID];
		while (!UserMessages.empty())
		{

			cout << UserMessages.top().Index << "| " << UserMessages.top().Content<<"  ";
			PrintDate(UserMessages.top().SentDate);
			UserMessages.pop();
		}
		return true;
	}

}
bool UserAccount::PutFavorite(int User_ID, int Msg_Index) {
	return false;

}
bool UserAccount::RemoveOldestFavorite(int User_ID) {
	return false;

}

bool UserAccount::ViewFavorites(void)
{
	if (Favorites.empty())
	{
		cout << "No favorite messages to display.\n";
		return false;
	}

	queue<pair<int, Message>> msgs = Favorites;
	pair<int, Message> msg;

	cout << "\n\n>>>>>>>>>> YOUR FAVORITE MESSEAGES <<<<<<<<<<\n\n";

	while (!msgs.empty())
	{
		msg = msgs.front();

		cout << "[" << DATE2STR(msg.second.SentDate) << "] "
			<< to_string(msg.first)
			<< (GetContact(msg.first) ? " (contact): " : ": ")
			<< msg.second.Content << '\n';

		msgs.pop();
	}
	return true;
}

int UserAccount::GetMessagesFromUser(UserAccount* user) {
	return Messages[user->m_id].size();

}
bool UserAccount::GetContact(int User_ID) {
	auto it = Contacts.find(User_ID);
	if (it != Contacts.end()) {
		return true;
	}
	return NULL;
}
