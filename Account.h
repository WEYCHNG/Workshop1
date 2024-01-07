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
	double budget_amount,budget_remainder,balance;
	string UserID,account_name,start_date,end_date;
	
	Account(); // constructor method
	Account(sql::ResultSet* data);


	static vector<Account> findAccount(string userid,string sortColumn, bool ascending);
	static vector<Account> selectAccount(string userid);
	bool confirmtoEdit(string UserId);
	void addAccount();
	void update();
	void updateAfterTrans();
	void removeAccount(string UserID);
	void deleteAllTransaction(string UserID, string account_name);
	void getAccount(string UserId,string account_name);
	void getBlcBdg(string UserId,int AccountID);
	double totalAmount();
	//double chgeByTrans(string UserID);
	void getStartDateAndEndDate(int AccountID);
	
	~Account();
};

#endif




