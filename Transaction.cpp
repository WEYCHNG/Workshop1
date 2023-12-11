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
	db.prepareStatement("INSERT INTO transaction (AccountID,description,category,transaction_type,transaction_amount,newbalance) VALUES (?,?,?,?,?,?)");
	db.stmt->setInt(1, AccountID);
	db.stmt->setString(2, description);
	db.stmt->setString(3, category);
	db.stmt->setString(4, transaction_type);
	db.stmt->setDouble(5, transaction_amount);
	db.stmt->setDouble(6, newbalance);
	db.QueryStatement();
	AccountID = db.getGeneratedId();

}

