//System header
//#include <windows.h>
#include <conio.h>//_getch()
#include <iomanip>//setw()
#include <ctime>//get time
#include <sstream>//set precision
#include <iostream>
#include <chrono>//waiting
#include <regex>//checking alphabetical

//Project header
#include "User.h"
#include "Account.h"
#include "Transaction.h"
#include "Menu.h"
#include "DBConnection.h"

//colour
#define RESET   "\033[0m"     // Reset to default color
#define RED     "\033[31m"    // Red color
#define GREEN   "\033[32m"    // Green color
#define YELLOW  "\033[33m"    // Yellow color
#define BLUE    "\033[34m"    // DeepBlue color
#define CYAN    "\033[36m"    // Cyan color
#define WHITE   "\033[37m"    // White color
using namespace std;

//User table
void registerAccount();
void loginMenu();
void UserPage(User user);
User profile(User user);	

//Account table
void AccountPage(string UserId);
void newAccount(string UserId);
void modifyAccountPage(string confirmation,string UserID);
void modifyAccount(Account account,string UserID); 

//Transaction table
void TransactionPage(string UserId);
void newTrans(string UserId,string account_name);
void transHistory(string UserId);
void modifyTrans(Transaction transaction,string UserId);

//Report and analysis
void statistic(string UserId);


//Other
string formatAmount(double amount);//to correct into 2 d.p.
string getCurrentMonthAbbreviation();
bool isValidFirstName(const string& firstName);
bool isValidLastName(const string& lastName);
bool isValidPassword(const string& password);
bool isValidEmail(const string& email);
//--------------------------------------------------------------------------------------------------------------------------------------------------//

int main()
{
	Menu LoginPage;
	LoginPage.header = "\t\t\t\tWelcome to money manager !!";
	LoginPage.addOption("Register");
	LoginPage.addOption("Login");
	LoginPage.addOption("Exit");

	while (1) {
		switch (LoginPage.prompt())
		{
		case 1:
			registerAccount();
			break;
		case 2:
			loginMenu();
			break;
		case 3:
			exit(0);
			break;
		default:
			break;
		}
	}
}

bool isValidFirstName(const string& firstName) {
	// Regular expression to match only alphabetic characters
	regex pattern("^[a-zA-Z]+$");

	// Check if the first name matches the pattern
	return regex_match(firstName, pattern);
}

bool isValidLastName(const string& lastName) {
	// Regular expression to match only alphabetic characters with at least one space
	regex pattern("^[a-zA-Z]+( [a-zA-Z]+)+$");

	// Check if the last name matches the pattern
	return regex_match(lastName, pattern);
}

bool isValidPassword(const string& password) {
	// Regular expressions for character classes
	regex upper("(?=.*[A-Z])");
	regex lower("(?=.*[a-z])");
	regex digit("(?=.*[0-9])");
	regex special("(?=.*[^A-Za-z0-9])");

	// Check the length
	if (password.length() < 8) {
		cout << RED"\n\t\tPassword must be at least 8 characters long." RESET<< std::endl;
		_getch();
		return false;
	}

	// Check for required character classes
	if (!regex_search(password, upper)) {
		cout << RED"\n\t\tPassword must contain at least one uppercase letter." RESET << std::endl;
		_getch();
		return false;
	}
	if (!regex_search(password, lower)) {
		cout << RED"\n\t\tPassword must contain at least one lowercase letter." RESET << std::endl;
		_getch();
		return false;
	}
	if (!std::regex_search(password, digit)) {
		cout << RED"\n\t\tPassword must contain at least one digit." RESET<< std::endl;
		_getch();
		return false;
	}
	if (!std::regex_search(password, special)) {
		cout << RED"\n\t\tPassword must contain at least one special character." RESET << std::endl;
		_getch();
		return false;
	}
	// Password meets all criteria
	return true;
}

bool isValidEmail(const string& email) {
	// Regular expression pattern for email validation
	regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

	// Check if the email matches the pattern
	return std::regex_match(email, pattern);
}

