#include <stdlib.h>
#include <iostream>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include <jdbc/mysql_connection.h>

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

using namespace std;

class sqldb {

private:
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::ResultSet* res;
	sql::PreparedStatement* pstmt;



public:
	sqldb() {
		//try {
		 /* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "");
		/* Connect to the MySQL test database */
		con->setSchema("Game");
		//}
	   /* catch (sql::SQLException& e) {
		 cout << "# ERR: SQLException in " << __FILE__;
		 cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		 cout << "# ERR: " << e.what();
		 cout << " (MySQL error code: " << e.getErrorCode();
		 cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}*/
	}
	int getDamage(string attack_type, bool defense) {

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "");
		/* Connect to the MySQL test database */
		con->setSchema("Game");

		pstmt = con->prepareStatement("SELECT Damage from Attack_details where Player_ID='1' and Attack_Type = ?");
		pstmt->setString(1, attack_type);
		res = pstmt->executeQuery();
		int damage;
		if (res->next())
		{
			damage = res->getInt(1);
			if (defense)
			{
				damage = damage - 5;
			}
		}

		return damage;


	}
	int getDamage2(string attack_type, bool defense) {

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "");
		/* Connect to the MySQL test database */
		con->setSchema("Game");

		pstmt = con->prepareStatement("SELECT Damage from Attack_details where Player_ID='2' and Attack_Type = ?");
		pstmt->setString(1, attack_type);
		res = pstmt->executeQuery();
		int damage;
		if (res->next())
		{
			damage = res->getInt(1);
			if (defense)
			{
				damage = damage - 5;
			}
		}

		return damage;


	}


	~sqldb() {
		delete res;
		delete stmt;
		delete con;
		delete pstmt;
	}


}; #pragma once
