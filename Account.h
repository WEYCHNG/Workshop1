#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector> 
#include "DBConnection.h" 

using namespace std;
class Account
{
public:
	int AccountID;
	double budget_amount, balance;
	string UserID,account_name,start_date,end_date;
	
	Account(); // constructor method
	Account(sql::ResultSet* data);


	static vector<Account> findAccount(string userid,string sortColumn, bool ascending);
	static vector<Account> selectAccount(string userid);
	//static Account findAccount(int AccountID);
	bool confirmtoEdit();
	void addAccount();
	void update();
	void updateAfterTrans();
	void removeAccount();
	void getAccount(string UserId,string account_name);
	double totalAmount();
	double chgeByTrans();

	~Account();
};

#endif