//LOGIN PAGE
//1)Register
void registerAccount() {
	User newacc;

	Menu cnMenu;
	cnMenu.header ="\t\tRegistration";
	cout << endl;
	cnMenu.addOption("UserId");
	cnMenu.addOption("First Name");
	cnMenu.addOption("Last Name");
	cnMenu.addOption("Password (must include'A-Z''a-z''0-9''!@#$%^&*()-_+=<>?/')");
	cnMenu.addOption("Email");
	cnMenu.addOption("Phone Number (01157426036)");
	cnMenu.addOption("Confirm");
	cnMenu.addOption("Back to login page");

	string firstName, lastName, password, emailAddress;
	while (1) {

		switch (cnMenu.prompt()) {
		case 1:
			cout << "Enter UserId: ";
			cin >> newacc.UserId;
			cnMenu.setValue(0, newacc.UserId);
			break;
		case 2:
			cout << "Enter first name: ";
			getline(cin, firstName);
			if (firstName.empty())
			{
				getline(cin, firstName);
			}
			//Validation firstName
			if (isValidFirstName(firstName)) {
				newacc.first_name = firstName;
				cnMenu.setValue(1, newacc.first_name);
			}
			else {
				std::cout << RED"\n\t\tInvalid first name. Please enter only alphabetic characters." RESET<< endl;
				_getch();
			}
			break;
		case 3:
			cout << "Enter last name: ";
			getline(cin,lastName);
			if (lastName.empty())
			{
				getline(cin, lastName);
			}
			//Validation lastName
			if (isValidLastName(lastName)) {
				newacc.last_name = lastName;
				cnMenu.setValue(2, newacc.last_name);
			}
			else {
				std::cout << RED"\n\t\tInvalid last name. Please enter only alphabetic characters." RESET << endl;
				_getch();
			}	
			break;
		case 4:
			cout << "Enter password: ";
			getline(cin, password);
			if (password.empty())
			{
				getline(cin, password);
			}
			// Validate password
			if (isValidPassword(password)) {
				newacc.password = password;
				cnMenu.setValue(3, newacc.password);
			}
			else {
				cout << RED"\n\t\tInvalid password." RESET << std::endl;
				_getch();
			}
			break;
		case 5:
			cout << "Enter email: ";
			getline(cin, emailAddress);
			if (emailAddress.empty())
			{
				getline(cin, emailAddress);
			}
			// Validate email address
			if (isValidEmail(emailAddress)) {
				newacc.email = emailAddress;
				cnMenu.setValue(4, newacc.email);
			}
			else {
				cout << RED"\n\t\tInvalid email address." RESET << std::endl;
			}
			cnMenu.setValue(4, newacc.email);
			break;
		case 6:
			cout << "Enter Phone Number: ";
			while (true)
			{
				cin >> newacc.phone_number;
				bool isValid = true;
				for (char ch : newacc.phone_number)
				{
					if (!isdigit(ch))
					{
						isValid = false;
						break;
					}
				}
				if (isValid && newacc.phone_number.length() >= 10 && newacc.phone_number.length() <= 11)
				{
					break;
				}
				cout << "Phone Number should contain only 10 to 11 digits. \nPlease enter again: ";
			}
			cnMenu.setValue(5, newacc.phone_number);
			break;
		case 7:
			newacc.insert();
			cout <<YELLOW<< "Register sucessfull !\n\n"<<RESET;
			_getch();
			return;
		case 8:
			return;
			break;
		default:
			break;
		}
	}
}


//2)Login
void loginMenu()
{
	User user;
	char ch;
	char password[50];
	int index = 0;

	Menu loginMenu;
	loginMenu.header = "\tLOGIN";
	loginMenu.addOption("UserId");
	loginMenu.addOption("Password");
	loginMenu.addOption("Confirm");
	loginMenu.addOption("Back");

	while (1) {
		switch (loginMenu.prompt())
		{
		case 1:
			cout << "Enter UserId: ";
			cin >> user.UserId;
			loginMenu.setValue(0, user.UserId);
			break;
		case 2:
			cout << "Enter Password: ";
			while (true) {
				ch = _getch(); // Capture the input without displaying it
				if (ch == 13) { // 13 is the ASCII code for Enter key
					break;
				}
				else if (ch == 8 && index > 0) { // 8 is the ASCII code for Backspace
					// Handle backspace: move cursor back, overwrite with a space, move back again
					index--;
					cout << "\b \b";
				}
				else if (ch != 8) { // Ignore the backspace character
					password[index++] = ch;
					cout << '*';
					user.password = password;
				}
			}
			loginMenu.setValue(1,"********");
			break;
		case 3:
			if (user.login()) {
				UserPage(user);//UserId and password store in UserPage
			}
			else {
				cout << RED <<"\tInvalid Login !"<< CYAN <<" Please press enter to continues..."<<RESET;
				_getch();
			}
			break;
		case 4:
			return;
			break;
		default:
			break;
		}
	}
}

//USER PAGE
void UserPage(User user) 
{
	Account account;
	Transaction transaction;
	account.UserID = user.UserId;//pass value of UserID into account table,so can get data from account table

	Menu homeMenu;
	homeMenu.addOption("Profile");
	homeMenu.addOption("Account");
	homeMenu.addOption("Transaction");
	homeMenu.addOption("Analysis and Report");
	homeMenu.addOption("Logout");
	
	while (1)
	{
		homeMenu.header = "\t\tWelcome " + user.UserId;
		double totalAmount = account.totalAmount(); // Get the total amount
		string formattedTotalAmount;

		// Formatting totalAmount to 2 decimal places
		ostringstream stream;
		stream << fixed << setprecision(2) << totalAmount;
		formattedTotalAmount = CYAN"\nTotal amount of balance: " WHITE  "RM " GREEN + stream.str() + RESET;
		
		// Output the formatted totalAmount
		homeMenu.footer = formattedTotalAmount;
		
		switch (homeMenu.prompt())
		{
		case 1:
			user = profile(user);
			break;
		case 2:
			AccountPage(user.UserId);
			break;
		case 3:
			TransactionPage(user.UserId);
			break;
		case 4:
			statistic(user.UserId);
			break;
		case 5:
			main();
			break;
		default:
			break;
		}
	}
}

