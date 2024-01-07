//System header
#include <conio.h>//_getch()
#include <iomanip>//setw()
#include <ctime>//get time
#include <sstream>//set precision
#include <iostream>
#include <chrono>//waiting
#include <regex>//checking alphabetical
#include <cstdio>//printf
#include <cmath>
#include <map>
#include <unordered_map>
#include <string>
#include <stdexcept>

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
void graph(string UserId);

//Other
string formatAmount(double amount);//to correct into 2 d.p.
bool isValidEndDate(const string& startMonth, int startYear, const string& endMonth, int endYear);
bool isValidFutureMonth(const string& insertedMonth, const int insertedYear);
int getYear(const string& dateStr);
string getMonthAbbreviation(const string& dateStr);
bool validateUsername(const string& username);
bool isValidFirstName(const string& firstName);
bool isValidLastName(const string& lastName);
bool isValidPassword(const string& password);
bool isValidEmail(const string& email);
bool isValidYear(const string& input);
bool isValidYearAndMonth(const string& yearInput, const string& monthInput);
bool endDateWithCurrentDate(const string& startMonth, int startYear, const string& endMonth, int endYear);
//--------------------------------------------------------------------------------------------------------------------------------------------------//

//Register and login page
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

//validate UserID (username)
bool validateUsername(const string& username) {
	// Check length
	if (username.length() < 3 || username.length() > 20) {
		cout << RED"\n\t\tUsername length should be between 3 and 20 characters." RESET<< endl;
		return false;
	}

	// Check character set (allowing only alphanumeric characters and underscore)
	if (!regex_match(username, regex("^[a-zA-Z0-9_]+$"))) {
		cout << RED"\n\t\tUsername should only contain letters, numbers, or underscores." RESET << endl;
		return false;
	}

	return true;
}

//validateFirstName
bool isValidFirstName(const string& firstName) {
	// Regular expression to match only alphabetic characters
	regex pattern("^[a-zA-Z]+$");

	// Check if the first name matches the pattern
	return regex_match(firstName, pattern);
}

//validateLastName (can be space and no space)
bool isValidLastName(const string& lastName) {
	// Regular expression to match only alphabetic characters with at least one space
	regex pattern("^[a-zA-Z]+( [a-zA-Z]+)*$");

	// Check if the last name matches the pattern
	return regex_match(lastName, pattern);
}

//validatePassword
bool isValidPassword(const string& password) {
	// Regular expressions for character classes
	regex upper("(?=.*[A-Z])");
	regex lower("(?=.*[a-z])");
	regex digit("(?=.*[0-9])");
	regex special("(?=.*[^A-Za-z0-9])");

	// Check the length
	if (password.length() < 8) {
		cout << RED"\n\t\tPassword must be at least 8 characters long." RESET<< endl;
		_getch();
		return false;
	}

	// Check for required character classes
	if (!regex_search(password, upper)) {
		cout << RED"\n\t\tPassword must contain at least one uppercase letter." RESET << endl;
		_getch();
		return false;
	}
	if (!regex_search(password, lower)) {
		cout << RED"\n\t\tPassword must contain at least one lowercase letter." RESET << endl;
		_getch();
		return false;
	}
	if (!std::regex_search(password, digit)) {
		cout << RED"\n\t\tPassword must contain at least one digit." RESET<< endl;
		_getch();
		return false;
	}
	if (!std::regex_search(password, special)) {
		cout << RED"\n\t\tPassword must contain at least one special character." RESET << endl;
		_getch();
		return false;
	}
	// Password meets all criteria
	return true;
}

