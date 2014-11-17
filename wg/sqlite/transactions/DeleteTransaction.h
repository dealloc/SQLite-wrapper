#ifndef WG_DELETETRANSACTION_H
#define WG_DELETETRANSACTION_H

#include <string>
#include <sstream>
#include <vector>
#include "../../wg_utils.h"
#include "../structs.h"
#include "../base/Callable.h"
#include "../base/Queryable.h"
#include "../base/Buildable.h"

namespace wg
{
	namespace sqlite
	{
		namespace transactions
		{
			WG_USE_STRING;
			WG_USE(stringstream);
			WG_USE(vector);

#ifdef WG_Cpp11
			typedef std::function<void(int)> delete_callback;
#else
			typedef void(*delete_callback)(int);
#endif

			using wg::sqlite::base::Callable;
			using wg::sqlite::base::Queryable;
			using wg::sqlite::base::Buildable;

			class DeleteTransaction : public Callable<delete_callback>, public Queryable, public Buildable
			{
			public:
				DeleteTransaction(const char* table);
				DeleteTransaction(const string table);
				~DeleteTransaction();
				virtual const string build();
			private:
				const string _name;
				virtual inline void _prefix(string &field);
			};
		}
	}
}

#endif