//1)Profile
User profile(User user) {

	User temp = user; // copy the object

	Menu profileMenu;
	profileMenu.header = "\t\tYour profile";
	profileMenu.addOption("Fisrt Name");
	profileMenu.addOption("Last Name");
	profileMenu.addOption("Password (must include'A-Z''a-z''0-9''!@#$%^&*()-_+=<>?/')");;
	profileMenu.addOption("Phone Number (Exp:01157426036)");
	profileMenu.addOption("Reset");
	profileMenu.addOption("Save");
	profileMenu.addOption("Back");
	profileMenu.addOption("Delete Account");

	string firstName;
	string lastName;
	string password;
	firstName = temp.first_name;
	lastName = temp.last_name;
	while (1) {
		profileMenu.setValue(0, temp.first_name);
		profileMenu.setValue(1, temp.last_name);
		profileMenu.setValue(2, temp.password);
		profileMenu.setValue(3, temp.phone_number);
	
		switch (profileMenu.prompt())
		{
		case 1:
			cout << "Enter first name:";
			getline(cin, temp.first_name);
			if (temp.first_name.empty())
			{
				getline(cin, temp.first_name);
			}
			if (isValidFirstName(temp.first_name)) {
				profileMenu.setValue(0, temp.first_name);
			}
			else {
				std::cout << RED"\n\t\tInvalid first name. Please enter only alphabetic characters." RESET << endl;
				temp.first_name = firstName;
				_getch();
			}
			break;
		case 2:
			cout << "Enter last name:";
			getline(cin,temp.last_name);
			if (temp.last_name.empty())
			{
				getline(cin, temp.last_name);
			}
			//Validation lastName
			if (isValidLastName(temp.last_name)) {
				profileMenu.setValue(2, temp.last_name);
				
			}
			else {
				std::cout << RED"\n\t\tInvalid last name. Please enter only alphabetic characters." RESET << endl;
				temp.last_name = lastName;
				_getch();
			}
			break;
		case 3:
			cout << "Enter new password: ";
			cin >> password;
			// Validate password
			if (isValidPassword(password)) {
				temp.password = password;
				profileMenu.setValue(3, temp.password);
			}
			else {
				cout << RED"\n\t\tInvalid password." RESET << std::endl;
				_getch();
			}
			break;
		case 4:
			cout << "Enter new phone number: ";
			while (true) 
			{
				cin>>temp.phone_number;
				bool isValid = true;

				for (char ch : temp.phone_number)
				{
					if (!isdigit(ch)) 
					{
						isValid = false;
						break;
					}
				}

				if (isValid && temp.phone_number.length() >= 10 && temp.phone_number.length() <= 11)
				{	
					break;
				}
				cout << RED"\t\tPhone Number should contain only 10 to 11 digits." RESET;
				cout << "\nPlease enter again: ";
			}
			break;
		case 5:
			temp = user;
			break;
		case 6:
			user = temp;
			user.update();
			cout << CYAN"\t\tUpdated"<<RESET;
			_getch();
			break;
		case 7:
			return user;
			break;
		case 8:
			cout <<RED<< "\t\tDelete your account? [Y/N]"<<RESET;
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') {
				user = temp;
				user.remove();
				main();
			}
			break;
		default:
			break;
		}
	}
}

//ACCOUNT PAGE
void AccountPage(string UserId)
{
	Account account;
	Transaction transaction;
	vector<Account> accounts;
	string displayString = "";
	string sortColumn = "account_name";
	bool ascending = true;
	string confirmation;

	Menu homeAccount;
	homeAccount.addOption("Search or refresh Account / Wallet");
	homeAccount.addOption("Order By ");
	homeAccount.addOption("Odering ");
	homeAccount.addOption("Add account");
	homeAccount.addOption("Edit account");
	homeAccount.addOption("Back to User Page");

	Menu sortingSubMenu;
	sortingSubMenu.header = "\t\t\tSelect Sort category";
	sortingSubMenu.addOption("Account name");
	sortingSubMenu.addOption("Balance");
	sortingSubMenu.addOption("Budget Amount");
	
	while (1)
	{
		homeAccount.setValue(1, sortColumn);
		if (ascending) {
			homeAccount.setValue(2, "Ascending");
		}
		else {
			homeAccount.setValue(2, "Descending");
		}
		
		if (displayString == "") {
			displayString = BLUE"\nSearch Result:\n" RESET;
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5)  << "Account Name" << "|" << setw(18) << "Balance"
				<< "|" << setw(23) << "Budget amount" << "|" << setw(23) << "Start date" << "|" << setw(23) << "End date" <<  "|"<< endl;

			for (int i = 0; i < accounts.size(); i++) {
				tmpString << setw(12) << accounts[i].account_name << "|" << setw(16); 
				if (accounts[i].balance >0 )
				{
					tmpString << GREEN  << accounts[i].balance << WHITE;
				}
				else
				{
					tmpString << RED <<accounts[i].balance << WHITE;
				}
				
				tmpString << "|" << setw(21); 
				if(accounts[i].budget_amount >0)
				{
					tmpString << GREEN  << accounts[i].budget_amount << WHITE;
				}
				else
				{
					tmpString << RED  << accounts[i].budget_amount<< WHITE;
				}
				tmpString << "|" << setw(23) << accounts[i].start_date << "|" << setw(23) << accounts[i].end_date << "|" << endl;
			}
			displayString += tmpString.str();
		}

		homeAccount.header = "\tAccounts";
		homeAccount.footer = displayString;
		switch (homeAccount.prompt())
		{
		case 1:
			accounts = Account::findAccount(UserId, sortColumn, ascending);
			displayString = "";
			break;
		case 2:
			switch (sortingSubMenu.prompt())
			{
			case 1:
				sortColumn = "account_name";
				break;
			case 2:
				sortColumn = "balance";
				break;
			case 3:
				sortColumn = "budget_amount";
				break;
			}
			break;
		case 3:
			ascending = !ascending;
			break;
		case 4:
			newAccount(UserId);
			break;
		case 5:
			confirmation = "ConfirmEdit";
			modifyAccountPage(confirmation,UserId);
			break;
		case 6:
			return;
			break;

		default:
			break;
		}
	}
}

