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

			cout << User_ID<<" is already in your contacts list.\n";
		}
		else {

			Contacts.insert(User_ID);
			cout << User_ID << " has been added to your contacts list.\n";

		}
		return true;
	}

	return false;

}


bool UserAccount::RemoveContact(int User_ID) {

	if (Contacts.find(User_ID) != Contacts.end()) {
		Contacts.erase((User_ID));
		cout << User_ID << " has been removed from your contacts list.\n";
		return true;
	}
	return false;
}

bool UserAccount::SendUserMessage(UserAccount* recipient, string content)
{
	if (!recipient)
		return false;

	SYSTEMTIME time;
	GetSystemTime(&time);
	Message msg{ 0, content, time };
	
	// >> SENDER SIDE <<
	auto it = Messages.find(recipient->m_id);

	// if received msgs from this user before
	if (it != Messages.end())
	{
		msg.Index = (!it->second.first.empty() ? it->second.first.top().Index : 0) + 1;
		it->second.first.push(msg);
	}

	// first time to receive msg from this user
	else
	{
		pair<stack<Message>, stack<Message>> msgs;
		msgs.first.push(msg);

		Messages[recipient->m_id] = msgs;
	}

	
	// >> RECIPIENT/RECEIVER SIDE <<
	it = recipient->Messages.find(m_id);

	// if received msgs from this user before
	if (it != recipient->Messages.end())
	{
		msg.Index = (!it->second.second.empty() ? it->second.second.top().Index : 0) + 1;
		it->second.second.push(msg);
	}

	// first time to receive msg from this user
	else
	{
		pair<stack<Message>, stack<Message>> msgs;
		msgs.second.push(msg);

		recipient->Messages[m_id] = msgs;
	}
	return true;
}

bool UserAccount::PopMessage(UserAccount* user) {

	// Pop from sender side
	auto it = Messages.find(user->m_id);
	if (it == Messages.end() || it->second.first.empty()) {
		return false;
	}
	else {
		it->second.first.pop();
	}

	// Pop from receiver side
	it = user->Messages.find(m_id);
	if (it == user->Messages.end() || it->second.second.empty()) {
		return false;
	}
	else {
		it->second.second.pop();
		return true;
	}
}

void UserAccount::ViewContacts() { 
	set<int>::iterator itr;		   	
	vector<pair<int,int>>ContactTotalMessages;

	for (itr = Contacts.begin();itr != Contacts.end(); itr++) // Retrieving Contact's Sent Messages
	{
		int ContactID = *itr;
		int ContactMessages = Messages[ContactID].first.size();
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
		pair<stack<Message>, stack<Message>> UserMessages;
		unordered_map<int, pair<stack<Message>, stack<Message>>>::iterator it;

		for (it = Messages.begin(); it != Messages.end(); it++)
		{
			UserMessages = it->second;
			while (!UserMessages.second.empty())
			{
				AllMessages.emplace_back(UserMessages.second.top(), it->first);
				UserMessages.second.pop();
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
	pair<stack<Message>, stack<Message>> UserMessages;

	if (Messages.find(User_ID) == Messages.end())
	{
		return false;
	}
	else
	{
		UserMessages = Messages[User_ID];
		while (!UserMessages.second.empty())
		{

			cout << UserMessages.second.top().Index << "| " << UserMessages.second.top().Content<<"  ";
			PrintDate(UserMessages.second.top().SentDate);
			UserMessages.second.pop();
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
	return Messages[user->m_id].second.size();

}
bool UserAccount::GetContact(int User_ID) {
	auto it = Contacts.find(User_ID);
	if (it != Contacts.end()) {
		return true;
	}
	return NULL;
}
