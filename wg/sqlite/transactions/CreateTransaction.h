#ifndef WG_CREATE_TRANSACTION_H
#define WG_CREATE_TRANSACTION_H

#include <string>
#include <vector>
#include <sstream>
#include "../../wg_utils.h"
#include "../builders/column.h"

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
			private:
				vector<Column*> *_columns;
				string _name;
                bool _gaurd;
			};
		}
	}
}

#endif