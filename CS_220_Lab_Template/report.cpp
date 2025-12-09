#include <iostream>
#include "report.h"

Database::Database(const std::string& name)
{
	sqlite3_open(name.c_str(), &db);

}

Database::~Database()
{
	if (db) sqlite3_close(db);

}

bool Database::doThing(const std::string& query)
{
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}
	return true;
}

void Database::results(const std::string& query)
{
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	int cols = sqlite3_column_count(stmt);
	for (int i = 0; i < cols; ++i)
	{
		std::cout << sqlite3_column_name(stmt, i) << "\t";
	}
	std::cout << std::endl;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		for (int i = 0; i < cols; ++i)
		{
			const unsigned char* text = sqlite3_column_text(stmt, i);
			std::cout << (text ? reinterpret_cast<const char*>(text) : "NULL") << "\t";
		}
		std::cout << std::endl;
	}
	sqlite3_finalize(stmt);
}

void Database::createDatabase()
{
	std::vector<std::string> queries =
	{
		"CREATE TABLE IF NOT EXISTS Manufacturer (ManufacturerID INTEGER PRIMARY KEY AUTOINCREMENT, Brand TEXT NOT NULL UNIQUE);",
		"CREATE TABLE IF NOT EXISTS MOSFET (MOSFETID INTEGER PRIMARY KEY AUTOINCREMENT, Brand TEXT NOT NULL, Model TEXT NOT NULL, UNIQUE(Brand, Model));",
		"CREATE TABLE IF NOT EXISTS Optics (OpticsID INTEGER PRIMARY KEY AUTOINCREMENT, Type TEXT NOT NULL, Brand TEXT NOT NULL, UNIQUE(Type, Brand));",
		"CREATE TABLE IF NOT EXISTS Attachment (AttachmentID INTEGER PRIMARY KEY AUTOINCREMENT, Type TEXT NOT NULL, Brand TEXT NOT NULL, UNIQUE(Type, Brand));",
		"CREATE TABLE IF NOT EXISTS Weapon (WeaponID TEXT PRIMARY KEY, WeaponClass TEXT NOT NULL, FiringMethod TEXT NOT NULL, ManufacturerID INTEGER NOT NULL, Model TEXT NOT NULL, Version TEXT, Length REAL CHECK(Length > 0), Width REAL CHECK(Width > 0), Height REAL CHECK(Height > 0), Weight REAL CHECK(Weight > 0), Materials TEXT, PowerSource TEXT, MOSFETID INTEGER, Velocity INTEGER CHECK(Velocity >= 0), Range INTEGER CHECK(Range >= 0), MagCapacity INTEGER CHECK(MagCapacity >= 0), BB_Diameter REAL CHECK(BB_Diameter > 0), BB_Weight REAL CHECK(BB_Weight > 0), HopUp_Brand TEXT, HopUp_Size TEXT, Barrel_Brand TEXT, Barrel_Size TEXT, FOREIGN KEY(ManufacturerID) REFERENCES Manufacturer(ManufacturerID), FOREIGN KEY(MOSFETID) REFERENCES MOSFET(MOSFETID));",
		"CREATE TABLE IF NOT EXISTS Weapon_Optics (WeaponID TEXT NOT NULL, OpticsID INTEGER NOT NULL, PRIMARY KEY(WeaponID, OpticsID), FOREIGN KEY(WeaponID) REFERENCES Weapon(WeaponID), FOREIGN KEY(OpticsID) REFERENCES Optics(OpticsID));",
		"CREATE TABLE IF NOT EXISTS Weapon_Attachment (WeaponID TEXT NOT NULL, AttachmentID INTEGER NOT NULL, PRIMARY KEY(WeaponID, AttachmentID), FOREIGN KEY(WeaponID) REFERENCES Weapon(WeaponID), FOREIGN KEY(AttachmentID) REFERENCES Attachment(AttachmentID));",
		"CREATE TABLE IF NOT EXISTS Ammo (AmmoID INTEGER PRIMARY KEY AUTOINCREMENT, Type TEXT NOT NULL UNIQUE, Quantity INTEGER NOT NULL CHECK(Quantity >= 0));",
		"CREATE TABLE IF NOT EXISTS Throwable (ThrowableID INTEGER PRIMARY KEY AUTOINCREMENT, Type TEXT NOT NULL UNIQUE, Quantity INTEGER NOT NULL CHECK(Quantity >= 0));",
		"CREATE TABLE IF NOT EXISTS LargeEquipment (LargeEquipID INTEGER PRIMARY KEY AUTOINCREMENT, Type TEXT NOT NULL UNIQUE, Quantity INTEGER NOT NULL CHECK(Quantity >= 0));",
		"CREATE TABLE IF NOT EXISTS Maintenance (MaintenanceID INTEGER PRIMARY KEY AUTOINCREMENT, WeaponID TEXT NOT NULL, Date TEXT NOT NULL, Service TEXT NOT NULL, WearIndicator TEXT, Repair TEXT, FOREIGN KEY(WeaponID) REFERENCES Weapon(WeaponID));",
		"CREATE TABLE IF NOT EXISTS Outfit (OutfitID INTEGER PRIMARY KEY AUTOINCREMENT, Camo TEXT NOT NULL, Brand TEXT NOT NULL, Size TEXT NOT NULL, Quantity INTEGER NOT NULL CHECK(Quantity >= 0), UNIQUE(Camo, Brand, Size));",
		"CREATE TABLE IF NOT EXISTS MainRig (RigID INTEGER PRIMARY KEY AUTOINCREMENT, Type TEXT NOT NULL, Brand TEXT NOT NULL, Size TEXT NOT NULL, Quantity INTEGER NOT NULL CHECK(Quantity >= 0), UNIQUE(Type, Brand, Size));",
		"CREATE TABLE IF NOT EXISTS MagPouch (PouchID INTEGER PRIMARY KEY AUTOINCREMENT, Brand TEXT NOT NULL, CaliberSize TEXT NOT NULL, Quantity INTEGER NOT NULL CHECK(Quantity >= 0), UNIQUE(Brand, CaliberSize));",
		"CREATE TABLE IF NOT EXISTS AccessoryPouch (AccPouchID INTEGER PRIMARY KEY AUTOINCREMENT, Type TEXT NOT NULL, Brand TEXT NOT NULL, Size TEXT NOT NULL, Quantity INTEGER NOT NULL CHECK(Quantity >= 0), UNIQUE(Type, Brand, Size));",
		"CREATE TABLE IF NOT EXISTS Boot (BootID INTEGER PRIMARY KEY AUTOINCREMENT, Color TEXT NOT NULL, Brand TEXT NOT NULL, Size TEXT NOT NULL, Quantity INTEGER NOT NULL CHECK(Quantity >= 0), UNIQUE(Color, Brand, Size));",
		"CREATE TABLE IF NOT EXISTS Helmet (HelmetID INTEGER PRIMARY KEY AUTOINCREMENT, Camo TEXT NOT NULL, Brand TEXT NOT NULL, Size TEXT NOT NULL, Quantity INTEGER NOT NULL CHECK(Quantity >= 0), UNIQUE(Camo, Brand, Size));",
		"CREATE TABLE IF NOT EXISTS WeaponPart (PartID INTEGER PRIMARY KEY AUTOINCREMENT, Model TEXT NOT NULL, Size TEXT NOT NULL, Brand TEXT NOT NULL, Quantity INTEGER NOT NULL CHECK(Quantity >= 0), UNIQUE(Model, Size, Brand));",
		"CREATE TABLE IF NOT EXISTS GearPart (PartID INTEGER PRIMARY KEY AUTOINCREMENT, Size TEXT NOT NULL, Color TEXT NOT NULL, Brand TEXT NOT NULL, Quantity INTEGER NOT NULL CHECK(Quantity >= 0), UNIQUE(Size, Color, Brand));"

	};
	for (const auto& q : queries)
	{
		doThing(q);
	}
	
	std::cout << "Database created." << std::endl;
}

