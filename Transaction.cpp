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


vector<Transaction> findTrans(int AccountID,string sortColumn, bool ascending)
{
	string query = "SELECT account_name,transaction_type,transaction_amount,newbalance,transaction_date, FROM `account`,'transaction' "
		"WHERE account.AccountID = transaction.AccountID=? "
		"ORDER BY" + sortColumn;
	if (ascending) {
		query += " ASC";
	}
	else
	{
		query += " DESC";
	}

	DBConnection db;
	db.prepareStatement(query);
	db.stmt->setInt(1, AccountID);

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