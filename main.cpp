#include "main.h"
#include <chrono>
#include <ctime>

#define CONFIG_FILE "data.ini"

Config cfg(CONFIG_FILE);
UserAccount* current_user = NULL;

void TestingViewContact()
{
	UserAccount acc(11111, "Wael", "12345");
	set<int>temp = { 1,2,3,4,5 };
	acc.Contacts = temp;
	Message m = { 0,"Test",time(NULL),false};
	Message m1 = { 1,"Test",time(NULL),false };
	Message m2 = { 2,"Test",time(NULL),false };

	acc.Messages[1].push(m);
	acc.Messages[1].push(m1);
	acc.Messages[1].push(m2);
	acc.Messages[2].push(m);
	acc.Messages[2].push(m1);
	acc.Messages[3].push(m);
	acc.ViewContacts();
}

// Main functions
bool Register(string Username, string Password);
UserAccount* Login(string Username, string Password);
void LoginRegisterMenu();
void HomeScreen();
void MessageScreen();
void ContactScreen();
void FavoriteScreen();
void OnExit();


//--------------------------------------------------------------------------

int main()
{
	// Load configuration on startup
	cfg.Load();

	//--------------------------------------------------------------------------
	/*
	Karim: This next section is me testing how the program would run on console
	You can comment it if you don't like it or you wanna modify stuff	
	*/


	
	//--------------------------------------------------------------------------

	// Save configuration on exit (not finished yet)
	LoginRegisterMenu();

	atexit(OnExit);
	return 0;
}

void OnExit()
{
	cfg.Save();
}


//--------------------------------------------------------------------------

bool Register(string Username, string Password)
{
	if (cfg.AccountExists(Username) == NULL)
	{
		int new_id = cfg.PopNextAccountID();
		UserAccount new_acc(new_id, Username, Password);
		cfg.UserAccounts[new_acc.ID()] = new_acc;
		return true;
	}
	else
		return false;
}

UserAccount* Login(string Username, string Password)
{
	if (cfg.AccountExists(Username, Password) == NULL)
		return NULL;
	else
		return cfg.AccountExists(Username, Password);
}

void ClearConsole()
{
	system("cls");
}

void LoginRegisterMenu()
{
	ClearConsole();

	cout << "----------------- Welcome to Saraha -----------------\n";
	cout << "1. LOGIN\n2. REGISTER\n3. EXIT\n\nPlease choose an option: ";
	int choice;
	cin >> choice;
	string username, password;
	switch (choice)
	{
	case 1:
	{
		ClearConsole();
		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter Password: ";
		cin >> password;

		if (Login(username, password) == NULL)
		{
			cout << "Invalid credentials, please try again. \n";
			system("pause");
			LoginRegisterMenu();
		}
		else
		{
			current_user = Login(username, password);
			cout << "Login Successful\n";
			HomeScreen();
			system("pause");
		}
		break;
	}

	case 2:
	{
		ClearConsole();

		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter Password: ";
		cin >> password;
		char confirm;
		cout << "Are you sure you want to register an account with username " << username << " and password " << password << " > y/n\n";
		cin >> confirm;
		if (tolower(confirm) == 'n')
		{
			LoginRegisterMenu();
		}
		else
		{
			if (Register(username, password))
			{
				cout << "\Account Created\n";
				cout << "\Please login.\n";
				system("pause");
			}
			else
			{
				cout << "Error: Account already exists\n";
				system("pause");
				LoginRegisterMenu();
			}
		}
		break;
	}

	case 3:
		return;

	default:
		cout << "Please enter a valid choice\n";
		system("pause");
		LoginRegisterMenu();

	}
}

void HomeScreen()
{	
	ClearConsole();
	cout << "1. View messages.\n2. View contacts.\n3. View favorite messages.\n4. Logout.\nPlease select a choice\n";
	int choice;
	cin >> choice;
	switch (choice)
	{
	case 1:
		MessageScreen();
		break;
	case 2:
		ContactScreen();
		break;

	case 3:
		FavoriteScreen();
		break;
	case 4:
		LoginRegisterMenu();
		break;
	default:
		cout << "Please enter a valid choice\n";
		system("pause");
		HomeScreen();
	}
}
void MessageScreen() 
{	
	ClearConsole();
	cout << "1. Send a message.\n2. Pop last message.\n3. View all messages.\n4. View messages from a specific user.\n5. Return home screen.\n";
	int choice;
	cin >> choice;

	switch (choice)
	{
	case 1:
	{
		cout << "Please enter the user ID to send him/her a message\n";
		int ID;
		cin >> ID;

		cout << "Please enter the message you'd like to send\n";
		string msg;
		cin >> msg;

		UserAccount* receiver = cfg.GetUserAccount(ID);
		if (receiver != NULL)
		{
			receiver->ReceiveMessage(current_user->ID(), msg);
			cout << "Message has been sent.\n";
		}
		else
			cout << "User was not found.\n";
		break;
	}

	case 2:
	{
		cout << "Please enter the user ID to pop last message you sent him/her\n";
		int ID;
		cin >> ID;
		UserAccount* receiver = cfg.GetUserAccount(ID);
		if (receiver != NULL)
		{
			bool result = receiver->PopMessage(current_user->ID());
			if (result) {
				cout << "Message has been popped.\n";
			}
			else {
				cout << "No messages to delete.\n";

			}
		}
		else
			cout << "User was not found.\n";

		break;
	}

	case 3:
		cout << "Here is your inbox from most recent to oldest:\n";
		current_user->ViewMessages();
		//Add Adding to Favorite Logic
		break;

	case 4:
	{
		cout << "Please enter the user ID to display their messages\n";
		int ID;
		cin >> ID;
		current_user->ViewMessages(ID);
		//Add Adding to Favorite Logic
		break;
	}

	case 5:
		HomeScreen();
		break;

	default:
		cout << "Please enter a valid choice\n";
		system("pause");
		MessageScreen();
	}
}

void ContactScreen()
{	
	ClearConsole();
	current_user->ViewContacts();
	cout << "------------------------------------------------------\n\n";
	cout << "Enter User ID to view their messages.\nTo return home, press 0\n";
	int ID;
	cin >> ID;
	if (!ID)
		HomeScreen();
	else {
		if (current_user->ViewMessages(ID)) // Momkn n5liha boolean;
		{

		}
		else {
			cout << "Invalid user ID. Please, try again.\n";
			system("pause");
			ContactScreen();
		}
	}
}
void FavoriteScreen() {

}
