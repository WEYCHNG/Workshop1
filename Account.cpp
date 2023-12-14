#include "Account.h"

using namespace std;

Account::Account()
{
	account_name = "";
	budget_amount = 0;
	balance = 0;
	start_date="";
	end_date="";
}


void Account::addAccount()
{
	DBConnection db;
	
	db.prepareStatement("Insert into account (UserID,account_name,budget_amount,balance,start_date,end_date) VALUES (?,?,?,?,?,?)");
	db.stmt->setString(1, UserID);
	db.stmt->setString(2, account_name);
	db.stmt->setDouble(3, budget_amount);
	db.stmt->setDouble(4, balance);
	db.stmt->setString(5, start_date);
	db.stmt->setString(6, end_date);
	db.QueryStatement();
	db.~DBConnection();

}

void Account::update() 
{
	DBConnection db;
	db.prepareStatement("UPDATE account SET budget_amount=?,balance=?, start_date=?, end_date=? WHERE UserID=? AND account_name=?");
	db.stmt->setDouble(1, budget_amount);
	db.stmt->setDouble(2, balance);
	db.stmt->setString(3, start_date);
	db.stmt->setString(4, end_date);
	db.stmt->setString(5, UserID);
	db.stmt->setString(6, account_name);
	db.QueryStatement();
	db.~DBConnection();

}

void Account::updateAfterTrans()
{
	DBConnection db;
	db.prepareStatement("UPDATE account SET budget_amount=?,balance=? WHERE UserID=? AND account_name=?");
	db.stmt->setDouble(1, budget_amount);
	db.stmt->setDouble(2, balance);
	db.stmt->setString(3, UserID);
	db.stmt->setString(4, account_name);
	db.QueryStatement();
	db.~DBConnection();

}

void Account::removeAccount() {
	DBConnection db;
	db.prepareStatement("DELETE FROM account WHERE UserID=? AND account_name=?");
	db.stmt->setString(1, UserID);
	db.stmt->setString(2, account_name);
	db.QueryStatement();
	db.~DBConnection();
}

Account::Account(sql::ResultSet* data)//retrieve data from database
{
	account_name = data->getString("account_name");
	budget_amount = data->getDouble("budget_amount");
	balance = data->getDouble("balance");
	start_date = data->getString("start_date");
	end_date = data->getString("end_date");
}


vector<Account> Account::findAccount(string userid,string sortColumn,bool ascending) 
{
	string query = "SELECT account_name,budget_amount,balance,start_date,end_date FROM `account` WHERE UserID=?"
		" ORDER BY "+sortColumn;

	if (ascending) {
		query += " ASC";
	}
	else
	{
		query += " DESC";
	}

	DBConnection db;
	db.prepareStatement(query);
	db.stmt->setString(1,userid);
	
	vector<Account> accounts;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) 
		{
			Account tmpAccount(db.res);
			accounts.push_back(tmpAccount);
		}
	}
	db.~DBConnection();
	return accounts;
}

vector<Account> Account::selectAccount(string userid) {
	string query = "SELECT account_name,budget_amount,balance,start_date,end_date FROM `account` WHERE UserID=? ";

	DBConnection db;
	db.prepareStatement(query);
	db.stmt->setString(1, userid);

	vector<Account> Acc;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) 
		{
			Account tmpAccount(db.res);
			Acc.push_back(tmpAccount);
		}
	}
	db.~DBConnection();
	return Acc;
}

double Account::totalAmount()
{
	DBConnection db;
	db.prepareStatement("SELECT SUM(balance) as balance FROM account WHERE UserID=? ");
	db.stmt->setString(1, UserID);
	db.QueryResult();
	if (db.res->rowsCount() == 1)
	{
		while (db.res->next())
		{
			balance = db.res->getDouble("balance");
		}
		db.~DBConnection();
		return balance;
	}
	else 
	{
		db.~DBConnection();
	}
}

bool Account::confirmtoEdit(string UserId)
{
	DBConnection db;
	db.prepareStatement("SELECT account_name,budget_amount,balance,start_date,end_date FROM account WHERE UserID=? AND account_name=?");
	db.stmt->setString(1, UserID);
	db.stmt->setString(2, account_name);
	db.QueryResult();
	if (db.res->rowsCount() == 1)
	{
		while (db.res->next()) {
			account_name = db.res->getString("account_name");
			balance = db.res->getDouble("balance");
			budget_amount = db.res->getDouble("budget_amount");
			start_date = db.res->getString("start_date");
			end_date = db.res->getString("end_date");
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

double Account::chgeByTrans()
{
	if (AccountID == 0)
	{
		return 0;
	}
	else
	{
		double totalExpenses = 0;
		double totalDeposit = 0;
		
		DBConnection db;
		db.prepareStatement("SELECT SUM(transaction_amount) as transaction_amount,transaction_type FROM transaction GROUP BY transaction_type WHERE AccountID=?");
		db.stmt->setInt(1, AccountID);
		db.QueryResult();
		string type = db.res->getString("transaction_type");
		if (db.res->rowsCount() >= 1)
		{
			while (db.res->next())
			{
				if (type == "Deposit")
				{
					totalDeposit = db.res->getDouble("transaction_amount");
				}
				else if (type == "Expenses")
				{
					totalExpenses= db.res->getDouble("transaction_amount");
				}
			}
			db.~DBConnection();
			return totalDeposit-totalExpenses;
		}
	}
}

void Account::getAccount(string UserID,string account_name)
{
	DBConnection db;
	db.prepareStatement("SELECT AccountID,account_name,budget_amount,balance FROM account WHERE UserID=? AND account_name=?");
	db.stmt->setString(1, UserID);
	db.stmt->setString(2, account_name);
	db.QueryResult();
	if (db.res->rowsCount() > 0)
	{
		while (db.res->next()) {
			account_name = db.res->getString("account_name");
			AccountID = db.res->getInt("AccountID");
			balance = db.res->getDouble("balance");
			budget_amount = db.res->getDouble("budget_amount");
		}
		db.~DBConnection();
	}
	else
	{
		db.~DBConnection();
	}
}

void Account::getBlcBdg(string UserId,int AccountID)
{
	DBConnection db;
	db.prepareStatement("SELECT account_name,budget_amount,balance FROM account WHERE UserID=? AND AccounID=?");
	db.stmt->setString(1, UserID);
	db.stmt->setInt(2, AccountID);
	db.QueryResult();
	if (db.res->rowsCount() > 0)
	{
		while (db.res->next()) {
			balance = db.res->getDouble("balance");
			budget_amount = db.res->getDouble("budget_amount");
		}
		db.~DBConnection();
	}
	else
	{
		db.~DBConnection();
	}
}



Account::~Account() {}