//convert amount to 2 decimal places
string formatAmount(double amount) {
	ostringstream formattedStream;
	formattedStream << "RM " << fixed << setprecision(2) << amount; // Format with 2 decimal places
	return formattedStream.str(); // Return the formatted string
}

string getCurrentMonthAbbreviation() {
	// Get the current system time
	auto now = std::chrono::system_clock::now();

	// Convert the current time to a time_t object
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	// Declare a tm structure
	std::tm timeInfo;

	// Use localtime_s to fill the tm structure with the current time information
	if (localtime_s(&timeInfo, &currentTime) != 0) {
		// Handle error if localtime_s fails
		return "Error";
	}

	// Array containing month abbreviations
	const char* monthAbbreviations[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
										"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	// Extract current month index and return its abbreviation
	int currentMonthIndex = timeInfo.tm_mon;
	return monthAbbreviations[currentMonthIndex];
}

//1)Add account
void newAccount(string UserId) 
{
	Account addAccount;
	Menu accountMenu;
	accountMenu.header = "\t\tAdd Account";
	accountMenu.addOption("Account Name");
	accountMenu.addOption("Balance ");
	accountMenu.addOption("Budget Amount");
	accountMenu.addOption("Start Date");
	accountMenu.addOption("End Date");
	accountMenu.addOption("Confirm");
	accountMenu.addOption("Back");

	string month;
	string tmpinput;
	string formattedBudget;
	string formattedBalance;

	time_t current = time(0); // get time in epoch seconds (since 1900)
	tm now; // create a struct/object of tm to hold data
	localtime_s(&now, &current); //populate the now object with data from current
	string abbreviation = getCurrentMonthAbbreviation();
	

	while (1) {
		
		switch (accountMenu.prompt())
		{
		case 1:
			cout << "Eneter account name (Example: RHB Bank): ";
			getline(cin, addAccount.account_name);
			if (addAccount.account_name.empty())
			{
				getline(cin, addAccount.account_name);
			}
			accountMenu.setValue(0, addAccount.account_name);
			break;
		case 2:
			cout << "Enter account balance: RM ";
			cin >> addAccount.balance;
			if (addAccount.balance < 0)
			{
				cout << RED"\t\tAccount balance cannot be less than zero." RESET;
				cout << "\nPlease enter account balance again: ";
				cin >> addAccount.balance;
			}
			// Format the balance amount using formatAmount function
			formattedBalance = formatAmount(addAccount.balance);
			// Set the formatted string to the menu value
			accountMenu.setValue(1, formattedBalance);
			break;
		case 3:
			cout << "Enter budget amount: RM ";
			cin >> addAccount.budget_amount;
			if (addAccount.budget_amount < 0)
			{
				cout << RED"\t\tBudget amount cannot be less than zero." RESET;
				cout << "\nPlease enter budget amount again: ";
				cin >> addAccount.budget_amount;
			}
			// Format the budget amount using formatAmount function
			formattedBudget = formatAmount(addAccount.budget_amount);
			// Set the formatted string to the menu value
			accountMenu.setValue(2, formattedBudget);
			break;
		case 4:
			cout << "Fisrt 3 letter abbreviation of month for set budget amount (Example: Jan,Feb): ";
			month = abbreviation;
			cout<<month;
			if (month == "Jan" || month == "Mar" || month == "May" || month == "Jul" || month == "Aug" || month == "Oct" || month == "Dec")
			{
				addAccount.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
				accountMenu.setValue(3,addAccount.start_date);
				addAccount.end_date = "31-" + month + "-" + to_string(now.tm_year + 1900);
				accountMenu.setValue(4, addAccount.end_date);
			}
			else if (month == "Apr" || month == "Jun" || month == "Sep" || month == "Nov")
			{
				addAccount.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
				accountMenu.setValue(3, addAccount.start_date);
				addAccount.end_date = "30-" + month + "-" + to_string(now.tm_year + 1900);
				accountMenu.setValue(4, addAccount.end_date);
			}
			else if (month == "Feb")
			{
				// leap year if perfectly divisible by 400
				if ((now.tm_year + 1900) % 400 == 0)
				{
					addAccount.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
					accountMenu.setValue(3, addAccount.start_date);
					addAccount.end_date = "29-" + month + "-" + to_string(now.tm_year + 1900);
					accountMenu.setValue(4, addAccount.end_date);
				}
				// not a leap year if divisible by 100
				// but not divisible by 400
				else if ((now.tm_year + 1900) % 100 == 0)
				{
					addAccount.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
					accountMenu.setValue(3, addAccount.start_date);
					addAccount.end_date = "28-" + month + "-" + to_string(now.tm_year + 1900);
					accountMenu.setValue(4, addAccount.end_date);
				}
				// leap year if not divisible by 100
				// but divisible by 4
				else if ((now.tm_year + 1900) % 4 == 0)
				{
					addAccount.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
					accountMenu.setValue(3, addAccount.start_date);
					addAccount.end_date = "29-" + month + "-" + to_string(now.tm_year + 1900);
					accountMenu.setValue(4, addAccount.end_date);
				}
				// all other years are not leap years
				else 
				{
					addAccount.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
					accountMenu.setValue(3, addAccount.start_date);
					addAccount.end_date = "28-" + month + "-" + to_string(now.tm_year + 1900);
					accountMenu.setValue(4, addAccount.end_date);
				}
			}
			else
			{
				cout << RED"\t\tInvlaid input" RESET;
				_getch();
			}
			break;
		case 5:
			break;
		case 6:
			addAccount.UserID = UserId;
			addAccount.addAccount();
			cout << CYAN<<"\t\tAdd sucessful !"<<RESET;
			_getch();
		case 7:
			//return account;//create account page
			return;
			break;
		default:
			break;
		}
		
	}
}

void modifyAccountPage(string confirmation,string UserId)
{
	vector <Account >Acc;
	string disPlayAcc = "";

	Account account;
	
	Menu mdfAccPage;
	mdfAccPage.header = "\t\t\tYour account / wallet";
	mdfAccPage.addOption("Search account / wallet");
	mdfAccPage.addOption("Account name");
	mdfAccPage.addOption("Confirm");
	mdfAccPage.addOption("Back");

	while (1)
	{
		if (disPlayAcc == "") {
			disPlayAcc = BLUE"\nSearch Result:\n" RESET;
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "Account Name" << "|" << endl;

			for (int i = 0; i < Acc.size(); i++) {
				tmpString << setw(12) << Acc[i].account_name << "|" << endl;
			}
			disPlayAcc += tmpString.str();
		}
		mdfAccPage.footer = disPlayAcc;
		
		switch (mdfAccPage.prompt())
		{
		case 1:
			Acc = Account::selectAccount(UserId);
			disPlayAcc = "";
			break;
		case 2:
			cout << "Enter the account name to select: ";
			getline(cin, account.account_name);
			mdfAccPage.setValue(1,account.account_name);
			break;
		case 3:
			if (confirmation == "ConfirmEdit")
			{
				if (account.confirmtoEdit(UserId))
				{
					modifyAccount(account,UserId);
				}
				else 
				{
					cout << RED << "\tInvalid account !" << CYAN << " Please press enter to continues..." << RESET;
					_getch();
				}
			}
			else if (confirmation == "ConfirmAdd")
			{
				newTrans(UserId, account.account_name);
			}
			break;
		case 4:
			return;
			break;
		default:
			break;
		}
	}
}

//2)Modify account
void modifyAccount(Account account,string UserID)
{ 
	Account temp = account;

	Menu modifyAccMenu;
	modifyAccMenu.header = "\t\t\t\t\tEdit information of account / wallet";
	modifyAccMenu.addOption("Account name");//1
	modifyAccMenu.addOption("Balance");//2
	modifyAccMenu.addOption("Budget amount");//3
	modifyAccMenu.addOption("Start date");//4
	modifyAccMenu.addOption("End date");//5
	modifyAccMenu.addOption("Reset");//6
	modifyAccMenu.addOption("Save");//7
	modifyAccMenu.addOption("Back");//8
	modifyAccMenu.addOption("Delete Account");//9

	string month;
	string formattedBudget;
	string formattedBalance;

	time_t current = time(0); // get time in epoch seconds (since 1900)
	tm now; // create a struct/object of tm to hold data
	localtime_s(&now, &current); //populate the now object with data from current
	string abbreviation = getCurrentMonthAbbreviation();

	string accountName = temp.account_name;
	
	while(1)
	{
		modifyAccMenu.setValue(0, temp.account_name);

		formattedBalance = formatAmount(temp.balance);
		modifyAccMenu.setValue(1, formattedBalance);

		formattedBudget = formatAmount(temp.budget_amount);
		modifyAccMenu.setValue(2, formattedBudget);

		modifyAccMenu.setValue(3, temp.start_date);
		modifyAccMenu.setValue(4, temp.end_date);
		switch (modifyAccMenu.prompt())
		{
		case 1:
			cout << "Eneter new account name (Example: RHB Bank): ";
			getline(cin, temp.account_name);
			if (temp.account_name.empty())
			{
				temp.account_name = accountName;
			}
			break;
		case 2:
			cout << "Enter new balance: RM ";
			cin >> temp.balance;
			break;
		case 3:
			cout << "Enter new budget amount: RM ";
			cin >> temp.budget_amount;
			break;
		case 4:
			cout << "Fisrt 3 letter abbreviation of month for set budget amount (Example: Jan,Feb): ";
			month = abbreviation;
			cout << month;
			if (month == "Jan" || month == "Mar" || month == "May" || month == "Jul" || month == "Aug" || month == "Oct" || month == "Dec")
			{
				temp.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
				temp.end_date = "31-" + month + "-" + to_string(now.tm_year + 1900);
			}
			else if (month == "Apr" || month == "Jun" || month == "Sep" || month == "Nov")
			{
				temp.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
				temp.end_date = "30-" + month + "-" + to_string(now.tm_year + 1900);
			}
			else if (month == "Feb")
			{
				if ((now.tm_year + 1900) % 400 == 0)
				{
					temp.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
					temp.end_date = "29-" + month + "-" + to_string(now.tm_year + 1900);
				}
				else if ((now.tm_year + 1900) % 100 == 0)
				{
					temp.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
					temp.end_date = "28-" + month + "-" + to_string(now.tm_year + 1900);
				}
				else if ((now.tm_year + 1900) % 4 == 0)
				{
					temp.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
					temp.end_date = "29-" + month + "-" + to_string(now.tm_year + 1900);
				}
				else
				{
					temp.start_date = "01-" + month + "-" + to_string(now.tm_year + 1900);
					temp.end_date = "28-" + month + "-" + to_string(now.tm_year + 1900);
				}
			}
			break;
		case 5:
			
			break;
		case 6:
			temp = account;
			break;
		case 7:
			account = temp;
			account.UserID = UserID;
			account.update();
			cout << CYAN << "Updated" << RESET;
			_getch();
		case 8:
			return ;
			break;
		case 9:
			cout << RED << "Delete your account? [Y/N]" << RESET;
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') {
				account = temp;
				account.removeAccount();
				return ;
			}
			break;
		default:
			break;
		}
	}
}


