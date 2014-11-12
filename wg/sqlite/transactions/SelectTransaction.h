#ifndef WG_SELECTTRANSACTION_H
#define WG_SELECTTRANSACTION_H

#include "../../wg_utils.h"
#include "../structs.h"
#include <string>
#include <sstream>
#include <vector>
#include <functional>

namespace wg
{
	namespace sqlite
	{
		namespace transactions
		{
			WG_USE_STRING; // include std::string
			WG_USE(vector);
			WG_USE(stringstream);

#ifdef WG_Cpp11
			typedef std::function<void(void)> select_callback;
#else
			typedef void(*select_callback)(void);
#endif

			class SelectTransaction
			{
			public:
				SelectTransaction();
				~SelectTransaction();
				SelectTransaction* from(string table); // define from which table we query
				SelectTransaction* from(string table, string alias); // defines from which table we query (allows alias)
				SelectTransaction* select(string field); // select which fields we take
				SelectTransaction* where(string field, string eq, string val); // select what field has to match what value
				SelectTransaction* where(string field, string val); // select what field has to match what value
				SelectTransaction* orWhere(string field, string eq, string val); // select what field has to match what value
				SelectTransaction* orWhere(string field, string val); // select what field has to match what value
				SelectTransaction* andWhere(string field, string eq, string val); // select what field has to match what value
				SelectTransaction* andWhere(string field, string val); // select what field has to match what value
				const string build();
				bool hasCallback();
				void callback(select_callback handler);
				select_callback getCallback();
			private:
				select_callback _callback = WG_NULL;
                string _current;
				vector<wg_table*> *_tables = WG_NULL;
				vector<string> *_selects = WG_NULL;
				vector<wg_where*> *_wheres = WG_NULL;
			};
		}
	}
}

#endif
