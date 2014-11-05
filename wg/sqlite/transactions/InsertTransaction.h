#ifndef WG_INSERTTRANSACTION_H
#define WG_INSERTTRANSACTION_H

#include <string>
#include <vector>
#include <sstream>
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

			class InsertTransaction
			{
			public:
				InsertTransaction(const char* name);
				InsertTransaction(string name);
				~InsertTransaction();
				InsertTransaction* insert(string field, string value);
				InsertTransaction* insert(string field, int value);
				const string build();
			private:
				const string _name;
				vector<wg_field*> *_fields;
			};
		}
	}
}

#endif