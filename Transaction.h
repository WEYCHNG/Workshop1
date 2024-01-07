#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector> 
#include "DBConnection.h" 
using namespace std;

class Transaction
{
public:
	int AccountID,TransactionID;
	double transaction_amount,newbalance;
	string description, category, transaction_type, transaction_date;

	Transaction();
	Transaction(sql::ResultSet* data);

	static vector<Transaction> findTransaction(string UserID, string sortColumn, bool ascending);
	void addTrans();
	bool confirmToUpdate(int TransactionID);//Edit Transaction
	void updateTrans(int TransactionID);//Update transaction 
	void deleteTrans(int transactionID);//remove transaction //if delete transaction,balance and budget will reoriginal.
	static int totalTimesOfTrans(string UserID);
	static double transAmount(string UserID);
	static double totalTransactionAmount(string UserID);
	static double netTrans(string UserID);
	static double totalDeposit(string UserID, int x, int y);
    static double totalExpenses(string UserID, int x, int y);
	static double totalDepositInYear(string UserID, int a);
	static double totalExpensesInYear(string UserID, int a);

	~Transaction();
};

#endif

