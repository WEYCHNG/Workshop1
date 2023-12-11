#include "Transaction.h"
#include<string>
using namespace std;

Transaction::Transaction()
{
	AccountID = 0;
	TransactionID = 0;
	description = "";
	category = "";
	transaction_type = "";
	transaction_amount = 0;
	newbalance = 0;
}

//Get data from database
Transaction::Transaction(sql::ResultSet* data)
{
	description = data->getString("description");
	category = data->getString("category");
	transaction_type = data->getString("transaction_type");
	newbalance = data->getDouble("transaction_amount");
	transaction_date = data->getString("transaction_date");
	newbalance = data->getDouble("newbalance");
}

//Add transaction
void Transaction::addTrans()
{
	DBConnection db;
	db.prepareStatement("INSERT INTO transaction (TransactionID,AccountID,description,category,transaction_type,transaction_amount,newbalance) VALUES (?,?,?,?,?,?)");
	db.stmt->setInt(1, TransactionID);
	db.stmt->setInt(2, AccountID);
	db.stmt->setString(3, description);
	db.stmt->setString(4, category);
	db.stmt->setString(5, transaction_type);
	db.stmt->setDouble(6, transaction_amount);
	db.stmt->setDouble(7, newbalance);
	db.QueryStatement();
	AccountID = db.getGeneratedId();

}

