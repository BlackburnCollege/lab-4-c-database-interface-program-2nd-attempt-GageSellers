#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sqlite3.h>

class Database
{


private:
	sqlite3* db;

public:
	Database(const std::string& name);
	~Database();
	bool doThing(const std::string& query);
	void results(const std::string& query);

	void createDatabase(Database& db);
	void importData(Database& db);
	void simpleSelect(Database& db);
	void complexSelect(Database& db);
	void userDefinedSelect(Database& db);

};