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
		table->insert("username", "user_1");
		table->insert("password", "pwd_1");
		table->callback([](int)
		{
			cout << "insert is called" << endl;
		});
	});

	db->update("users", [](UpdateTransaction* table)
	{
		table->set("username", "__user1__");
		table->where("username", "user_1");
		table->callback([](int)
		{
			cout << "update is called" << endl;
		});
	});

	db->query([](SelectTransaction* table)
	{
		table->select("username");
		table->from("users");
		table->where("id", "1");
		table->callback([]()
		{
			cout << "select is called!" << endl;
		});
	});
	
	//db->commit();
	try { db->commit(); }
	catch (QueryException &ex) { cout << "exception thrown: " << ex.what() << endl; }

	cin.get();
	return 0;
}