//TRANSACTION PAGE
void TransactionPage(string UserId)
{
	User user;
	user.UserId = UserId;
	string confirmation;
	Menu transPage;
	transPage.header = "\t\tTransaction Page";
	transPage.addOption("Create transaction");
	transPage.addOption("Transaction History");
	transPage.addOption("Back to User Page");

	while (1)
	{
		switch (transPage.prompt())
		{
		case 1:
			confirmation = "ConfirmAdd";
			modifyAccountPage(confirmation, UserId);
			break;
		case 2:
			transHistory(UserId);
			break;
		case 3:
			return UserPage(user);
			break;
		default:
			break;
		}
	}
}

void newTrans(string UserID, string account_name)
{
	Transaction addTrans;
	Account account;

	bool Expenses = true;
	string UserId;
	
	account.UserID = UserID;
	account.account_name = account_name;

	Menu homeTrans;
	homeTrans.addOption("Search and select account / wallet");//1
	homeTrans.addOption("Type of transaction");//2
	homeTrans.addOption("Amount of transaction");//3
	homeTrans.addOption("Category");//4
	homeTrans.addOption("Description");//5
	homeTrans.addOption("New balance");//6
	homeTrans.addOption("New budget amount");//7
	homeTrans.addOption("Confirm");//8
	homeTrans.addOption("Back to transaction page");//9

	string formattedTransAmount;
	string formattednewBalance;
	string formattednewBudgetAmount;
	string confirmation;
	string accountName;
	double tempAmount;
	double tempBudget;

	while (1)
	{
		homeTrans.setValue(0, account_name);
		account.getAccount(UserID, account_name);//get account data
		addTrans.AccountID = account.AccountID;
		if (Expenses) 
		{
			addTrans.transaction_type = "Expenses";
			homeTrans.setValue(1, addTrans.transaction_type);
		}
		else
		{
			addTrans.transaction_type ="Deposit";
			homeTrans.setValue(1, addTrans.transaction_type);	
		}
		homeTrans.header = "Add Transaction";
		switch (homeTrans.prompt())
		{
		case 1:
			confirmation = "ConfirmAdd";
			modifyAccountPage(confirmation,UserID);
			break;
		case 2:
			Expenses = !Expenses;
			break;
		case 3:
			cout << "Enter amount: RM ";
			cin >> addTrans.transaction_amount;
			if (addTrans.transaction_amount < 0)
			{
				cout << RED"\t\tInvalid ! Amount of transaction is insufficient." RESET;
				cout << "\nPlease enter amount again: ";
			}
			formattedTransAmount = formatAmount(addTrans.transaction_amount);
			homeTrans.setValue(2, formattedTransAmount);
			break;
		case 4:
			cout << "Enter category (Example: Food & Beverage, Transportation, Bills): ";
			getline(cin,addTrans.category);
			if (addTrans.category.empty())
			{
				getline(cin, addTrans.category);
			}
			homeTrans.setValue(3, addTrans.category);
			break;
		case 5:
			cout << "Description: ";
			if (addTrans.transaction_type == "Expenses")
			{
				getline(cin, addTrans.description);
				if (addTrans.description.empty())
				{
					getline(cin, addTrans.description);
				}
				homeTrans.setValue(4, addTrans.description);
			}
			else 
			{
				addTrans.description = "NULL";
				homeTrans.setValue(4, addTrans.description);
			}
		case 6:
			if (addTrans.transaction_type == "Expenses")
			{
				tempAmount = account.balance - addTrans.transaction_amount;
				formattednewBalance = formatAmount(tempAmount);
				homeTrans.setValue(5, formattednewBalance);

				tempBudget = account.budget_amount - addTrans.transaction_amount;
				formattednewBudgetAmount = formatAmount(tempBudget);
				homeTrans.setValue(6, formattednewBudgetAmount);
			}
			else
			{
				tempAmount = account.balance + addTrans.transaction_amount;
				formattednewBalance = formatAmount(tempAmount);
				homeTrans.setValue(5, formattednewBalance);

				tempBudget = account.budget_amount;
				formattednewBudgetAmount = formatAmount(account.budget_amount);
				homeTrans.setValue(6, formattednewBudgetAmount);
			}
			break;
		case 7:
			break;
		case 8:
			addTrans.newbalance = tempAmount;
			account.balance = tempAmount;
			account.budget_amount = tempBudget;
			addTrans.addTrans();
			account.updateAfterTrans();
			cout << CYAN"Transaction is create" RESET;
			_getch();
		case 9:
			return TransactionPage(UserID);
			break;
		default:
			break;
		}
	}
}

