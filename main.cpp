#include <time.h>
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
	})->gaurd();

	db->insert("users", [](InsertTransaction* table)
	{
		table->insert("username", "user1");
		table->insert("password", "password1");
	});

	db->insert("users", [](InsertTransaction* table)
	{
		table->insert("username", 5);
		table->insert("password", "password2");
	});

	db->update("users", [](UpdateTransaction* table)
	{
		table->set("username", "username_1");
		table->set("password", "password_1");
		table->where("id", "1");
		table->andWhere("username", "user1");
	});

	db->query([](SelectTransaction* table)
	{
		table->from("users");
		table->select("username");
	});
	db->commit();
	return 0;
}
