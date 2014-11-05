SQLite-wrapper: SQLite made simple  
===

### what is it
SQLite-wrapper provides a fluent and intuitive OOP wrapper around the low level C library of SQLite. it's syntax was loosely based on the popular PHP framework [laravel](http://laravel.com)'s Eloquent.

### how to use it
the API is *transaction based*, this means that everything you do will be stored as a transaction until you *commit* the changes to the underlying database.

a small example of how to create a database:

    Database *memorydb = new Database(); // create database in memory (deleted upon exit)
    Database *diskdb = new Database("test.db"); // create a database file "test.db"

and here are some samples of how to perform basic **CRUD** tasks with the database:

    // create a table
    db->create("users", [](CreateTransaction* schema)
    {
        schema->integer("id")->increments()->primary();
        schema->varchar("username")->required();
        schema->varchar("password")->required();
	})->gaurd(); // the 'gaurd' call is optional but makes sure IF NOT EXISTS is added
	
	// insert a data ito the database
	db->insert("users", [](InsertTransaction* table)
	{
		table->insert("username", "user1");
		table->insert("password", "password1");
	});
	
	// update the database
	db->update("users", [](UpdateTransaction* table)
	{
		table->set("username", "username_1");
		table->set("password", "password_1");
		table->where("id", "1")->andWhere("username", "user1");
	});
	
	// and of course lookup data
	db->query([](SelectTransaction* table)
	{
		table->from("users");
		table->select("username");
	});

**WARNING:** please note that all changes remain in memory untill they are commited like this:

    db->commit();

this will execute all pending transactions.

## == ALPHA WARNING ==
this project is under active development and some (*key*) features are still unavailable. for example actually getting the data from a select query isn't implemented yet (coming *soon*)
also note that the checking for errors etc is currently (for the most part) omitted. these features will be implemented later on
