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
	TransactionID = data->getInt("TransactionID");
	description = data->getString("description");
	category = data->getString("category");
	transaction_type = data->getString("transaction_type");
	transaction_amount = data->getDouble("transaction_amount");
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

vector<Transaction> Transaction::findTransaction(string UserID,string sortColumn, bool ascending)
{
	string query = "SELECT TransactionID, transaction_type, transaction_amount, category, description, newbalance, transaction_date FROM transaction JOIN account USING (AccountID) WHERE UserID = ? ORDER BY " + sortColumn;
	if (ascending) {
		query += " ASC";
	}
	else
	{
		query += " DESC";
	}

	DBConnection db;
	db.prepareStatement(query);
	db.stmt->setString(1,UserID);

	vector<Transaction> trans;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next())
		{
			Transaction tmpTrans(db.res);
			trans.push_back(tmpTrans);
		}
	}
	db.~DBConnection();
	return trans;
}

bool Transaction::confirmToEdit(int transactionId)
{
	DBConnection db;
	db.prepareStatement("SELECT AccountID,description,category,transaction_type,transaction_amount,newbalance FROM transaction WHERE TransactionID=?");
	db.stmt->setInt(1, TransactionID);
	db.QueryResult();
	if (db.res->rowsCount() == 1)
	{
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");
			description = db.res->getString("description");
			category = db.res->getString("category");
			transaction_type = db.res->getString("transaction_type");
			transaction_amount = db.res->getDouble("transaction_amount");
			newbalance = db.res->getDouble("newbalance");
		}
		db.~DBConnection();
		return true;
	}
	else
	{
		db.~DBConnection();
		return false;
	}
}

Transaction::~Transaction() {}