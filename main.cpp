#include "main.h"
#include <chrono>
#include <ctime>
#include <conio.h>

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
string PasswordCensoring();
bool PasswordTypoChecker(string pass1, string pass2);
void OnExit();


//--------------------------------------------------------------------------

int main()
{
	// Load configuration on startup
	cfg.Load();


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

string PasswordCensoring()
{
	string passw = "";
	char ch;
	ch = _getch();
	while (ch != 13) //Equivalent of Pressing "Enter"
	{
		if (ch != 8) //Equivalent of Pressing "Backspace"
		{
			passw.push_back(ch);
			cout << "*";
		}
		else
		{
			if (!passw.empty())
			{
				passw.pop_back();
				cout << "\b" << " " << "\b";
			}
		}
		ch = _getch();
	}
	return passw;
}
bool PasswordTypoChecker(string pass1, string pass2) 
{
	if (pass1.compare(pass2) == 0)
		return true;
	else
		return false;
}

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

void SystemPause()
{
	system("pause");
}

void LoginRegisterMenu()
{
	ClearConsole();

	cout << "----------------- Welcome to Saraha -----------------\n";
	cout << "1. LOGIN\n2. REGISTER\n3. EXIT\n\nPlease choose an option: ";
	char choice;
	cin >> choice;
	string username, password, password_check;
	switch (choice)
	{
	case '1':
	{
		ClearConsole();
		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter password: ";
		password = PasswordCensoring();

		if (Login(username, password) == NULL)
		{
			cout << "\n\nError: Invalid credentials, please try again. \n";
			SystemPause();
			LoginRegisterMenu();
		}
		else
		{
			current_user = Login(username, password);
			cout << "\n\nLogin Successful\n";
			SystemPause();
			HomeScreen();
		}
		break;
	}

	case '2':
	{
		ClearConsole();

		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter Password: ";
		password = PasswordCensoring();
		cout << "\nRe-enter your password: ";
		password_check = PasswordCensoring();

		if (!PasswordTypoChecker(password, password_check))
		{
			cout << "\n\nError: Passwords do not match\n";
			SystemPause();
			LoginRegisterMenu();
		}
		else
		{
			if (Register(username, password))
			{
				cout << "\n\nAccount Created\n";
				cout << "Please login.\n";
				SystemPause();
				LoginRegisterMenu();
				;
			}
			else
			{
				cout << "\n\nError: Account already exists\n";
				SystemPause();
				LoginRegisterMenu();
			}
		}
		break;
	}

	case '3':
		return;

	default:
		cout << "\n\nError: Please enter a valid choice\n";
		SystemPause();
		LoginRegisterMenu();
	}
}

void HomeScreen()
{	
	ClearConsole();
	cout << "1. View messages.\n2. View contacts.\n3. View favorite messages.\n4. Logout\n5. Exit.\n\nPlease select a choice: ";
	char choice;
	cin >> choice;
	switch (choice)
	{
	case '1':
		MessageScreen();
		break;
	case '2':
		ContactScreen();
		break;

	case '3':
		FavoriteScreen();
		break;
	case '4':
		LoginRegisterMenu();
		break;
	case '5':
		return;
	default:
		cout << "Please enter a valid choice\n";
		SystemPause();
		HomeScreen();
	}
}
void MessageScreen() 
{	
	ClearConsole();
	cout << "1. Send a message.\n2. Pop last message.\n3. View all messages.\n4. View messages from a specific user.\n5. Return home screen.\n";
	char choice;
	cin >> choice;

	switch (choice)
	{
	case '1':
	{
		cout << "Please enter the user ID to send him/her a message\n";
		int ID;
		cin >> ID;

		cout << "Please enter the message you'd like to send\n";
		cin.ignore();
		string msg;
		getline(cin, msg, '\n');

		UserAccount* receiver = cfg.GetUserAccount(ID);
		if (receiver != NULL)
		{
			receiver->ReceiveMessage(current_user->ID(), msg);
			cout << "Message has been sent.\n";
		}
		else
			cout << "User was not found.\n";
		system("pause");
		MessageScreen();
		break;
	}

	case '2':
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
		system("pause");
		MessageScreen();
		break;
	}

	case '3':
		cout << "Here is your inbox from most recent to oldest:\n";
		current_user->ViewMessages();
		//Add Adding to Favorite Logic
		break;

	case '4':
	{
		cout << "Please enter the user ID to display their messages\n";
		int ID;
		cin >> ID;
		current_user->ViewMessages(ID);
		//Add Adding to Favorite Logic
		system("pause");
		MessageScreen();
		break;
	}

	case '5':
		HomeScreen();
		break;

	default:
		cout << "Please enter a valid choice\n";
		SystemPause();
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
			SystemPause();
			ContactScreen();
		}
	}
}
void FavoriteScreen() {

}