//validateEmail (email must has "@")
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
	cnMenu.addOption("Password");
	cnMenu.addOption("Email");
	cnMenu.addOption("Phone Number");
	cnMenu.addOption("Confirm");
	cnMenu.addOption("Back to login page");

	string firstName, lastName, password, emailAddress, username;	
	while (1) {

		switch (cnMenu.prompt()) {
		case 1:
			cout << "Enter UserId: ";
			cin >> username;
			if (validateUsername(username)) {
				newacc.UserId = username;
				cnMenu.setValue(0, newacc.UserId);
			}
			else {
				cout << RED"\n\t\tInvalid username." << endl;
				_getch();
			}
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
				cout << RED"\n\t\tInvalid first name. Please enter only alphabetic characters." RESET<< endl;
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
			cout << "Enter password (must include'A-Z''a-z''0-9''!@#$%^&*()-_+=<>?/'): ";
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
			cout << "Enter Phone Number (01157426036): ";
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
			if (newacc.UserId.empty() || newacc.first_name.empty() || newacc.last_name.empty() || newacc.password.empty() || newacc.phone_number.empty() || newacc.email.empty())
			{
				cout << RED"\n\t\tRegistration details is not completed. Please complete it." << RESET;
			}
			else 
			{
				newacc.insert();
				cout << YELLOW << "Register sucessfull !\n\n" << RESET;
				_getch();
				return;
			}
			break;
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
			while (true) 
			{
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
	profileMenu.addOption("Password");;
	profileMenu.addOption("Phone Number");
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
			cout << "Enter new password (must include'A-Z''a-z''0-9''!@#$%^&*()-_+=<>?/'): ";
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
			cout << "Enter new phone number (Exp:01157426036): ";
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
			if (temp.first_name.empty() || temp.last_name.empty() || temp.password.empty() || temp.phone_number.empty())
			{
				cout << RED"\n\t\tProfile in not completed. Please complete it."<<RESET;
			}
			else
			{
				user = temp;
				user.update();
				cout << CYAN"\t\tUpdated" << RESET;
				_getch();
			}
			break;
		case 7:
			return user;
			break;
		case 8:
			cout <<RED<< "\t\tDelete your account? [Y/N]"<<RESET;
			char confirm;
			confirm = _getch();//to enter input
			if (confirm == 'Y' || confirm == 'y') {
				user = temp;
				user.remove();
				main();
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				break;
			}
			else
			{
				cout <<RED "\n\t\tInvalid input. Pleasee enter again..."<<RESET;
				_getch();
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
			tmpString << fixed << setprecision(2)<< setw(12)  << "Account Name" << "|" << setw(18)<< "Balance"
				<< "|" << setw(23) << "Budget Amount" << "|" << setw(23) << "Budget Remainder" << "|"<<setw(23) << "Start date" << "|" << setw(23)  << "End date" <<  "|"<< endl;
			tmpString << setw(13) << "-------------" << setw(19) << "--------" << setw(24) << "--------------" << setw(24) << "-----------------" << setw(24) << "-----------" << setw(24) << "---------" << endl;
			
			for (int i = 0; i < accounts.size(); i++) {
				tmpString << setw(12) << accounts[i].account_name << "|";
				if (accounts[i].balance >0 )
				{
					tmpString << GREEN << setw(18) << accounts[i].balance  << WHITE;
				}
				else
				{
					tmpString << RED << setw(18) << accounts[i].balance << WHITE;
				}
				
				tmpString << "|" << setw(23) << accounts[i].budget_amount << "|";
				if(accounts[i].budget_remainder >=0)
				{
					tmpString << GREEN << setw(23) << accounts[i].budget_remainder << WHITE;
				}
				else
				{
					tmpString << RED << setw(23) << accounts[i].budget_remainder << WHITE;
				}
				tmpString << "|" <<setw(23) << accounts[i].start_date << "|" << setw(23) << accounts[i].end_date << "|" << endl;
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

//compare insert month and year with current month and year
bool isValidFutureMonth(const string& insertedMonth, const int insertedYear) {
	std::map<std::string, int> monthMap = {
		{"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},
		{"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8},
		{"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
	};

	time_t t = time(nullptr);
	tm currentTime;
	localtime_s(&currentTime, &t);

	int currentMonth = currentTime.tm_mon + 1;
	int currentYear = currentTime.tm_year + 1900;

	int insertedMonthNum = monthMap[insertedMonth];

	if (insertedYear > currentYear || (insertedYear == currentYear && insertedMonthNum >= currentMonth)) {
		return true;
	}
	return false;
}

//compare satrt date and end date
bool isValidEndDate(const string& startMonth, int startYear, const string& endMonth, int endYear) {
	map<string, int> monthMap = {
		{"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},
		{"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8},
		{"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
	};

	int startMonthNum = monthMap[startMonth];
	int endMonthNum = monthMap[endMonth];

	// Comparing years first, then months if years are equal
	if (endYear > startYear || (endYear == startYear && endMonthNum >= startMonthNum)) {
		return true;
	}
	return false;
}

//1)Add account
void newAccount(string UserId) 
{
	//need to redo!(compare two month but forgot to comment two year)
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
	string insertedMonth = "", insertedMonth1 = "", tempInsertedStartMonth;
	int insertedYear = 0, insertedYear1 = 0, tempInsertedStartYear;
	int startMonthNum;
	int endMonthNum;
	string tmpEndMonth, tmpStartMonth;

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
			addAccount.budget_remainder = addAccount.budget_amount;
			// Format the budget amount using formatAmount function
			formattedBudget = formatAmount(addAccount.budget_amount);
			// Set the formatted string to the menu value
			accountMenu.setValue(2, formattedBudget);
			break;
		case 4:
			while (1)
			{
				try {
					cout << "Start date:";
					// Prompt user for month input (abbreviation)
					cout << "\nEnter month abbreviation (e.g., Jan, Feb...): ";
					cin >> insertedMonth;
					// Check if the input is a valid month abbreviation (case-insensitive)
				

					if (insertedMonth != "Jan" && insertedMonth != "Feb" && insertedMonth != "Mar" &&
						insertedMonth != "Apr" && insertedMonth != "May" && insertedMonth != "Jun" &&
						insertedMonth != "Jul" && insertedMonth != "Aug" && insertedMonth != "Sep" &&
						insertedMonth != "Oct" && insertedMonth != "Nov" && insertedMonth != "Dec") {
						throw invalid_argument(RED"\t\tInvalid input for month! Please enter a valid abbreviation (e.g., Jan, Feb...).");
					}

					// Prompt user for year input
					cout << "Enter year (Example: 2023, 2024...): ";
					cin >> insertedYear;

					// Check if input failed or not a four-digit integer
					if (cin.fail() || insertedYear < 1000 || insertedYear > 9999) {
						throw invalid_argument(RED"\t\tInvalid input for year! Please enter a four-digit integer.");
					}
				}
				catch (const invalid_argument& e) {
					// Catch the exception and display the error message
					cout << e.what() << endl;

					// Clear error state to allow new input
					cin.clear(); // Clear error state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input
					_getch();
					insertedMonth = "";
					insertedYear = 0;
					break;
				}

				map<string, int> monthMap = {{"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},{"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8},{"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}};

				tmpStartMonth = insertedMonth;
				tmpEndMonth = insertedMonth1;
				startMonthNum = monthMap[tmpStartMonth];
				endMonthNum = monthMap[tmpEndMonth];

				if (insertedMonth1=="" && insertedYear1 == 0)//End_date is empty
				{
					//To check start month and start year is invalid or not compare to current month
					if (isValidFutureMonth(insertedMonth, insertedYear))
					{
						month = insertedMonth;
					}
					else {//Invalid
						cout << RED"\n\t\tInvalid insertion! You can only insert the current/future month and insert the current/future year." RESET << endl;
						_getch();
						//If invalid need to reset back
						insertedMonth = "";
						insertedYear = 0;
						break;
					}

					if (month == "Jan" || month == "Feb" || month == "Mar" || month == "Apr" || month == "May" || month == "Jun" || month == "Jul" || month == "Aug" || month == "Sep" || month == "Oct" || month == "Nov" || month == "Dec")
					{
						addAccount.start_date = "01-" + month + "-" + to_string(insertedYear);
						accountMenu.setValue(3, addAccount.start_date);
						tempInsertedStartMonth = insertedMonth;//to store true month
						tempInsertedStartYear = insertedYear;//to store true year
					}
					break;
				}
				//Check check start_date not beyond than end_date
				else if(insertedYear < insertedYear1 || (insertedYear == insertedYear1 && startMonthNum<= endMonthNum))
				{

					if (isValidFutureMonth(insertedMonth, insertedYear))//To check start month and start year is invalid or not compare to current month
					{
						
						if (isValidEndDate(insertedMonth, insertedYear, insertedMonth1, insertedYear1))//To compare start_date and end_date
						{
							month = insertedMonth;
						}
						else {
							cout << RED"\n\t\tStart date cannot be inserted as it is not before the end date." RESET << endl;
							_getch();
							insertedMonth = tempInsertedStartMonth;
							insertedYear = tempInsertedStartYear;
							break;
						}
					}
					else
					{
						cout << RED"\n\t\tInvalid insertion! You can only insert the current/future month and insert the current/future year." RESET << endl;
						_getch();
						insertedMonth = tempInsertedStartMonth;
						insertedYear = tempInsertedStartYear;
						break;
					}

					if (month == "Jan" || month == "Feb" || month == "Mar" || month == "Apr" || month == "May" || month == "Jun" || month == "Jul" || month == "Aug" || month == "Sep" || month == "Oct" || month == "Nov" || month == "Dec")
					{
						addAccount.start_date = "01-" + month + "-" + to_string(insertedYear);
						accountMenu.setValue(3, addAccount.start_date);
					}
					break;
				}
				else
				{
					cout << RED"\n\t\tStart date cannot be inserted as it is not before the end date." RESET;
					cout << endl;
					_getch();
					insertedMonth = tempInsertedStartMonth;
					insertedYear = tempInsertedStartYear;
					break;
				}
			}
			break;
		case 5:
			while (1)
			{
				if (insertedMonth == "" && insertedYear == 0)
				{
					cout << RED"\n\t\tStart date is empty! Please fill in start date first..." RESET;
					cout << endl;
					_getch();
					break;
				}
				else
				{
					try {
						cout << "End date:";
						// Prompt user for month input (abbreviation)
						cout << "\nEnter month abbreviation (e.g., Jan, Feb...): ";
						cin >> insertedMonth1;

						if (insertedMonth1 != "Jan" && insertedMonth1 != "Feb" && insertedMonth1 != "Mar" &&
							insertedMonth1 != "Apr" && insertedMonth1 != "May" && insertedMonth1 != "Jun" &&
							insertedMonth1 != "Jul" && insertedMonth1 != "Aug" && insertedMonth1 != "Sep" &&
							insertedMonth1 != "Oct" && insertedMonth1 != "Nov" && insertedMonth1 != "Dec") {
							throw invalid_argument(RED"\t\tInvalid input for month! Please enter a valid abbreviation (e.g., Jan, Feb...)." RESET);
						}

						// Prompt user for year input
						cout << "Enter year (Example: 2023, 2024...): ";
						cin >> insertedYear1;

						// Check if input failed or not a four-digit integer
						if (cin.fail() || insertedYear1 < 1000 || insertedYear1 > 9999) {
							throw invalid_argument(RED"\t\tInvalid input for year! Please enter a four-digit integer." RESET);
						}
					}
					catch (const std::invalid_argument& e) {
						// Catch the exception and display the error message
						cout << e.what() << endl;

						// Clear error state to allow new input
						cin.clear(); // Clear error state
						cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input
						insertedMonth1 = "";
						insertedYear1 = 0;
						_getch();
						break;
					}

					if (isValidFutureMonth(insertedMonth1, insertedYear1)) {//To check end month and end year is invalid or not compare to current month
						month = insertedMonth1;
					}
					else {
						cout << RED"\t\tInvalid insertion! You can only insert the current/future month and insert the current/future year." RESET << endl;
						_getch();
						insertedMonth1 = "";
						insertedYear1 = 0;
						break;
					}

					if (isValidEndDate(insertedMonth, insertedYear, insertedMonth1, insertedYear1))//To compare start_date and end_date
					{//If end date valid 
						if (month == "Jan" || month == "Mar" || month == "May" || month == "Jul" || month == "Aug" || month == "Oct" || month == "Dec")
						{
							addAccount.end_date = "31-" + month + "-" + to_string(insertedYear1);
							accountMenu.setValue(4, addAccount.end_date);
						}
						else if (month == "Apr" || month == "Jun" || month == "Sep" || month == "Nov")
						{
							addAccount.end_date = "30-" + month + "-" + to_string(insertedYear1);
							accountMenu.setValue(4, addAccount.end_date);
						}
						else if (month == "Feb")
						{
							// leap year if perfectly divisible by 400
							if ((insertedYear1) % 400 == 0)
							{
								addAccount.end_date = "29-" + month + "-" + to_string(insertedYear1);
								accountMenu.setValue(4, addAccount.end_date);
							}
							// not a leap year if divisible by 100
							// but not divisible by 400
							else if ((insertedYear1) % 100 == 0)
							{
								addAccount.end_date = "28-" + month + "-" + to_string(insertedYear1);
								accountMenu.setValue(4, addAccount.end_date);
							}
							// leap year if not divisible by 100
							// but divisible by 4
							else if ((insertedYear1) % 4 == 0)
							{
								addAccount.end_date = "29-" + month + "-" + to_string(insertedYear1);
								accountMenu.setValue(4, addAccount.end_date);
							}
							// all other years are not leap years
							else
							{
								addAccount.end_date = "28-" + month + "-" + to_string(insertedYear1);
								accountMenu.setValue(4, addAccount.end_date);
							}

						}
						break;
					}
					else {
						cout << RED"\n\t\tEnd date cannot be inserted as it is not after the start date." RESET<< endl;
						_getch();
						cout << endl;
						break;
					}
				}
			}
			break;
		case 6:
			if (addAccount.account_name.empty() || addAccount.start_date.empty() || addAccount.end_date.empty())
			{
				cout << RED"\n\t\tAccount is not complete to fill in." << RESET << endl;
			}
			else 
			{
				cout << CYAN << "\t\tAre you sure to confirm? [Y/N]" << RESET;
				cout << RED"\n\t\tNote: 'Start Date' will not be change again!\n" << RESET;
				char confirm;
				confirm = _getch();//to enter input
				if (confirm == 'Y' || confirm == 'y') {
					addAccount.UserID = UserId;
					addAccount.addAccount();
					cout << CYAN << "\t\tAdd sucessful !" << RESET;
					_getch();
					return;
					break;
				}
				else if (confirm == 'N' || confirm == 'n')
				{
					break;
				}
				else
				{
					cout << RED "\n\t\tInvalid input. Pleasee enter again..." << RESET;
					_getch();
				}
				break;
			}
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
			tmpString << setw(6) << "-------------" << endl;
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
			if (account.account_name.empty())
			{
				getline(cin, account.account_name);
			}
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

//To get month of abbreviation
string getMonthAbbreviation(const string& dateStr) {
	// Months abbreviation array
	const std::string months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
								  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	// Extract month abbreviation (e.g., "Dec" from "12-Dec-2023")
	std::string extractedMonth = dateStr.substr(3, 3); // Assuming the month abbreviation is always at positions 3-5

	return extractedMonth;
}

//to get year
int getYear(const string& dateStr) {
	// Extract year (e.g., 2023 from "12-Dec-2023")
	istringstream iss(dateStr);
	string day, month, year;
	getline(iss, day, '-');
	getline(iss, month, '-');
	getline(iss, year, '-');
	return std::stoi(year);
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
	
	string accountName = temp.account_name;
	string insertedMonth1, endMonth, startMonth, dateStr, dateStr1;
	int insertedYear1,startYear,endYear;
	
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
				getline(cin, temp.account_name);
			}
			break;
		case 2:
			cout << "Enter new balance: RM ";
			cin >> temp.balance;
			if (temp.balance <= 0)
			{
				cout << RED"\n\t\tNew balance is insufficient!" << RESET << endl << endl;
				_getch();
				cin >> temp.balance;
			}
			break;
		case 3:
			cout << "Enter new budget amount: RM ";
			cin >> temp.budget_amount;
			temp.budget_remainder = temp.budget_amount;
			if (temp.budget_amount <= 0)
			{
				cout << RED"\n\t\tNew budget amount is insufficient!" << RESET << endl << endl;
				cin >> temp.budget_amount;
				temp.budget_remainder = temp.budget_amount;
			}
			break;
		case 4:
			break;
		case 5:
			dateStr = account.start_date;
			startMonth = getMonthAbbreviation(dateStr);
			startYear = getYear(dateStr);
			dateStr1 = account.end_date;
			endMonth = getMonthAbbreviation(dateStr1);
			endYear = getYear(dateStr1);
			while (1)
			{
				try {
					cout << "End date:";
					// Prompt user for month input (abbreviation)
					cout << "\nEnter month abbreviation (e.g., Jan, Feb...): ";
					cin >> insertedMonth1;

					if (insertedMonth1 != "Jan" && insertedMonth1 != "Feb" && insertedMonth1 != "Mar" &&
						insertedMonth1 != "Apr" && insertedMonth1 != "May" && insertedMonth1 != "Jun" &&
						insertedMonth1 != "Jul" && insertedMonth1 != "Aug" && insertedMonth1 != "Sep" &&
						insertedMonth1 != "Oct" && insertedMonth1 != "Nov" && insertedMonth1 != "Dec") {
						throw invalid_argument(RED"\t\tInvalid input for month! Please enter a valid abbreviation (e.g., Jan, Feb...)." RESET);
					}

					// Prompt user for year input
					cout << "Enter year (Example: 2023, 2024...): ";
					cin >> insertedYear1;

					// Check if input failed or not a four-digit integer
					if (cin.fail() || insertedYear1 < 1000 || insertedYear1 > 9999) {
						throw invalid_argument(RED"\t\tInvalid input for year! Please enter a four-digit integer." RESET);
					}
				}
				catch (const std::invalid_argument& e) {
					// Catch the exception and display the error message
					cout << e.what() << endl;

					// Clear error state to allow new input
					cin.clear(); // Clear error state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input
					insertedMonth1 = endMonth;
					insertedYear1 = endYear;
					_getch();
				}
			
				if (isValidFutureMonth(insertedMonth1, insertedYear1)) {
					month = insertedMonth1;
				}
				else {
					cout << RED"\t\tInvalid insertion! You can only insert the current/future month and insert the current/future year." RESET << endl;
					_getch();
					insertedMonth1 = endMonth;
					insertedYear1 = endYear;
					break;
				}

				if (isValidEndDate(startMonth, startYear, insertedMonth1, insertedYear1))
				{//If end date valid 
					if (month == "Jan" || month == "Mar" || month == "May" || month == "Jul" || month == "Aug" || month == "Oct" || month == "Dec")
					{
						temp.end_date = "31-" + month + "-" + to_string(insertedYear1);
					}
					else if (month == "Apr" || month == "Jun" || month == "Sep" || month == "Nov")
					{
						temp.end_date = "30-" + month + "-" + to_string(insertedYear1);
					}
					else if (month == "Feb")
					{
						if ((insertedYear1) % 400 == 0)
						{
							temp.end_date = "29-" + month + "-" + to_string(insertedYear1);
						}
						else if ((insertedYear1) % 100 == 0)
						{
							temp.end_date = "28-" + month + "-" + to_string(insertedYear1);
						}
						else if ((insertedYear1) % 4 == 0)
						{
							temp.end_date = "29-" + month + "-" + to_string(insertedYear1);
						}
						else
						{
							temp.end_date = "28-" + month + "-" + to_string(insertedYear1);
						}
					}
					break;
				}
				else {
					cout << RED"\t\n\nEnd date cannot be inserted as it is not after the start date." RESET << endl;
					cout << endl;
					_getch();
					insertedMonth1 = endMonth;
					insertedYear1 = endYear;
					break;
				}
			}
			break;
		case 6:
			temp = account;
			break;
		case 7:
			account = temp;
			account.UserID = UserID;
			account.update();
			cout << CYAN << "\t\tUpdated" << RESET;
			_getch();
		case 8:
			return ;
			break;
		case 9:
			cout << RED << "Delete your account? [Y/N]";
			cout << "\nNote: Are you sure to delete account? All the transaction also will be delete." << RESET<< endl;
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') {
				account = temp;
				account.UserID = UserID;
				
				// delete transaction first then delete account
				account.deleteAllTransaction(account.UserID, account.account_name);
				account.removeAccount(account.UserID);
				return ;
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				break;
			}
			else
			{
				cout << "\nInvalid input! Please input Y/N only";
				_getch();
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

//To compare end_date with current_current date
bool endDateWithCurrentDate(const string& startMonth, int startYear, const string& endMonth, int endYear) {
	std::map<std::string, int> monthMap = {
		{"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},
		{"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8},
		{"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
	};

	// Get the current date
	auto now = chrono::system_clock::now();
	time_t currentTime = chrono::system_clock::to_time_t(now);
	tm currentDate = {};
	localtime_s(&currentDate, &currentTime);

	int currentMonth = currentDate.tm_mon + 1; // tm_mon is 0-indexed
	int currentYear = currentDate.tm_year + 1900; // tm_year is years since 1900

	int startMonthNum = monthMap[startMonth];
	int endMonthNum = monthMap[endMonth];

	// Comparing years first, then months if years are equal
	if (endYear > currentYear || (endYear == currentYear && endMonthNum >= currentMonth))
	{//compare current_date with end_date
		if (startYear < currentYear || (startYear == currentYear && startMonthNum <= currentMonth))
		{//compare current_date with start_date
			return true;
		}
	}
	return false;
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
	homeTrans.addOption("Budeget remainder");//7
	homeTrans.addOption("Confirm");//8
	homeTrans.addOption("Back to transaction page");//9

	string formattedTransAmount;
	string formattednewBalance;
	string formattednewBudgetAmount;
	string confirmation;
	string accountName;
	double tempAmount;
	double tempBudget;
	string startDate, startMonth, endDate, endMonth;
	int startOfYear,endOfYear;


	while (1)
	{
		homeTrans.setValue(0, account_name);
		account.getAccount(UserID, account_name);//get account data
		account.getStartDateAndEndDate(account.AccountID);
		addTrans.AccountID = account.AccountID;

		startDate = account.start_date;
		endDate = account.end_date;
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
		homeTrans.header = "\t\tAdd Transaction";
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
			if (addTrans.transaction_amount <= 0)
			{
				cout << RED"\t\tInvalid ! Amount of transaction is insufficient." RESET;
				cout << "\nPlease enter amount again: ";
				cin >> addTrans.transaction_amount;
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

				tempBudget = account.budget_remainder - addTrans.transaction_amount;
				formattednewBudgetAmount = formatAmount(tempBudget);
				homeTrans.setValue(6, formattednewBudgetAmount);
			}
			else
			{
				tempAmount = account.balance + addTrans.transaction_amount;
				formattednewBalance = formatAmount(tempAmount);
				homeTrans.setValue(5, formattednewBalance);

				formattednewBudgetAmount = formatAmount(account.budget_remainder);
				homeTrans.setValue(6, formattednewBudgetAmount);
			}
			break;
		case 7:
			break;
		case 8:
			addTrans.newbalance = tempAmount;
			account.balance = tempAmount;
			account.budget_remainder = tempBudget;

			//string space wrongly
			startMonth = getMonthAbbreviation(startDate);
			startOfYear = getYear(startDate);
			endMonth = getMonthAbbreviation(endDate);
			endOfYear = getYear(endDate);
		
			if (endDateWithCurrentDate(startMonth, startOfYear, endMonth, endOfYear))
			{
				addTrans.addTrans();
				account.updateAfterTrans();
				cout << CYAN"\n\t\tTransaction is create" RESET;
				_getch();
				return TransactionPage(UserID);
				break;
			}
			else 
			{
				cout << RED"\n\t\tThe end date is in the past. Please change end date first!" << RESET << endl;
				_getch();
			}
			break;
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
			tmpString << fixed << setprecision(2) << setw(15) << "Transaction ID" << "|" << setw(25)
				<< "Transaction Type" << "|" << setw(28) << "Transaction Amount" << "|" << setw(23)
				<< "Category" << "|" << setw(23) << "Description" << "|" << setw(18) << "Newbalance" << "|"
				<< setw(28) << "Transaction date" << "|" << endl;
			tmpString << setw(16) << "---------------" << setw(26) << "-----------------" << setw(29) 
				<< "-------------------" << setw(24) << "---------" << setw(24) << "------------" << setw(19) 
				<< "-----------" << setw(29) << "-----------------" << endl;

			for (int i = 0; i < trans.size(); i++) 
			{
				tmpString << setw(15) << trans[i].TransactionID << "|" << setw(25) << trans[i].transaction_type << "|";

				if (trans[i].transaction_type == "Deposit")
				{
					tmpString << setw(24) << GREEN "+" <<setw(9)<< trans[i].transaction_amount << WHITE;
				}
				else
				{
					tmpString << setw(24) << RED "-" << setw(9) << trans[i].transaction_amount << WHITE;
				}

				tmpString << "|" << setw(23) << trans[i].category << "|" << setw(23) << trans[i].description << "|";

				if (trans[i].newbalance < 0)
				{
					tmpString << RED << setw(18) << trans[i].newbalance << WHITE;
				}
				else
				{
					tmpString << GREEN <<setw(18) << trans[i].newbalance << WHITE;
				}
				tmpString << "|" << setw(28) << trans[i].transaction_date << "|" << endl;
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
	string startOfMonth,startDate,endOfMonth,endDate;
	int startOfYear,endOfYear;

	account.getBlcBdg(UserId, account.AccountID);//get balance and budget remainder

	oldTransAmount = temp.transaction_amount;
	oldNewBalance = temp.newbalance;

	if (temp.transaction_type == "Expenses")
	{
		account.balance = oldTransAmount + oldNewBalance;//get original balance

		account.budget_amount = account.budget_amount + oldTransAmount;//get original budget
		initialBalance = account.balance;
		initialBudget = account.budget_remainder;
	}
	else if (temp.transaction_type == "Deposit")
	{
		account.balance = oldNewBalance - oldTransAmount;
		initialBalance = account.balance;
	}

	while (1)
	{
		account.getStartDateAndEndDate(account.AccountID);
		startDate = account.start_date;
		endDate = account.end_date;

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
			if (temp.transaction_amount <=0)
			{
				cout << RED"\t\tInvalid ! Amount of transaction is insufficient." RESET;
				cout << "\nPlease enter amount again: ";
				cin >> temp.transaction_amount;
			}
			break;
		case 3:
			cout << "Enter category (Example: Food & Beverage, Transportation, Bills): ";
			getline(cin, temp.category);
			if (temp.category.empty())
			{
				getline(cin, temp.category);
			}
			break;
		case 4:
			cout << "Description: ";
			if (temp.transaction_type == "Expenses")
			{
				getline(cin, temp.description);
				if (temp.description.empty())
				{
					getline(cin, temp.description);
				}
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
				tempBudget = account.budget_remainder;
			}

			temp.newbalance = tempAmount;
			account.balance = tempAmount;
			account.budget_remainder = tempBudget;

			break;
		case 6:
			temp = transaction;
			break;
		case 7:
			startOfMonth = getMonthAbbreviation(startDate);
			startOfYear = getYear(startDate);
			endOfMonth = getMonthAbbreviation(endDate);
			endOfYear = getYear(endDate);
			//to check transaction is can/cannot to edit and makesure transaction date is between the range of start date and end date of budget.
			if (endDateWithCurrentDate(startOfMonth, startOfYear, endOfMonth, endOfYear))
			{
				transaction = temp;
				transaction.updateTrans(transaction.TransactionID);
				account.updateAfterTrans();
				cout << CYAN"\n\t\tUpdated !" RESET;
				_getch();
			}
			else
			{
				cout << RED "\n\t\tThis transaction cannot be edit due to end date is in the past. Please change end date of budget first!" << RESET << endl;
				_getch();
			}
			break;
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
			else if(confirm)
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
	TH.header = "\tStatistic";
	TH.addOption(CYAN"Number of transaction(per month)" RESET);//1
	TH.addOption(CYAN"Net transaction amount (per month)" RESET);//2
	TH.addOption(CYAN"Total transaction amount(per month/per week)" RESET);//3
	TH.addOption("Graph");//4
	TH.addOption("Back to User Page");//5

	Menu SubTA;
	SubTA.addOption("This month");
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
			graph(UserId);
			break;
		case 5:
			return;
			break;
		default:
			break;
		}
	}
}


bool isValidYear(const string& input) {
	// Check if the input contains only digits
	for (char ch : input) {
		if (!isdigit(ch)) {
			return false;
		}
	}

	// Convert the input to an integer
	int year = std::stoi(input);

	// Check if the year is a four-digit number
	if (input.length() != 4 || year < 0) {
		return false;
	}

	// Get the current year
	std::time_t currentTime = std::time(nullptr);
	std::tm currentTimeStruct;
	localtime_s(&currentTimeStruct, &currentTime);
	const int currentYear = currentTimeStruct.tm_year + 1900;

	// Check if the year is within a reasonable range
	if (year < 1900 || year > currentYear) {
		return false;
	}

	return true;
}

bool isValidYearAndMonth(const string& yearInput, const string& monthInput) {
	// Check if the year input contains only digits
	for (char ch : yearInput) {
		if (!isdigit(ch)) {
			return false;
		}
	}

	// Check if the month input contains only digits
	for (char ch : monthInput) {
		if (!isdigit(ch)) {
			return false;
		}
	}

	// Convert year and month inputs to integers
	int year = std::stoi(yearInput);
	int month = std::stoi(monthInput);

	// Check if the year is a four-digit number and the month is within range (1 to 12)
	if (yearInput.length() != 4 || year < 0 || month < 1 || month > 12) {
		return false;
	}

	// Get the current year and month
	std::time_t currentTime = std::time(nullptr);
	std::tm currentTimeStruct;
	localtime_s(&currentTimeStruct, &currentTime);
	const int currentYear = currentTimeStruct.tm_year + 1900;
	const int currentMonth = currentTimeStruct.tm_mon + 1; // Adding 1 because tm_mon is zero-based

	// Check if the year is within a reasonable range and if the month is not in the future
	if (year < 1900 || year > currentYear || (year == currentYear && month > currentMonth)) {
		return false;
	}

	return true;
}

void graph(string UserId)
{
	Menu GPH;
	GPH.header = "\tGraph";
	GPH.addOption("Deposit Versus Expenses in same month");
	GPH.addOption("Deposit Versus Expenses between two different years");
	GPH.addOption("Back to statistic");

	while (1)
	{
		double DPT = 0.0, EPS = 0.0, yearOfDeposit = 0.0, yearOfExpenses = 0.0, yearOfDeposit1 = 0.0, yearOfExpenses1 = 0.0, NetTrs = 0.0, yearOfNetTransaction = 0.0, yearOfNetTransaction1 = 0.0;
		double PDPT, PEPS, PDPT1, PEPS1, PDPT2, PEPS2, PBTY, PBTY1, PBTY2, PBTY3, PBTY4, PBTY5, PDBTY, PDBTY1, PDBTY2, PDBTY3, PDBTY4, PDBTY5, Ave, Ave1, Ave2;
		int number1, number2, number3, number4, number5, number6, number7, CHGPTV, number8, CHGPTV1, number9, CHGPTV2;
		int x, y, a, b, YOD, YOE, YOB, YOD1, YOE1, YOB1;
		string Deposit = "", Expenses = "", stars = "", Deposit1 = "", Expenses1 = "", Deposit2 = "", Expenses2 = "", NetTrans = "", NetTrans1 = "", NetTrans2 = "";
		string tmpMonth;

		switch (GPH.prompt())
		{
		case 1:
			while (1)//infinite loop that runs continually until a break statement is explicitly issued
			{	
				try {
					// Prompt user for month input
					cout << "\nEnter month [integer number from 1 to 12]: ";
					cin >> x;

					// Check if input failed or not within the valid range (1-12)
					if (cin.fail() || x < 1 || x > 12) {
						throw invalid_argument(RED"Invalid input for month! Please enter an integer between 1 and 12." RESET);
					}

					// Prompt user for year input
					cout << "\nEnter year (Example: 2023, 2024...): ";
					cin >> y;

					// Check if input failed or not a four-digit integer
					if (cin.fail() || y < 1000 || y > 9999) {
						throw invalid_argument(RED"Invalid input for year! Please enter a four-digit integer." RESET);
					}
				}
				catch (const invalid_argument& e) {
					// Catch the exception and display the error message
					cout << e.what() << std::endl;

					// Clear error state to allow new input
					cin.clear(); // Clear error state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input
				}

				if (isValidYearAndMonth(to_string(y), to_string(x)))
				{
					int j = 0, h = 0, t = 0;

					DPT = Transaction::totalDeposit(UserId, x, y);
					EPS = Transaction::totalExpenses(UserId, x, y);
					if (x == 1)
					{
						tmpMonth = "Jan";
					}
					else if (x == 2)
					{
						tmpMonth = "Feb";
					}
					else if (x == 3)
					{
						tmpMonth = "Mar";
					}
					else if (x == 4)
					{
						tmpMonth = "Apr";
					}
					else if (x == 5)
					{
						tmpMonth = "May";
					}
					else if (x == 6)
					{
						tmpMonth = "Jun";
					}
					else if (x == 7)
					{
						tmpMonth = "Jul";
					}
					else if (x == 3)
					{
						tmpMonth = "Aug";
					}
					else if (x == 9)
					{
						tmpMonth = "Sep";
					}
					else if (x == 10)
					{
						tmpMonth = "Oct";
					}
					else if (x == 11)
					{
						tmpMonth = "Nov";
					}
					else if (x == 12)
					{
						tmpMonth = "Dec";
					}

					NetTrs = DPT - EPS;

					if (DPT > 0 && DPT <= 1000)
					{
						h = 10;
						number1 = static_cast<int>(DPT / h); //converts a given expression to a specified type
						for (int i = 0; i < number1; i++)
						{
							stars += "*";
						}
						Deposit += stars;
					}
					else if (DPT > 1000 && DPT <= 5000)
					{
						h = 50;
						number2 = static_cast<int>(DPT / h);
						for (int i = 0; i < number2; i++)
						{
							stars += "*";
						}
						Deposit += stars;
					}
					else if (DPT < 0)
					{
						throw out_of_range("Total deposit value is negative.");
					}
					else if (DPT > 5000 && DPT <= 25000)
					{
						h = 250;
						number2 = static_cast<int>(DPT / h);
						for (int i = 0; i < number2; i++)
						{
							stars += "*";
						}
						Deposit += stars;
					}
					else if (DPT > 25000)
					{
						h = 500;
						number2 = static_cast<int>(EPS / h);
						for (int i = 0; i < number2; i++)
						{
							stars += "*";
						}
						Deposit += stars;
					}
					stars = "";

					if (EPS > 0 && EPS <= 1000)
					{
						j = 10;
						number2 = static_cast<int>(EPS / j);
						for (int i = 0; i < number2; i++)
						{
							stars += "*";
						}
						Expenses += stars;
					}
					else if (EPS > 1000 && EPS <= 5000)
					{
						j = 50;
						number2 = static_cast<int>(EPS / j);
						for (int i = 0; i < number2; i++)
						{
							stars += "*";
						}
						Expenses += stars;
					}
					else if (EPS < 0)
					{
						throw out_of_range("Total expenses value is negative.");
					}
					else if (EPS > 5000 && EPS <= 25000)
					{
						j = 250;
						number2 = static_cast<int>(EPS / j);
						for (int i = 0; i < number2; i++)
						{
							stars += "*";
						}
						Expenses += stars;
					}
					else if (EPS > 25000)
					{
						j = 500;
						number2 = static_cast<int>(EPS / j);
						for (int i = 0; i < number2; i++)
						{
							stars += "*";
						}
						Expenses += stars;
					}

					stars = "";

					if (NetTrs > 0 && NetTrs <= 1000)
					{
						t = 10;
						number7 = static_cast<int>(NetTrs / t);
						for (int i = 0; i < number7; i++)
						{
							stars += "*";
						}
						NetTrans += stars;
					}
					else if (NetTrs > 1000 && NetTrs <= 5000)
					{
						t = 50;
						number7 = static_cast<int>(NetTrs / t);
						for (int i = 0; i < number7; i++)
						{
							stars += "*";
						}
						NetTrans += stars;
					}
					else if (NetTrs < 0)
					{
						t = 100;
						number7 = static_cast<int>(NetTrs / t);
						CHGPTV = abs(number7);//to make in positive value (clear negative sign)
						for (int i = 0; i < CHGPTV; i++)
						{
							stars += "*";
						}
						NetTrans += stars;
					}
					else if (NetTrs > 5000 && NetTrs <= 25000)
					{
						t = 250;
						number7 = static_cast<int>(NetTrs / t);
						for (int i = 0; i < number7; i++)
						{
							stars += "*";
						}
						NetTrans += stars;
					}
					else if (NetTrs > 25000)
					{
						t = 500;
						number7 = static_cast<int>(NetTrs / t);
						for (int i = 0; i < number7; i++)
						{
							stars += "*";
						}
						NetTrans += stars;
					}
					PDPT = (DPT / (DPT + EPS)) * 100;
					PEPS = (EPS / (DPT + EPS)) * 100;
					cout << "\n\n";
					cout << "--------------------------------------------- " << YELLOW  "Date(MM-YYYY): " << x << "-" << y << RESET << " -------------------------------------------------------";
					cout << "\n\nDeposit:                " << CYAN << Deposit << RESET << "  "; printf("%.2f", DPT);
					cout << "\nExpeneses:              " << CYAN << Expenses << RESET << "  "; printf("%.2f", EPS);
					if (NetTrs < 0)
					{
						cout << "\nNet transaction amount: " << RED << NetTrans << RESET << "  "; printf("%.2f", NetTrs);
					}
					else
					{
						cout << "\nNet transaction amount: " << CYAN << NetTrans << RESET << "  "; printf("%.2f", NetTrs);
					}
					cout << "\n\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®deposit¡¯" << RESET << " means" << GREEN << " RM " << h << RESET;
					cout << "\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®expenses¡¯" << RESET << " means" << GREEN << " RM " << j << RESET;
					if (NetTrs < 0)
					{
						cout << "\nOne" << RED << " '*' " << RESET << "for " << BLUE << "¡®net trnasaction amount¡¯" << RESET << " means negative value" << GREEN << " RM " << t << RESET;
					}
					else
					{
						cout << "\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®net trnasaction amount¡¯" << RESET << " means negative value" << GREEN << " RM " << t << RESET;
					}
					cout << "\n________________________________________________________________________________________";
					cout << "\n\t\t\t\tReport";
					cout << "\n________________________________________________________________________________________";
					cout << "\n\nPercentage of deposit: "; printf("%.2f", PDPT); cout << "%";
					cout << "\nPercentage of expenses: "; printf("%.2f", PEPS); cout << "%";

					_getch();
					break;
				}
			}
			break;
		case 2:

			while (1)
			{
				try {
					// Prompt user for year input
					cout << "\nEnter first year: ";
					cin >> a;

					// Check if input failed or not a four-digit integer
					if (cin.fail() || a < 1000 || a > 9999) {
						throw invalid_argument(RED"Invalid input for year! Please enter a four-digit integer." RESET);
					}
				}
				catch (const invalid_argument& e) {
					// Catch the exception and display the error message
					cout << e.what() << endl;

					// Clear error state to allow new input
					cin.clear(); // Clear error state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input
				}

				if (isValidYear(to_string(a)))
				{
					YOE = 0;
					YOD = 0;
					YOB = 0;
					yearOfDeposit = Transaction::totalDepositInYear(UserId, a);
					yearOfExpenses = Transaction::totalExpensesInYear(UserId, a);
					yearOfNetTransaction = yearOfDeposit - yearOfExpenses;
					break;
					//budgetRemainderYear()...
				}
			}

			if (yearOfDeposit > 0 && yearOfDeposit <= 10000)
			{
				YOD = 100;
				number3 = static_cast<int>(yearOfDeposit / YOD);
				for (int i = 0; i < number3; i++)
				{
					stars += "*";
				}
				Deposit1 += stars;
			}
			else if (yearOfDeposit < 0) {
				throw out_of_range("Deposit value is negative.");
			}
			else if (yearOfDeposit > 10000 && yearOfDeposit <= 50000)
			{
				YOD = 500;
				number3 = static_cast<int>(yearOfDeposit / YOD);
				for (int i = 0; i < number3; i++)
				{
					stars += "*";
				}
				Deposit1 += stars;
			}
			else if (yearOfDeposit > 50000 && yearOfDeposit <= 250000)
			{
				YOD = 2500;
				number3 = static_cast<int>(yearOfDeposit / YOD);
				for (int i = 0; i < number3; i++)
				{
					stars += "*";
				}
				Deposit1 += stars;
			}
			else if (yearOfDeposit > 250000)
			{
				YOD = 5000;
				number3 = static_cast<int>(yearOfDeposit / YOD);
				for (int i = 0; i < number3; i++)
				{
					stars += "*";
				}
				Deposit1 += stars;
			}
			stars = "";

			if (yearOfExpenses > 0 && yearOfExpenses <= 10000)
			{
				YOE = 100;
				number4 = static_cast<int>(yearOfExpenses / YOE);
				for (int i = 0; i < number4; i++)
				{
					stars += "*";
				}
				Expenses1 += stars;
			}
			else if (yearOfExpenses < 0) {
				throw out_of_range("Expenses value is negative.");
			}
			else if (yearOfExpenses > 10000 && yearOfExpenses <= 50000)
			{
				YOE = 500;
				number4 = static_cast<int>(yearOfExpenses / YOE);
				for (int i = 0; i < number4; i++)
				{
					stars += "*";
				}
				Expenses1 += stars;
			}
			else if (yearOfExpenses > 50000 && yearOfExpenses <= 250000)
			{
				YOE = 2500;
				number4 = static_cast<int>(yearOfExpenses / YOE);
				for (int i = 0; i < number4; i++)
				{
					stars += "*";
				}
				Expenses1 += stars;
			}
			else if (yearOfExpenses > 250000)
			{
				YOE = 5000;
				number4 = static_cast<int>(yearOfExpenses / YOE);
				for (int i = 0; i < number4; i++)
				{
					stars += "*";
				}
				Expenses1 += stars;
			}
			stars = "";

			if (yearOfNetTransaction > 0 && yearOfNetTransaction <= 10000)
			{
				YOB = 100;
				number8 = static_cast<int>(yearOfNetTransaction / YOB);
				for (int i = 0; i < number8; i++)
				{
					stars += "*";
				}
				NetTrans1 += stars;
			}
			else if (yearOfNetTransaction < 0)
			{
				YOB = 100;
				number8 = static_cast<int>(yearOfNetTransaction / YOB);
				CHGPTV1 = abs(number8);
				for (int i = 0; i < CHGPTV1; i++)
				{
					stars += "*";
				}
				NetTrans1 += stars;
			}
			else if (yearOfNetTransaction > 10000 && yearOfNetTransaction <= 50000)
			{
				YOB = 500;
				number8 = static_cast<int>(yearOfNetTransaction / YOB);
				for (int i = 0; i < number8; i++)
				{
					stars += "*";
				}
				NetTrans1 += stars;
			}
			else if (yearOfNetTransaction > 50000 && yearOfNetTransaction <= 250000)
			{
				YOB = 2500;
				number8 = static_cast<int>(yearOfNetTransaction / YOB);
				for (int i = 0; i < number8; i++)
				{
					stars += "*";
				}
				NetTrans1 += stars;
			}
			else if (yearOfNetTransaction > 250000)
			{
				YOB = 5000;
				number8 = static_cast<int>(yearOfNetTransaction / YOB);
				for (int i = 0; i < number8; i++)
				{
					stars += "*";
				}
				NetTrans1 += stars;
			}

			while (1)
			{
				try {
					// Prompt user for year input
					cout << "\nEnter second year: ";
					cin >> b;

					// Check if input failed or not a four-digit integer
					if (cin.fail() || b < 1000 || b > 9999) {
						throw invalid_argument(RED"Invalid input for year! Please enter a four-digit integer." RESET);
					}
				}
				catch (const invalid_argument& e) {
					// Catch the exception and display the error message
					cout << e.what() << endl;

					// Clear error state to allow new input
					cin.clear(); // Clear error state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input
				}
				if (isValidYear(to_string(b)))
				{
					YOD1 = 0;
					YOE1 = 0;
					yearOfDeposit1 = Transaction::totalDepositInYear(UserId, b);
					yearOfExpenses1 = Transaction::totalExpensesInYear(UserId, b);
					yearOfNetTransaction1 = yearOfDeposit1 - yearOfExpenses1;
					break;
				}
			}

			if (yearOfDeposit1 > 0 && yearOfDeposit1 <= 10000)
			{
				YOD1 = 100;
				number5 = static_cast<int>(yearOfDeposit1 / YOD1);
				for (int i = 0; i < number5; i++)
				{
					stars += "*";
				}
				Deposit2 += stars;
			}
			else if (yearOfDeposit1 < 0) {
				throw out_of_range("Deposit value is negative.");
			}
			else if (yearOfDeposit1 > 10000 && yearOfDeposit1 <= 50000)
			{
				YOD1 = 500;
				number5 = static_cast<int>(yearOfDeposit1 / YOD1);
				for (int i = 0; i < number5; i++)
				{
					stars += "*";
				}
				Deposit2 += stars;
			}
			else if (yearOfDeposit1 > 50000 && yearOfDeposit1 <= 250000)
			{
				YOD1 = 2500;
				number5 = static_cast<int>(yearOfDeposit1 / YOD1);
				for (int i = 0; i < number5; i++)
				{
					stars += "*";
				}
				Deposit2 += stars;
			}
			else if (yearOfDeposit1 > 250000)
			{
				YOD1 = 5000;
				number5 = static_cast<int>(yearOfDeposit1 / YOD1);
				for (int i = 0; i < number5; i++)
				{
					stars += "*";
				}
				Deposit2 += stars;
			}
			stars = "";

			if (yearOfExpenses1 > 0 && yearOfExpenses1 <= 10000)
			{
				YOE1 = 100;
				number6 = static_cast<int>(yearOfExpenses1 / YOE1);
				for (int i = 0; i < number6; i++)
				{
					stars += "*";
				}
				Expenses2 += stars;
			}
			else if (yearOfExpenses1 < 0) {
				throw out_of_range("Expenses value is negative.");
			}
			else if (yearOfExpenses1 > 10000 && yearOfExpenses1 <= 50000)
			{
				YOE1 = 500;
				number5 = static_cast<int>(yearOfExpenses1 / YOE1);
				for (int i = 0; i < number5; i++)
				{
					stars += "*";
				}
				Expenses2 += stars;
			}
			else if (yearOfExpenses1 > 50000 && yearOfExpenses1 <= 250000)
			{
				YOE1 = 2500;
				number5 = static_cast<int>(yearOfExpenses1 / YOE1);
				for (int i = 0; i < number5; i++)
				{
					stars += "*";
				}
				Expenses2 += stars;
			}
			else if (yearOfExpenses > 250000)
			{
				YOE1 = 5000;
				number5 = static_cast<int>(yearOfExpenses1 / YOE1);
				for (int i = 0; i < number5; i++)
				{
					stars += "*";
				}
				Expenses2 += stars;
			}
			stars = "";

			if (yearOfNetTransaction1 > 0 && yearOfNetTransaction1 <= 10000)
			{
				YOB1 = 100;
				number9 = static_cast<int>(yearOfNetTransaction1 / YOB1);
				for (int i = 0; i < number9; i++)
				{
					stars += "*";
				}
				NetTrans2 += stars;
			}
			else if (yearOfNetTransaction1 < 0)
			{
				YOB1 = 100;
				number9 = static_cast<int>(yearOfNetTransaction1 / YOB1);
				CHGPTV2 = abs(number9);
				for (int i = 0; i < CHGPTV2; i++)
				{
					stars += "*";
				}
				NetTrans2 += stars;
			}
			else if (yearOfNetTransaction1 > 10000 && yearOfNetTransaction1 <= 50000)
			{
				YOB1 = 500;
				number9 = static_cast<int>(yearOfNetTransaction1 / YOB1);
				for (int i = 0; i < number9; i++)
				{
					stars += "*";
				}
				NetTrans2 += stars;
			}
			else if (yearOfNetTransaction1 > 50000 && yearOfNetTransaction1 <= 250000)
			{
				YOB1 = 2500;
				number9 = static_cast<int>(yearOfNetTransaction1 / YOB1);
				for (int i = 0; i < number9; i++)
				{
					stars += "*";
				}
				NetTrans2 += stars;
			}
			else if (yearOfNetTransaction1 > 250000)
			{
				YOB1 = 5000;
				number9 = static_cast<int>(yearOfNetTransaction1 / YOB1);
				for (int i = 0; i < number9; i++)
				{
					stars += "*";
				}
				NetTrans2 += stars;
			}

			//Calculate percentage between deposit and expenses
			PDPT1 = (yearOfDeposit / (yearOfDeposit + yearOfExpenses)) * 100;
			PEPS1 = (yearOfExpenses / (yearOfDeposit + yearOfExpenses)) * 100;
			PDPT2 = (yearOfDeposit1 / (yearOfDeposit1 + yearOfExpenses1)) * 100;
			PEPS2 = (yearOfExpenses1 / (yearOfDeposit1 + yearOfExpenses1)) * 100;

			//calaculate precentage of deposit/expenses/net transaction amount between two years
			PBTY = (yearOfDeposit / (yearOfDeposit + yearOfDeposit1)) * 100;
			PBTY1 = (yearOfDeposit1 / (yearOfDeposit + yearOfDeposit1)) * 100;
			PBTY2 = (yearOfExpenses / (yearOfExpenses + yearOfExpenses1)) * 100;
			PBTY3 = (yearOfExpenses1 / (yearOfExpenses + yearOfExpenses1)) * 100;
			PBTY4 = (yearOfNetTransaction / (yearOfNetTransaction + yearOfNetTransaction1)) * 100;
			PBTY5 = (yearOfNetTransaction1 / (yearOfNetTransaction + yearOfNetTransaction1)) * 100;

			//calaculate precentage of deposit/expenses/net transaction amount compare  two years
			PDBTY = ((yearOfDeposit - yearOfDeposit1) / yearOfDeposit) * 100;
			PDBTY1 = ((yearOfDeposit1 - yearOfDeposit) / yearOfDeposit1) * 100;
			PDBTY2 = ((yearOfExpenses - yearOfExpenses1) / yearOfExpenses) * 100;
			PDBTY3 = ((yearOfExpenses1 - yearOfExpenses) / yearOfExpenses1) * 100;
			PDBTY4 = ((yearOfNetTransaction - yearOfNetTransaction1) / yearOfNetTransaction) * 100;
			PDBTY5 = ((yearOfNetTransaction1 - yearOfNetTransaction) / yearOfNetTransaction1) * 100;


			Ave = ((yearOfDeposit + yearOfDeposit1) / 2);
			Ave1 = ((yearOfExpenses + yearOfExpenses1) / 2);
			Ave2 = ((yearOfNetTransaction + yearOfNetTransaction1) / 2);
			
			cout << "-------------------------------------------------- " << YELLOW  "Years: " << a << "-" << b << RESET << " -----------------------------------------------------------";
			cout << "\n\nYear: " << GREEN << a << RESET;
			cout << "\nDeposit:                " << CYAN << Deposit1 << RESET << " "; printf("%.2f", yearOfDeposit);
			cout << "\nExpenses:               " << CYAN << Expenses1 << RESET << " "; printf("%.2f", yearOfExpenses);
			if (yearOfNetTransaction < 0)
			{
				cout << "\nNet transaction amount: " << RED << NetTrans1 << RESET << "  "; printf("%.2f", yearOfNetTransaction);
			}
			else
			{
				cout << "\nNet transaction amount: " << CYAN << NetTrans1 << RESET << "  "; printf("%.2f", yearOfNetTransaction);
			}
			cout << "\n\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®deposit¡¯" << RESET << " means" << GREEN << " RM " << YOD << RESET;
			cout << "\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®expenses¡¯" << RESET << " means" << GREEN << " RM " << YOE << RESET;
			if (yearOfNetTransaction < 0)
			{
				cout << "\nOne" << RED << " '*' " << RESET << "for " << BLUE << "¡®net transaction amount¡¯" << RESET << " means negative value" << GREEN << " RM " << YOB << RESET;
			}
			else
			{
				cout << "\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®net transaction amount¡¯" << RESET << " means" << GREEN << " RM " << YOB << RESET;
			}
			cout << endl;

			cout << "\n";
			cout << "\nYear: " << GREEN << b << RESET;
			cout << "\nDeposit:                " << CYAN << Deposit2 << RESET << " "; printf("%.2f", yearOfDeposit1);
			cout << "\nExpenses:               " << CYAN << Expenses2 << RESET << " "; printf("%.2f", yearOfExpenses1);
			if (yearOfNetTransaction1 < 0)
			{
				cout << "\nNet transaction amount: " << RED << NetTrans2 << RESET << "  "; printf("%.2f", yearOfNetTransaction1);
			}
			else
			{
				cout << "\nNet transaction amount: " << CYAN << NetTrans2 << RESET << "  "; printf("%.2f", yearOfNetTransaction1);
			}
			cout << "\n\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®deposit¡¯" << RESET << " means" << GREEN << " RM " << YOD1 << RESET;
			cout << "\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®expenses¡¯" << RESET << " means" << GREEN << " RM " << YOE1 << RESET;
			if (yearOfNetTransaction1 < 0)
			{
				cout << "\nOne" << RED << " '*' " << RESET << "for " << BLUE << "¡®net transaction amount¡¯" << RESET << " means negative value" << GREEN << " RM " << YOB1 << RESET;
			}
			else
			{
				cout << "\nOne" << CYAN << " '*' " << RESET << "for " << BLUE << "¡®net transaction amount¡¯" << RESET << " means" << GREEN << " RM " << YOB1 << RESET;
			}
			cout << endl;
			cout << "\n_______________________________________________________________________________________________";
			cout << "\n\t\t\t\t\tReport";
			cout << "\n_______________________________________________________________________________________________";
			cout << "\nPercentage of deposit in " << a << ": "; printf("%.2f", PDPT1); cout << "%";
			cout << "\nPercentage of expenses in " << a << ": "; printf("%.2f", PEPS1); cout << "%";
			
			cout << "\n\nPercentage of deposit in " << b << ": "; printf("%.2f", PDPT2); cout << "%";
			cout << "\nPercentage of expenses in " << b << ": "; printf("%.2f", PEPS2); cout << "%";
			
			cout << "\n\nAverage of deposit for both years: ";  printf("%.2f", Ave);
			cout << "\nAverage of expenses for both years: ";  printf("%.2f", Ave1);
			cout << "\nAverage of net transaction amount for both years: ";  printf("%.2f", Ave2);


			if (yearOfDeposit > yearOfDeposit1)
			{
				cout << endl << endl;
				cout << a << " has higher deposit than " << b;
				cout << "\nPercentage of deposit in " << a << " is "; printf("%.2f", PBTY); cout << "%";
				cout << "\nPercentage of deposit in " << b << " is "; printf("%.2f", PBTY1); cout << "%";
				cout << "\nPercentage of deposit in " << a << " more than " << b << " is "; printf("%.2f", PDBTY); cout << "%";
			}
			else
			{
				cout << endl << endl;
				cout << b << " has higher deposit than " << a;
				cout << "\nPercentage of deposit in " << a << " is "; printf("%.2f", PBTY); cout << "%";
				cout << "\nPercentage of deposit in " << b << " is "; printf("%.2f", PBTY1); cout << "%";
				cout << "\nPercentage of deposit in " << b << " more than " << a << " is "; printf("%.2f", PDBTY1); cout << "%";
			}
			if (yearOfExpenses > yearOfExpenses1)
			{
				cout << endl << endl;
				cout << a << " has higher expenses than " << b;
				cout << "\nPercentage of expenses in " << a << " is "; printf("%.2f", PBTY2); cout << "%";
				cout << "\nPercentage of expenses in " << b << " is "; printf("%.2f", PBTY3); cout << "%";
				cout << "\nPercentage of expenses in " << a << " more than " << b << " is "; printf("%.2f", PDBTY2); cout << "%";
			}
			else
			{
				cout << endl << endl;
				cout << b << " has higher expenses than " << a;
				cout << "\nPercentage of expenses in " << a << " is "; printf("%.2f", PBTY2); cout << "%";
				cout << "\nPercentage of expenses in " << b << " is "; printf("%.2f", PBTY3); cout << "%";
				cout << "\nPercentage of expenses in " << b << " more than " << a << " is "; printf("%.2f", PDBTY3); cout << "%";
			}
			if (yearOfNetTransaction > yearOfNetTransaction1)
			{
				cout << endl << endl;
				cout << a << " has higher net transaction amount than " << b;
				cout << "\nPercentage of net transaction amount in " << a << " is "; printf("%.2f", PBTY4); cout << "%";
				cout << "\nPercentage of net transaction amount in " << b << " is "; printf("%.2f", PBTY5); cout << "%";
				cout << "\nPercentage of net transaction amount in " << a << " more than " << b << " is "; printf("%.2f", PDBTY4); cout << "%";
			}
			else
			{
				cout << endl << endl;
				cout << b << " has higher net transaction amount than " << a;
				cout << "\nPercentage of net transaction amount in " << a << " is "; printf("%.2f", PBTY4); cout << "%";
				cout << "\nPercentage of net transaction amount in " << b << " is "; printf("%.2f", PBTY5); cout << "%";
				cout << "\nPercentage of net transaction amount in " << b << " more than " << a << " is "; printf("%.2f", PDBTY5); cout << "%";
			}

			_getch();
			break;
		case 3:
			return;
			break;
		default:
			break;
		}
	}
}