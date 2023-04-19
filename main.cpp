#include "main.h"
#include <chrono>
#include <ctime>

#define CONFIG_FILE "data.ini"

Config cfg(CONFIG_FILE);
void TestAccountToConfig()
{
	UserAccount acc(50, "muhab", "12345666");
	cfg.UserAccounts[acc.ID()] = acc;

	

	// Save configuration on exit (not finished yet)
	cfg.Save();
}
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

bool Register(string Username, string Password)
{
	if (cfg.AccountExists(Username,Password) == NULL)
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

int main()
{
	// Load configuration on startup
	cfg.Load();

	//--------------------------------------------------------------------------
	/*
	Karim: This next section is me testing how the program would run on console
	You can comment it if you don't like it or you wanna modify stuff	
	*/
	UserAccount* current_user;
	int choice=0;
	string username, password;
	while (choice!=3)
	{ 
		system("cls");
		cout << "----------------- Welcome to Saraha -----------------\n";
		cout << "1. REGISTER\n2. LOGIN\n3. EXIST\n\nPlease choose an option: ";
		cin >> choice;

		switch (choice)
		{
			case 1:
			{
				system("cls");
				cout << "Enter Username: ";
				cin >> username;
				cout << "Enter Password: ";
				cin >> password;

				if (Register(username, password))
				{
					cout << "\Account Created\n";
					system("pause");
				}
				else
				{
					cout << "Error: Account already exists\n";
					system("pause");
				} 
				break;
			}
			case 2:
			{
				system("cls");
				cout << "Enter Username: ";
				cin >> username;
				cout << "Enter Password: ";
				cin >> password;
				
				if (Login(username, password) == NULL)
				{
					cout << "Invalid Credentials\n";
					system("pause");
				}
				else
				{
					current_user = Login(username, password);
					cout << "Login Successful\n";
					system("pause");
				}
				break;
			}
		}
	}
	//--------------------------------------------------------------------------

	// Save configuration on exit (not finished yet)
	//TestingViewContact();
	cfg.Save();
	return 0;
}


