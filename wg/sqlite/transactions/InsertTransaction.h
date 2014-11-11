#ifndef WG_INSERTTRANSACTION_H
#define WG_INSERTTRANSACTION_H

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
			typedef std::function<void(int)> insert_callback;
#else
			typedef void(*insert_callback)(int);
#endif

			class InsertTransaction
			{
			public:
				InsertTransaction(const char* name);
				InsertTransaction(string name);
				~InsertTransaction();
				InsertTransaction* insert(string field, string value);
				InsertTransaction* insert(string field, int value);
				const string build();
				bool hasCallback();
				void callback(insert_callback &handler);
				insert_callback getCallback();
			private:
				insert_callback *_callback = WG_NULL;
				const string _name;
				vector<wg_field*> *_fields;
			};
		}
	}
}

#endif