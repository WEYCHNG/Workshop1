#include "User.h"
#include "DBConnection.h" 
using namespace std;

User::User() 
{
	//initialize
	UserId = "";
	last_name = "";
	first_name = "";
	password = "";
	email = "";
	phone_number = "";
}

User::User(std::string UserId, std::string first_name,std::string last_name, std::string password, std::string email,std::string phone_number) 
{
	this->UserId = UserId;
	this->first_name = first_name;
	this->last_name = last_name;
	this->password = password;
	this->email = email;
	this->phone_number = phone_number;
}

bool User::login() {

	DBConnection db;
	db.prepareStatement("SELECT * FROM user WHERE UserId=? AND password=?");
	db.stmt->setString(1, UserId);
	db.stmt->setString(2, password);
	db.QueryResult();
	if (db.res->rowsCount() == 1) 
	{
		while (db.res->next()) {
			UserId = db.res->getString("UserID");
			first_name = db.res->getString("first_name");
			last_name = db.res->getString("last_name");
			password = db.res->getString("password");
			email = db.res->getString("email");
			phone_number = db.res->getString("phone_number");
		}
		db.~DBConnection();
		return true;
	}
	else {

		db.~DBConnection();
		return false;
	}
}

void User::insert() {
	//register part
	DBConnection db;//instantiate
	//prepare statement
	db.prepareStatement("Insert into user (UserId,first_name,last_name,password,email,phone_number) VALUES (?,?,?,?,?,?)");
	db.stmt->setString(1, UserId);
	db.stmt->setString(2, first_name);
	db.stmt->setString(3, last_name);
	db.stmt->setString(4, password);
	db.stmt->setString(5, email);
	db.stmt->setString(6, phone_number);
	db.QueryStatement();
	db.~DBConnection();//close database
}

void User::update() {

	DBConnection db;
	db.prepareStatement("UPDATE user SET first_name=?, last_name=?,password=?, email=?, phone_number=? WHERE UserId=?");
	db.stmt->setString(1, first_name);
	db.stmt->setString(2, last_name);
	db.stmt->setString(3, password);
	db.stmt->setString(4, email);
	db.stmt->setString(5, phone_number);
	db.stmt->setString(6, UserId);
	db.QueryStatement();
	db.~DBConnection();

}


User::~User() {}
