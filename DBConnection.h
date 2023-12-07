#ifndef DBConnection_H
#define DBConnection_H
#include <mysql/jdbc.h>
#include <string>

class DBConnection
{
public:
	//Constructor
	DBConnection();

	//Destructor
	~DBConnection();

	//PrepareStatement
	void prepareStatement(std::string query);

	//A pointer to store statement
	sql::PreparedStatement* stmt;//store

	//A pointer to store the result set
	sql::ResultSet* res;

	// For Insert, Update, Delete query = NO RESULTS
	void QueryStatement(); 

	// For Select query = RESULTS
	void QueryResult(); 

	// get id from database
	int getGeneratedId();
	

private:

	//Cannot change!!
	sql::Connection* connection;
};
#endif