void transHistory(string UserId)
{
	vector<Transaction> trans;
	string displayString = "";
	string sortColumn = "TransactionID";
	bool ascending = true;

	Transaction transaction;

	Menu transHis;
	transHis.header = "\tHistory !";
	transHis.addOption("Search / refresh transaction history");//1
	transHis.addOption("Order By ");//2
	transHis.addOption("Odering ");//3
	transHis.addOption("Edit transaction with Transaction ID");//4
	transHis.addOption("Confirm");//5
	transHis.addOption("Back to User Page");//6

	Menu sortingSubMenu;
	sortingSubMenu.header = "Select Sort category";
	sortingSubMenu.addOption("Transaction ID");
	sortingSubMenu.addOption("Transaction type");
	sortingSubMenu.addOption("Transaction date time");

	while (1)
	{
		transHis.setValue(1, sortColumn);
		if (ascending) {
			transHis.setValue(2, "Ascending");
		}
		else {
			transHis.setValue(2, "Descending");
		}

		if (displayString == "")
		{
			displayString = BLUE"\nSearch Result:\n" RESET;
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "Transaction ID" << "|" << setw(25)
				<< "Transaction Type" << "|" << setw(28) << "Transaction Amount" << "|" << setw(20)
				<< "Category" << "|" << setw(20) << "Description" << "|" << setw(20) << "Newbalance" << "|"
				<< setw(25) << "Transaction date" << "|" << endl;

			for (int i = 0; i < trans.size(); i++) 
			{
				tmpString << setw(14) << trans[i].TransactionID << "|" << setw(25)<< trans[i].transaction_type << "|" << setw(26);

				if (trans[i].transaction_type == "Deposit")
				{
					tmpString << GREEN "+" << trans[i].transaction_amount << WHITE;
				}
				else
				{
					tmpString << RED "-" << trans[i].transaction_amount << WHITE;
				}

				tmpString << "|" << setw(20) << trans[i].category << "|" << setw(20) << trans[i].description << "|" << setw(18);

				if (trans[i].newbalance < 0)
				{
					tmpString << RED"-" << trans[i].newbalance << WHITE;
				}
				else
				{
					tmpString << GREEN << trans[i].newbalance << WHITE;
				}
				tmpString << "|" << setw(25) << trans[i].transaction_date << "|" << endl;
			}
			displayString += tmpString.str();
		}

		transHis.footer = displayString;

		switch (transHis.prompt())
		{
		case 1:
			trans = Transaction::findTransaction(UserId, sortColumn, ascending);
			displayString = "";
			break;	
		case 2:
			switch (sortingSubMenu.prompt())
			{
			case 1:
				sortColumn = "TransactionID";
				break;
			case 2:
				sortColumn = "transaction_type";
				break;
			case 3:
				sortColumn = "transaction_date";
			}
			break;
		case 3:
			ascending = !ascending;
			break;
		case 4:
			cout << "Enter Transaction ID to edit: ";
			int transactionId;
			cin >> transactionId;
			transaction.TransactionID = transactionId;
			transHis.setValue(3,to_string(transaction.TransactionID));
			break;
		case 5:
			if (transaction.confirmToUpdate(transaction.TransactionID)) 
			{
				modifyTrans(transaction,UserId);
			}
			else 
			{
				cout << RED << "\tInvalid account !" << CYAN << " Please press enter to continues..." << RESET;
				_getch();
			}
			break;
		case 6:
			return;
			break;
		default:
			break;
		}
	}
}

