#ifndef WG_UPDATETRANSACTION_H
#define WG_UPDATETRANSACTION_H

#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include "../../wg_utils.h"
#include "../structs.h"

namespace wg
{
	namespace sqlite
	{
		namespace transactions
		{
			WG_USE_STRING;
			WG_USE(vector);
			WG_USE(stringstream);

#ifdef WG_Cpp11
			typedef std::function<void(int)> update_callback;
#else
			typedef void(*update_callback)(int);
#endif

			class UpdateTransaction
			{
			public:
				UpdateTransaction(const char* name);
				UpdateTransaction(const string name);
				~UpdateTransaction();
				UpdateTransaction* set(string name, string value);
				UpdateTransaction* where(string field, string eq, string val); // select what field has to match what value
				UpdateTransaction* where(string field, string val); // select what field has to match what value
				UpdateTransaction* orWhere(string field, string eq, string val); // select what field has to match what value
				UpdateTransaction* orWhere(string field, string val); // select what field has to match what value
				UpdateTransaction* andWhere(string field, string eq, string val); // select what field has to match what value
				UpdateTransaction* andWhere(string field, string val); // select what field has to match what value
				const string build();
				bool hasCallback();
				void callback(update_callback handler);
				update_callback getCallback();
			private:
				update_callback _callback;
				const string _name;
				vector<wg_field*> *_fields = WG_NULL;
				vector<wg_where*> *_wheres = WG_NULL;
			};
		}
	}
}

#endif