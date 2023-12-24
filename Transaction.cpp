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
	string query = "SELECT TransactionID,transaction_type, transaction_amount, category, description, newbalance, transaction_date FROM transaction JOIN account USING (AccountID) WHERE UserID = ? ORDER BY " + sortColumn;
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

bool Transaction::confirmToUpdate(int TransactionID)
{
	DBConnection db;
	db.prepareStatement("SELECT AccountID,category,description,transaction_type,transaction_amount,newbalance FROM transaction WHERE TransactionID = ?");
	db.stmt->setInt(1,TransactionID);
	db.QueryResult();
	if (db.res->rowsCount() == 1)
	{
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");
			category = db.res->getString("category");
			description = db.res->getString("description");
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

void Transaction::updateTrans(int TransactionID)
{
	DBConnection db;
	db.prepareStatement("UPDATE transaction SET transaction_type=?, transaction_amount=?, category=?, description=?, newbalance=? WHERE TransactionID=? ");
	db.stmt->setString(1, transaction_type);
	db.stmt->setDouble(2, transaction_amount);
	db.stmt->setString(3, category);
	db.stmt->setString(4, description);
	db.stmt->setDouble(5, newbalance);
	db.stmt->setInt(6, TransactionID);
	db.QueryStatement();
	db.~DBConnection();	
}

void Transaction::deleteTrans(int TransactionID)
{
	DBConnection db;
	db.prepareStatement("DELETE FROM transaction WHERE TransactionID=?");
	db.stmt->setInt(1, TransactionID);
	db.QueryStatement();
	db.~DBConnection();
}

int Transaction::totalTimesOfTrans(string UserId)
{
	DBConnection db;
	int totalTrans = -1;
	db.prepareStatement("SELECT COUNT(TransactionID) as totaltrans FROM transaction JOIN account USING (AccountID) "
		"WHERE UserID=? AND transaction_date >= DATE_FORMAT(NOW(), '%Y-%m-01')"
		"AND transaction_date < DATE_ADD(DATE_FORMAT(NOW(), '%Y-%m-01'), INTERVAL 1 MONTH);");
	db.stmt->setString(1, UserId);
	db.QueryResult();

	while (db.res->next())
	{
		totalTrans = db.res->getDouble("totaltrans");
	}
	db.~DBConnection();
	return totalTrans;
}

//Can check previous 7 days transaction amount
double Transaction::transAmount(string UserId)
{
	DBConnection db;
	double totalTransaction = 0;
	db.prepareStatement("SELECT SUM(transaction_amount) AS totaltransaction "
		"FROM transaction "
		"JOIN account USING(AccountID) "
		"WHERE UserID = ? "
		"AND transaction_date >= DATE_SUB(DATE(NOW()), INTERVAL 1 WEEK) "
		"AND transaction_date < DATE(NOW());");
	db.stmt->setString(1, UserId);
	db.QueryResult();

	while (db.res->next())
	{
		totalTransaction = db.res->getDouble("totaltransaction");
	}
	db.~DBConnection();
	return totalTransaction;
}

//Can check 1 month transaction amount
double Transaction::totalTransactionAmount(string UserId)
{
	DBConnection db;
	double totalTransaction = 0;
	db.prepareStatement("SELECT SUM(transaction_amount) AS totaltransaction FROM transaction JOIN account USING(AccountID)"
		"WHERE UserID = ? AND transaction_date >= DATE_FORMAT(NOW(), '%Y-%m-01')"
		"AND transaction_date < DATE_ADD(DATE_FORMAT(NOW(), '%Y-%m-01'), INTERVAL 1 MONTH); ");
	db.stmt->setString(1, UserId);
	db.QueryResult();

	while (db.res->next())
	{
		totalTransaction = db.res->getDouble("totaltransaction");
	}
	db.~DBConnection();
	return totalTransaction;
}

//Can check 1 month net transaction amount
double Transaction::netTrans(string UserId)
{
	DBConnection db;
	double netTrans = 0;
	db.prepareStatement("SELECT "
		"SUM(CASE WHEN transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS total_transfer_in, "
		"SUM(CASE WHEN transaction_type = 'Expenses' THEN transaction_amount ELSE 0 END) AS total_transfer_out, "
		"SUM(CASE WHEN transaction_type = 'Deposit' THEN transaction_amount ELSE -transaction_amount END) AS net_transfer "
		"FROM transaction "
		"JOIN account USING(AccountID) "
		"WHERE UserID = ? "
		"AND transaction_date >= DATE_FORMAT(NOW(), '%Y-%m-01') " // Start of current month
		"AND transaction_date < DATE_ADD(DATE_FORMAT(NOW(), '%Y-%m-01'), INTERVAL 1 MONTH);"); // Start of next month
	db.stmt->setString(1, UserId);
	db.QueryResult();

	while (db.res->next())
	{
		netTrans = db.res->getDouble("net_transfer");
	}
	db.~DBConnection();
	return netTrans;
}

//get total deposit
double* Transaction::totalDeposit(string UserId)
{
	DBConnection db;
	double* monthOfDeposit = new double[6] { 0, 0, 0, 0, 0, 0 };
	db.prepareStatement("SELECT DATE_FORMAT(transaction_date, '%Y-%m') AS month,"
		"SUM(CASE WHEN MONTH(transaction_date) = 10 AND transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS transfer_in_October,"
		"SUM(CASE WHEN MONTH(transaction_date) = 11 AND transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS transfer_in_November,"
		"SUM(CASE WHEN MONTH(transaction_date) = 12 AND transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS transfer_in_December,"
		"SUM(CASE WHEN MONTH(transaction_date) = 1 AND transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS transfer_in_Jan,"
		"SUM(CASE WHEN MONTH(transaction_date) = 2 AND transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS transfer_in_Feb,"
		"SUM(CASE WHEN MONTH(transaction_date) = 3 AND transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS transfer_in_Mar"
		"FROM transaction JOIN account USING(AccountID) WHERE UserID = ? AND((YEAR(transaction_date) = 2023 AND MONTH(transaction_date) BETWEEN 10 AND 12)"
		"OR(YEAR(transaction_date) = 2024 AND MONTH(transaction_date) BETWEEN 1 AND 3)) GROUP BY MONTH(transaction_date) ORDER BY MONTH(transaction_date) ASC;");
	db.stmt->setString(1, UserId);
	db.QueryResult();

	while (db.res->next())
	{
		monthOfDeposit[0] = db.res->getDouble("transafer_in_October");
		monthOfDeposit[1] = db.res->getDouble("transafer_in_November");
		monthOfDeposit[2] = db.res->getDouble("transafer_in_December");
		monthOfDeposit[3] = db.res->getDouble("transafer_in_Jan");
		monthOfDeposit[4] = db.res->getDouble("transafer_in_Feb");
		monthOfDeposit[5] = db.res->getDouble("transafer_in_Mar");
	}
	db.~DBConnection();
	return monthOfDeposit;
}

double* Transaction::totalExpenses(string UserId)
{
	DBConnection db;
	double* monthOfExpenses = new double[6] { 0, 0, 0, 0, 0, 0 };
	db.prepareStatement("SELECT DATE_FORMAT(transaction_date, '%Y-%m') AS month,"
		"SUM(CASE WHEN MONTH(transaction_date) = 10 AND transaction_type = 'Expenses' THEN transaction_amount ELSE 0 END) AS transfer_out_October,"
		"SUM(CASE WHEN MONTH(transaction_date) = 11 AND transaction_type = 'Expenses' THEN transaction_amount ELSE 0 END) AS transfer_out_November,"
		"SUM(CASE WHEN MONTH(transaction_date) = 12 AND transaction_type = 'Expenses' THEN transaction_amount ELSE 0 END) AS transfer_out_December,"
		"SUM(CASE WHEN MONTH(transaction_date) = 1 AND transaction_type = 'Expenses' THEN transaction_amount ELSE 0 END) AS transfer_out_Jan,"
		"SUM(CASE WHEN MONTH(transaction_date) = 2 AND transaction_type = 'Expenses' THEN transaction_amount ELSE 0 END) AS transfer_out_Feb,"
		"SUM(CASE WHEN MONTH(transaction_date) = 3 AND transaction_type = 'Expenses' THEN transaction_amount ELSE 0 END) AS transfer_out_Mar"
		"FROM transaction JOIN account USING(AccountID) WHERE UserID = ? AND((YEAR(transaction_date) = 2023 AND MONTH(transaction_date) BETWEEN 10 AND 12)"
		"OR(YEAR(transaction_date) = 2024 AND MONTH(transaction_date) BETWEEN 1 AND 3)) GROUP BY MONTH(transaction_date) ORDER BY MONTH(transaction_date) ASC;");
	db.stmt->setString(1, UserId);
	
	db.QueryResult();

	while (db.res->next())
	{
		monthOfExpenses[0] = db.res->getDouble("transafer_out_October");
		monthOfExpenses[1] = db.res->getDouble("transafer_out_November");
		monthOfExpenses[2] = db.res->getDouble("transafer_out_December");
		monthOfExpenses[3] = db.res->getDouble("transafer_out_Jan");
		monthOfExpenses[4] = db.res->getDouble("transafer_out_Feb");
		monthOfExpenses[5] = db.res->getDouble("transafer_out_Mar");
	}

	db.~DBConnection();
	return monthOfExpenses;
}

//compare two years

/*SELECT 
    YEAR(transaction_date) AS year,
    MONTH(transaction_date) AS month,
    SUM(CASE WHEN YEAR(transaction_date) = 2022 AND transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS transfer_in_2022,
    SUM(CASE WHEN YEAR(transaction_date) = 2023 AND transaction_type = 'Deposit' THEN transaction_amount ELSE 0 END) AS transfer_in_2023
FROM 
    transaction
JOIN 
    account USING(AccountID)
WHERE 
    UserID = "EugeneTeow"
    AND (YEAR(transaction_date) = 2022 OR YEAR(transaction_date) = 2023) -- Years to compare
GROUP BY 
    YEAR(transaction_date), MONTH(transaction_date)
ORDER BY 
    YEAR(transaction_date), MONTH(transaction_date) ASC;*/


Transaction::~Transaction() {}