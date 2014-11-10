#ifndef WG_DATABASE_H
#define WG_DATABASE_H

#include <string>
#include <functional>
#include <vector>
#include <map>
#include <sqlite3.h>
#include <iostream>
#include "transactions/SelectTransaction.h"
#include "transactions/CreateTransaction.h"
#include "transactions/InsertTransaction.h"
#include "transactions/UpdateTransaction.h"
#include "../wg_utils.h"

// when using this as a database name, everything is stored in memory
#define WG_SQLITE_VOLATILE_DB ":memory:"

namespace wg
{
    namespace sqlite
	{
		WG_USE_STRING; // include std::string
		WG_USE(vector);
		WG_USE(map);
#ifdef WG_Cpp11
        WG_USE(function);
#endif
		
		typedef std::vector<std::map<string, string>>* resultset;

		using wg::sqlite::transactions::SelectTransaction;
		using wg::sqlite::transactions::CreateTransaction;
		using wg::sqlite::transactions::InsertTransaction;
		using wg::sqlite::transactions::UpdateTransaction;

        class Database
        {
        public:
			Database(const char* name = WG_SQLITE_VOLATILE_DB); // create a new database with given name (empty for in-memory database)
			Database(const string name); // create a new database with given name (empty for in-memory database)
			~Database(); // destructor; clean up all used resources
			void commit(); // execute all pending transactions
			void execute(const string sql); // execute raw SQL queries
			SelectTransaction* query(); // push a select query onto the queque
			CreateTransaction* create(string name); // push a create query onto the queque
			InsertTransaction* insert(string name); // push an insert query onto the queque
			UpdateTransaction* update(string name); // push an update query onto the queque
#ifdef WG_Cpp11
			SelectTransaction* query(function<void(SelectTransaction*)> callback); // allows for building select queries using lambda callbacks (C++0x or later)
			CreateTransaction* create(string name, function<void(CreateTransaction*)> callback); // allows for building create queries using lambda callbacks (C++0x or later)
			InsertTransaction* insert(string name, function<void(InsertTransaction*)> callback); // allows for building insert queries using lambda callbacks (C++0x or later)
			UpdateTransaction* update(string name, function<void(UpdateTransaction*)> callback); // allows for building update queries using lambda callbacks (C++0x or later)
#else
			SelectTransaction* query(void (*callback)(SelectTransaction*)); // allows for building select queries using function pointer callbacks
			CreateTransaction* create(string name, void(*callback)(CreateTransaction*)); // allows for building create queries using function pointer callbacks
			InsertTransaction* insert(string name, void(*callback)(InsertTransaction*)); // allows for building insert queries using function pointer callbacks
			UpdateTransaction* update(string name, void(*callback)(UpdateTransaction*)); // allows for building update queries using function pointer callbacks
#endif
		private:
			vector<SelectTransaction*> *_selects;
			vector<CreateTransaction*> *_creates;
			vector<InsertTransaction*> *_inserts;
			vector<UpdateTransaction*> *_updates;
			sqlite3* _db = WG_NULL;
			char* _errors = WG_NULL;
            static int _callback(void* resp, int rowc, char** fields, char** columns); // general callback for SQLite
        };
    }
}

#endif // WG_DATABASE_H