//still need to change
void modifyTrans(Transaction transaction,string UserId)
{
	Transaction temp = transaction;
	bool Expenses = true;

	Account account;
	
	account.AccountID = transaction.AccountID;
	account.UserID = UserId;

	Menu modifyTrans;
	modifyTrans.header = "\t\tEdit transaction";
	modifyTrans.addOption("Transaction type");//1
	modifyTrans.addOption("Transaction amount");//2
	modifyTrans.addOption("Category");//3
	modifyTrans.addOption("Description");//4
	modifyTrans.addOption("New balance");//5
	modifyTrans.addOption("Reset");//6
	modifyTrans.addOption("Save");//7
	modifyTrans.addOption("Back");//8
	modifyTrans.addOption("Delete Account");//9
	

	string formattedTransAmount;
	string formattednewBalance;
	string formattednewBudgetAmount;
	string confirmation;
	string accountName;
	double tempAmount=0;
	double tempBudget=0;
	double initialBalance;
	double initialBudget;
	double tempnewBalance=0;
	double tempTransAmount=0;
	double oldTransAmount = 0;
	double oldNewBalance = 0;
	double latestBudget = 0;

	account.getBlcBdg(UserId, account.AccountID);

	oldTransAmount = temp.transaction_amount;
	oldNewBalance = temp.newbalance;

	if (temp.transaction_type == "Expenses")
	{
		account.balance = oldTransAmount + oldNewBalance;//get original balance

		account.budget_amount = account.budget_amount + oldTransAmount;//get original budget
		initialBalance = account.balance;
		initialBudget = account.budget_amount;
	}
	else if (temp.transaction_type == "Deposit")
	{
		account.balance = oldNewBalance - oldTransAmount;
		initialBalance = account.balance;
	}

	while (1)
	{
		if (temp.transaction_type == "Deposit" || temp.transaction_type == "Expenses")
		{
			modifyTrans.setValue(0, temp.transaction_type);
		}
		else
		{
			cout << RED << "\n\tInvalid input !" << CYAN << " Please press enter to continues..." << RESET;
			_getch();
			cout << "\nPlease select again: ";
			cin >> temp.transaction_type;
			modifyTrans.setValue(0, temp.transaction_type);
		}
		
		formattedTransAmount = formatAmount(temp.transaction_amount);
		modifyTrans.setValue(1, formattedTransAmount);

		modifyTrans.setValue(2, temp.category);
		modifyTrans.setValue(3, temp.description);

		formattednewBalance = formatAmount(temp.newbalance);
		modifyTrans.setValue(4, formattednewBalance);

		switch (modifyTrans.prompt())
		{
		case 1:
			cout << "Select 'Deposit' or 'Expenses': ";
			cin >> temp.transaction_type;
			break;
		case 2:
			cout << "Enter amount: RM ";
			cin >> temp.transaction_amount;
			if (temp.transaction_amount < 0)
			{
				cout << RED"\t\tInvalid ! Amount of transaction is insufficient." RESET;
				cout << "\nPlease enter amount again: ";
			}
			break;
		case 3:
			cout << "Enter category (Example: Food & Beverage, Transportation, Bills): ";
			cin >> temp.category;
			break;
		case 4:
			cout << "Description: ";
			if (temp.transaction_type == "Expenses")
			{
				cin >> temp.description;
			}
			else
			{
				temp.description = "NULL";
			}
		case 5:
			if (temp.transaction_type == "Expenses") {
				tempAmount = initialBalance - temp.transaction_amount;
				tempBudget = initialBudget - temp.transaction_amount;
			}
			else {
				tempAmount = initialBalance + temp.transaction_amount;
				tempBudget = account.budget_amount;
			}

			temp.newbalance = tempAmount;
			account.balance = tempAmount;
			account.budget_amount = tempBudget;

			break;
		case 6:
			temp = transaction;
			break;
		case 7:
			transaction = temp;
			transaction.updateTrans(transaction.TransactionID);
			account.updateAfterTrans();
			cout << CYAN"Updated !" RESET;
			_getch();
		case 8:
			return;
			break;
		case 9:
			cout << RED << "Delete this transaction? [Y/N]" << RESET;
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') 
			{
				transaction.deleteTrans(transaction.TransactionID);
				account.updateAfterTrans();
				return;
			}
			break;
		default:
			break;
		}
	}
}

