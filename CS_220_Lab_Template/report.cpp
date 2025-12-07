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
	sqlite3_stmt;
	sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	int cols = sqlite3_column_count(stmt);
	for (int i = 0; 1 < cols; ++i)
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

void createDatabase(Database& db)
{
	std::string queries[] =
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
	for (auto& q : queries) db.doThing(q);
	std::cout << "Database created." << std::endl;
}

void importData(Database& db)
{
	std::ifstream f("manufacturer.csv");
	std::string line;
	std::getline(f, line);
	while (std::getline(f, line))
	{
		std::stringstream ss(line);
		std::string brand;
		std::getline(ss, brand, ',');
		db.doThing("INSERT INTO Manufactuerer (Brand) VALUES ('" + brand + "');");
	}
	f.close();

}

f.open("mosfet.csv");
std::getline(f, line);
while (std::getline(f, line))
{
	std::stringstream ss(line);
	string brand, model;
	std::getline(ss, brand, ',');
	std::getline(ss, model, ',');
	db.doThing("INSERT INTO MOSFET (Brand, Model) VALUES ('" + brand + "', '" + model + "');");
}

f.open("optics.csv");
std::getline(f, line);
while (std::getline(f, line))
{
	std::stringstream ss(line);
	string type, brand;
	std::getline(ss, brand, ',');
	std::getline(ss, model, ',');
	db.doThing("INSERT INTO OPTICS (Type, Brand) VALUES ('" + type + "', '" + brand + "');");
}

f.open("attachment.csv");
std::getline(f, line);
while (std::getline(f, line))
{
	std::stringstream ss(line);
	string type, brand;
	std::getline(ss, brand, ',');
	std::getline(ss, model, ',');
	db.doThing("INSERT INTO ATTACHMENT (Type, Brand) VALUES ('" + type + "', '" + brand + "');");
}

f.open("weapon.csv");
std::getline(f, line);
while (std::getline(f, line))
{
	std::stringstream ss(line);
	std::string weaponID, weaponClass, firingMethod, manufacturerID, model, version, length, width, height, weight, materials, powerSource, mosfetID, velocity, range, magCapacity, bbDiameter, bbWeight, hopUpBrand, HopUpSize, barrelBrand, barrelSize;
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
	db.executeQuery(insert);
}
f.close();

f.open("ammo.csv");
std::getline(f, line);



