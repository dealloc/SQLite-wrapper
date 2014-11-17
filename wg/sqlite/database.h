#ifndef WG_DATABASE_H
#define WG_DATABASE_H

#include <string>
#include <functional>
#include <stack>
#include <sqlite3.h>
#include <iostream>
#include "transactions/SelectTransaction.h"
#include "transactions/CreateTransaction.h"
#include "transactions/InsertTransaction.h"
#include "transactions/UpdateTransaction.h"
#include "transactions/DeleteTransaction.h"
#include "../wg_utils.h"
#include "exceptions/OpenException.h"
#include "exceptions/CloseException.h"
#include "exceptions/UnknownOperationException.h"
#include "exceptions/QueryException.h"

// when using this as a database name, everything is stored in memory
#define WG_SQLITE_VOLATILE_DB ":memory:"

namespace wg
{
    namespace sqlite
	{
		WG_USE_STRING; // include std::string
		WG_USE(vector);
		
		typedef int(wg_raw_callback)(void*, int, char**, char**);

		using wg::sqlite::transactions::SelectTransaction;
		using wg::sqlite::transactions::select_callback;

		using wg::sqlite::transactions::CreateTransaction;
		using wg::sqlite::transactions::create_callback;

		using wg::sqlite::transactions::InsertTransaction;
		using wg::sqlite::transactions::insert_callback;

		using wg::sqlite::transactions::UpdateTransaction;
		using wg::sqlite::transactions::update_callback;

		using wg::sqlite::transactions::DeleteTransaction;
		using wg::sqlite::transactions::delete_callback;

		using wg::sqlite::exceptions::OpenException;
		using wg::sqlite::exceptions::CloseException;
		using wg::sqlite::exceptions::UnknownOperationException;
		using wg::sqlite::exceptions::QueryException;

        class Database
        {
        public:
			Database(const char* name = WG_SQLITE_VOLATILE_DB); // create a new database with given name (empty for in-memory database)
			Database(const string name); // create a new database with given name (empty for in-memory database)
			~Database(); // destructor; clean up all used resources
			void commit(); // execute all pending transactions
			void execute(const string sql, wg_raw_callback handler); // execute raw SQL queries
			void execute(const string sql, wg_raw_callback handler, void* obj); // execute raw SQL queries and pass object to callback
			SelectTransaction* query(); // push a select query onto the queque
			CreateTransaction* create(string name); // push a create query onto the queque
			InsertTransaction* insert(string name); // push an insert query onto the queque
			UpdateTransaction* update(string name); // push an update query onto the queque
			DeleteTransaction* remove(string name); // push a delete query onto the queque
			const sqlite3* raw() const;
#ifdef WG_Cpp11
			SelectTransaction* query(std::function<void(SelectTransaction*)> callback); // allows for building select queries using lambda callbacks (C++0x or later)
			CreateTransaction* create(string name, std::function<void(CreateTransaction*)> callback); // allows for building create queries using lambda callbacks (C++0x or later)
			InsertTransaction* insert(string name, std::function<void(InsertTransaction*)> callback); // allows for building insert queries using lambda callbacks (C++0x or later)
			UpdateTransaction* update(string name, std::function<void(UpdateTransaction*)> callback); // allows for building update queries using lambda callbacks (C++0x or later)
			DeleteTransaction* remove(string name, std::function<void(DeleteTransaction*)> callback); // allows for building delete queries using lambda callbacks (C++0x or later)
#else
			SelectTransaction* query(void (*callback)(SelectTransaction*)); // allows for building select queries using function pointer callbacks
			CreateTransaction* create(string name, void(*callback)(CreateTransaction*)); // allows for building create queries using function pointer callbacks
			InsertTransaction* insert(string name, void(*callback)(InsertTransaction*)); // allows for building insert queries using function pointer callbacks
			UpdateTransaction* update(string name, void(*callback)(UpdateTransaction*)); // allows for building update queries using function pointer callbacks
			DeleteTransaction* removes(string name, void(*callback)(DeleteTransaction*)); // allows for building delete queries using function pointer callbacks
#endif
		private:
			void exec_query(SelectTransaction* transaction);
			void exec_create(CreateTransaction* transaction);
			void exec_insert(InsertTransaction* transaction);
			void exec_update(UpdateTransaction* transaction);
			void exec_delete(DeleteTransaction* transaction);
			void initialize(); // setup all internal vectors & stuff
			// store transactions
			vector<SelectTransaction*> *_selects = WG_NULL;
			vector<CreateTransaction*> *_creates = WG_NULL;
			vector<InsertTransaction*> *_inserts = WG_NULL;
			vector<UpdateTransaction*> *_updates = WG_NULL;
			vector<DeleteTransaction*> *_deletes = WG_NULL;
			sqlite3* _db = WG_NULL;
			char* _errors = WG_NULL;
			static int _callback(void* resp, int rowc, char** fields, char** columns); // internal hook for SELECT callbacks
			static void _update_hook(void*, int, char const *, char const *, sqlite3_int64); // internal hook for update (INSERT, DELETE & UPDATE)
        };
    }
}

#endif // WG_DATABASE_H
