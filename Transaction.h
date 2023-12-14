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
	bool confirmToUpdate(int TransactionID);
	//bool editTrans();//Just confirm to edit transaction (using SQL only)
	//void updateTrans();//Update transaction //update transaction will be change follow by what are changes.
	//void deleteTrans();//remove transaction //if delete transaction,balance and budget will reoriginal.
	
	~Transaction();
};

#endif