void statistic(string UserId)
{
	int totalTrans = -1;
	totalTrans = Transaction::totalTimesOfTrans(UserId);

	double netTrans = 0;
	netTrans = Transaction::netTrans(UserId);

	double previousTransAmount = 0;
	previousTransAmount = Transaction::transAmount(UserId);

	double totalTransAmount = 0;
	totalTransAmount = Transaction::totalTransactionAmount(UserId);

	Menu TH;
	TH.header = "\t\t\tStatistic per month";
	TH.addOption("Number of transaction");
	TH.addOption("Net transaction");
	TH.addOption("Total transaction amount");
	TH.addOption("Back to User Page");

	Menu SubTA;
	SubTA.addOption("One month");
	SubTA.addOption("Previous 7 days");

	string formattedBalance;
	string formattedNetTrans;
	string formattedPrevious;
	//Format the balance amount using formatAmount function

	formattedPrevious = formatAmount(previousTransAmount);
	formattedNetTrans = formatAmount(netTrans);
	formattedBalance = formatAmount(totalTransAmount);
	
	
	while (1)
	{
		TH.setValue(0,to_string(totalTrans));
		TH.setValue(1, formattedNetTrans);
		
		switch(TH.prompt())
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			switch (SubTA.prompt())
			{
			case 1:
				TH.setValue(2, formattedBalance);
				break;
			case 2:
				TH.setValue(2, formattedPrevious);
				break;
			}
			break;
		case 4:
			return;
			break;
		default:
			break;
		}

	}
}
