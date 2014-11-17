#ifndef WG_UPDATETRANSACTION_H
#define WG_UPDATETRANSACTION_H

#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include "../../wg_utils.h"
#include "../structs.h"
#include "../base/Queryable.h"
#include "../base/Callable.h"
#include "../base/Buildable.h"

namespace wg
{
	namespace sqlite
	{
		namespace transactions
		{
			WG_USE_STRING;
			WG_USE(vector);
			WG_USE(stringstream);

			using wg::sqlite::base::Queryable;
			using wg::sqlite::base::Callable;
			using wg::sqlite::base::Buildable;

#ifdef WG_Cpp11
			typedef std::function<void(int)> update_callback;
#else
			typedef void(*update_callback)(int);
#endif

			class UpdateTransaction : public Queryable, public Callable<update_callback>, public Buildable
			{
			public:
				UpdateTransaction(const char* name);
				UpdateTransaction(const string name);
				~UpdateTransaction();
				UpdateTransaction* set(string name, string value);
				virtual const string build();
			private:
				virtual inline void _prefix(string &field);
				const string _name;
				vector<wg_field*> *_fields = WG_NULL;
			};
		}
	}
}

#endif