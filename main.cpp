#include <iostream>
using namespace std;
#include "wg/sqlite/database.h"
using namespace wg::sqlite;

int main()
{
	Database *db = new Database();

    db->create("users", [](CreateTransaction* schema)
    {
        schema->integer("id")->increments()->primary();
        schema->varchar("username")->required();
        schema->varchar("password")->required();
		schema->callback([](string arg)
		{
			cout << "completed creation (arg: " << arg << ")" << endl;
		});
	})->gaurd();

	db->insert("users", [](InsertTransaction* table)
	{
		table->insert("username", "user1");
		table->insert("password", "password1");
		table->callback([](int arg)
		{
			cout << "completed insertion (arg: " << arg << ")" << endl;
		});
	});

	db->insert("users", [](InsertTransaction* table)
	{
		table->insert("username", 5);
		table->insert("password", "password2");
		// no callback because insert already has a callback defined (it's not needed)
	});

	db->update("users", [](UpdateTransaction* table)
	{
		table->set("username", "username_1");
		table->set("password", "password_1");
		table->where("id", "2");
		table->andWhere("username", "5");

		table->callback([](int arg)
		{
			cout << "completed update (arg: " << arg << ")" << endl;
		});
	});

	db->remove("users", [](DeleteTransaction* table)
	{
		table->where("id", "1");
	});

	db->query([](SelectTransaction* table)
	{
		table->from("users");
		table->select("username");
		table->callback([]()
		{
			cout << "completed selection transaction!" << endl;
		});
	});
	
	try
	{
		db->commit();
	}
	catch (std::bad_function_call &ex) // current callback model isn't perfect, but neither are you.
	{
		cout << "exception: " << ex.what() << endl;
	}

	delete db; // call destructor
	return 0;
}
