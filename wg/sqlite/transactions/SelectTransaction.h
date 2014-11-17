#ifndef WG_SELECTTRANSACTION_H
#define WG_SELECTTRANSACTION_H

#include "../../wg_utils.h"
#include "../structs.h"
#include "../base/Queryable.h"
#include "../base/Callable.h"
#include "../base/Buildable.h"
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

			using wg::sqlite::base::Queryable;
			using wg::sqlite::base::Callable;
			using wg::sqlite::base::Buildable;

#ifdef WG_Cpp11
			typedef std::function<void(void)> select_callback;
#else
			typedef void(*select_callback)(void);
#endif

			class SelectTransaction : public Queryable, public Callable<select_callback>, public Buildable
			{
			public:
				SelectTransaction();
				~SelectTransaction();
				SelectTransaction* from(string table); // define from which table we query
				SelectTransaction* from(string table, string alias); // defines from which table we query (allows alias)
				SelectTransaction* select(string field); // select which fields we take
				virtual const string build();
			private:
				virtual inline void _prefix(string &field);
                string _current;
				vector<wg_table*> *_tables = WG_NULL;
				vector<string> *_selects = WG_NULL;
			};
		}
	}
}

#endif
