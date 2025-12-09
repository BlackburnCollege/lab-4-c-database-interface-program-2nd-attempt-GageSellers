#include <iostream>
#include "report.h"
#include "sqlite3.h"
#include <string>

using namespace std;

int main()
{
	Database db("airsoft_inventory.db");
	int choice;

	do
	{
		cout << "\n1. Create Database\n";
		cout << "2. Import Data\n";
		cout << "3. Perform Simple SELECT Statement\n";
		cout << "4. Perform Complex SELECT Statement\n";
		cout << "5. Perform User-Defined SELECT Statement\n";
		cout << "6. Exit\n";
		cout << "Enter your Choice: ";
		cin >> choice;
		cin.ignore();

		if (choice == 1)
		{
			db.createDatabase();
		}

		else if (choice == 2)
		{
			db.importData();
			cout << "Data imported.\n";
		}

		else if (choice == 3)
		{
			db.simpleSelect();
		}

		else if (choice == 4)
		{
			db.complexSelect();
		}

		else if (choice == 5)
		{
			db.userDefinedSelect();
		}

		else if (choice == 6)
		{
			cout << "Exiting Program.\n";
		}

		else
		{
			cout << "Invalid choice.";
		}
	} while (choice != 6);
}