void Database::importData()
{
	std::ifstream inFile;
	std::string line;

	inFile.open("manufacturer.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string brand;
		std::getline(ss, brand, ',');
		doThing("INSERT INTO Manufacturer (Brand) VALUES ('" + brand + "');");
	}
	inFile.close();



	inFile.open("mosfet.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string brand, model;
		std::getline(ss, brand, ',');
		std::getline(ss, model, ',');
		doThing("INSERT INTO MOSFET (Brand, Model) VALUES ('" + brand + "', '" + model + "');");
	}

	inFile.open("optics.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string type, brand;
		std::getline(ss, type, ',');
		std::getline(ss, brand, ',');
		
		doThing("INSERT INTO Optics (Type, Brand) VALUES ('" + type + "', '" + brand + "');");
	}

	inFile.open("attachment.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string type, brand;
		std::getline(ss, type, ',');
		std::getline(ss, brand, ',');
		doThing("INSERT INTO Attachment (Type, Brand) VALUES ('" + type + "', '" + brand + "');");
	}

	inFile.open("weapon.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string weaponID, weaponClass, firingMethod, manufacturerID, model, version, length, width, height, weight, materials, powerSource, mosfetID, velocity, range, magCapacity, bbDiameter, bbWeight, hopUpBrand, hopUpSize, barrelBrand, barrelSize;
		std::getline(ss, weaponID, ',');
		std::getline(ss, weaponClass, ',');
		std::getline(ss, firingMethod, ',');
		std::getline(ss, manufacturerID, ',');
		std::getline(ss, model, ',');
		std::getline(ss, version, ',');
		std::getline(ss, length, ',');
		std::getline(ss, width, ',');
		std::getline(ss, height, ',');
		std::getline(ss, weight, ',');
		std::getline(ss, materials, ',');
		std::getline(ss, powerSource, ',');
		std::getline(ss, mosfetID, ',');
		std::getline(ss, velocity, ',');
		std::getline(ss, range, ',');
		std::getline(ss, magCapacity, ',');
		std::getline(ss, bbDiameter, ',');
		std::getline(ss, bbWeight, ',');
		std::getline(ss, hopUpBrand, ',');
		std::getline(ss, hopUpSize, ',');
		std::getline(ss, barrelBrand, ',');
		std::getline(ss, barrelSize, ',');
		std::string insert = "INSERT INTO Weapon (WeaponID, WeaponClass, FiringMethod, ManufacturerID, Model, Version, Length, Width, Height, Weight, Materials, PowerSource, MOSFETID, Velocity, Range, MagCapacity, BB_Diameter, BB_Weight, HopUp_Brand, HopUp_Size, Barrel_Brand, Barrel_Size) VALUES ('" + weaponID + "', '" + weaponClass + "', '" + firingMethod + "', " + manufacturerID + ", '" + model + "', '" + version + "', " + length + ", " + width + ", " + height + ", " + weight + ", '" + materials + "', '" + powerSource + "', " + mosfetID + ", " + velocity + ", " + range + ", " + magCapacity + ", " + bbDiameter + ", " + bbWeight + ", '" + hopUpBrand + "', '" + hopUpSize + "', '" + barrelBrand + "', '" + barrelSize + "');";
		doThing(insert);
	}
	inFile.close();

	inFile.open("ammo.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string type, quantity;
		std::getline(ss, type, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO Ammo (Type, Quantity) VALUES ('" + type + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("throwable.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string type, quantity;
		std::getline(ss, type, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO Throwable (Type, Quantity) VALUES ('" + type + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("maintenance.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string weaponID, date, service, wearIndicator, repair;
		std::getline(ss, weaponID, ',');
		std::getline(ss, date, ',');
		std::getline(ss, service, ',');
		std::getline(ss, wearIndicator, ',');
		std::getline(ss, repair, ',');
		doThing("INSERT INTO Maintenance (WeaponID, Date, Service, WearIndicator, Repair) VALUES ('" + weaponID + "', '" + date + "', '" + service + "', '" + wearIndicator + "', '" + repair + "');");
	}
	inFile.close();

	inFile.open("outfit.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string camo, brand, size, quantity;
		std::getline(ss, camo, ',');
		std::getline(ss, brand, ',');
		std::getline(ss, size, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO Outfit (Camo, Brand, Size, Quantity) VALUES ('" + camo + "', '" + brand + "', '" + size + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("mainrig.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string type, brand, size, quantity;
		std::getline(ss, type, ',');
		std::getline(ss, brand, ',');
		std::getline(ss, size, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO MainRig (Type, Brand, Size, Quantity) VALUES ('" + type + "', " + brand + "', " + size + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("magpouch.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string brand, caliberSize, quantity;
		std::getline(ss, brand, ',');
		std::getline(ss, caliberSize, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO MagPouch (Brand, CaliberSize, Quantity) VALUES ('" + brand + "', '" + caliberSize + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("accessorypouch.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string type, brand, size, quantity;
		std::getline(ss, type, ',');
		std::getline(ss, brand, ',');
		std::getline(ss, size, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO AccessoryPouch (Type, Brand, Size, Quantity) VALUES ('" + type + "', '" + brand + "', '" + size + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("boot.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string color, brand, size, quantity;
		std::getline(ss, color, ',');
		std::getline(ss, brand, ',');
		std::getline(ss, size, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO Boot (color, Brand, Size, Quantity) VALUES ('" + color + "', '" + brand + "', '" + size + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("helmet.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string camo, brand, size, quantity;
		std::getline(ss, camo, ',');
		std::getline(ss, brand, ',');
		std::getline(ss, size, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO Helmet (Camo, Brand, Size, Quantity) VALUES ('" + camo + "', '" + brand + "', '" + size + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("weaponpart.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string model, brand, size, quantity;
		std::getline(ss, model, ',');
		std::getline(ss, brand, ',');
		std::getline(ss, size, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO WeaponPart (model, Brand, Size, Quantity) VALUES ('" + model + "', '" + brand + "', '" + size + "', " + quantity + ");");
	}
	inFile.close();

	inFile.open("gearpart.csv");
	std::getline(inFile, line);
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string color, brand, size, quantity;
		std::getline(ss, color, ',');
		std::getline(ss, brand, ',');
		std::getline(ss, size, ',');
		std::getline(ss, quantity, ',');
		doThing("INSERT INTO GearPart (Color, Brand, Size, Quantity) VALUES ('" + color + "', '" + brand + "', '" + size + "', " + quantity + ");");
	}
	inFile.close();
}

void Database::simpleSelect()
{
	std::string query = "SELECT * FROM Manufacturer;";
	results(query);
}

void Database::complexSelect()
{
	std::string query = "SELECT Weapon.WeaponID, Weapon.Model, Manufacturer.Brand FROM Weapon JOIN Manufacturer ON Weapon.ManufacturerID = Manufacturer.ManufacturerID ORDER BY Weapon.Model;";
	results(query);

}

void Database::userDefinedSelect()
{
	std::string query;
	std::cout << "Enter the SELECT query: ";
	std::getline(std::cin, query);
	results(query);
}