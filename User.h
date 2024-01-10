#ifndef USER_H
#define USER_H
#include <string>

class User
{
public://access specifier

	std::string UserId,first_name,last_name, password, email,phone_number; // text attributes

	User(); // constructor method
	User(std::string UserId, std::string first_name, std::string last_name, std::string password, std::string email, std::string phone_number);

	bool login();
	void insert();
	void update();

	~User(); //destructor method
};
#endif