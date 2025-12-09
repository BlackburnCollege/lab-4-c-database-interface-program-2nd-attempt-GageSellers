#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "sqlite3.h"

class Database
{


private:
	sqlite3* db;

public:
	Database(const std::string& name);
	~Database();
	bool doThing(const std::string& query);
	void results(const std::string& query);

	void createDatabase();
	void importData();
	void simpleSelect();
	void complexSelect();
	void userDefinedSelect();

};