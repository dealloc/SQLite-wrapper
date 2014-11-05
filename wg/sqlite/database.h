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
#include "results/row.h"
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
		using wg::sqlite::results::Row;

        class Database
        {
        public:
			Database(const char* name = WG_SQLITE_VOLATILE_DB);
			Database(const string name);
			~Database();
			void commit();
			void execute(const string sql);
			SelectTransaction* query();
			CreateTransaction* create(string name);
			InsertTransaction* insert(string name);
			UpdateTransaction* update(string name);
#ifdef WG_Cpp11
			SelectTransaction* query(function<void(SelectTransaction*)> callback);
			CreateTransaction* create(string name, function<void(CreateTransaction*)> callback);
			InsertTransaction* insert(string name, function<void(InsertTransaction*)> callback);
			UpdateTransaction* update(string name, function<void(UpdateTransaction*)> callback);
#else
			SelectTransaction* query(void(callback*)(SelectTransaction*));
#endif
        private:
			vector<SelectTransaction*> *_selects;
			vector<CreateTransaction*> *_creates;
			vector<InsertTransaction*> *_inserts;
			vector<UpdateTransaction*> *_updates;
			sqlite3* _db = NULL;
			char* _errors = NULL;
			bool _expecting = false; // we're not expecing any queries right now
            static int _callback(void* resp, int rowc, char** fields, char** columns);
        };
    }
}

#endif // WG_DATABASE_H
