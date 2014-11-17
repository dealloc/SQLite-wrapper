#ifndef WG_CREATE_TRANSACTION_H
#define WG_CREATE_TRANSACTION_H

#include <string>
#include <vector>
#include <sstream>
#include "../../wg_utils.h"
#include "../builders/column.h"
#include "../base/Callable.h"
#include "../base/Buildable.h"
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
			using wg::sqlite::base::Callable;
			using wg::sqlite::base::Buildable;

#ifdef WG_Cpp11
			typedef std::function<void(string)> create_callback;
#else
			typedef void(*create_callback)(string);
#endif

			class CreateTransaction : public Callable<create_callback>, public Buildable
			{
			public:
				CreateTransaction(const char* table);
				CreateTransaction(string table);
				~CreateTransaction();
				const string which();
				CreateTransaction* gaurd();
				Column* varchar(string field);
				Column* integer(string field);
				Column* boolean(string field);
				virtual const string build();
			private:
				vector<Column*> *_columns = WG_NULL;
				string _name;
                bool _gaurd;
			};
		}
	}
}

#endif
