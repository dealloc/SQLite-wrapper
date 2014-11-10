#ifndef WG_CREATE_TRANSACTION_H
#define WG_CREATE_TRANSACTION_H

#include <string>
#include <vector>
#include <sstream>
#include "../../wg_utils.h"
#include "../builders/column.h"
#include <functional>

#define WG_SQLITE_VARCHAR "TEXT"
#define WG_SQLITE_INTEGER "INTEGER"
#define WG_SQLITE_BOOLEAN WG_SQLITE_INTEGER

namespace wg
{
	namespace sqlite
	{
		namespace transactions
		{
			WG_USE_STRING;
			WG_USE(vector);
			WG_USE(stringstream);
			using wg::sqlite::builders::Column;

#ifdef WG_Cpp11
			typedef std::function<void(int, string)> create_callback;
#else
			//TODO write function pointer typedef
#endif

			class CreateTransaction
			{
			public:
				CreateTransaction(const char* table);
				CreateTransaction(string table);
				~CreateTransaction();
				CreateTransaction* gaurd();
				Column* varchar(string field);
				Column* integer(string field);
				Column* boolean(string field);
				const string build();
				bool hasCallback();
				void callback(create_callback &handler);
			private:
				create_callback *_callback = WG_NULL;
				vector<Column*> *_columns;
				string _name;
                bool _gaurd;
			};
		}
	}
}

#endif
