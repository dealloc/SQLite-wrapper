#ifndef WG_INSERTTRANSACTION_H
#define WG_INSERTTRANSACTION_H

#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include "../../wg_utils.h"
#include "../structs.h"
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

			using wg::sqlite::base::Callable;
			using wg::sqlite::base::Buildable;

#ifdef WG_Cpp11
			typedef std::function<void(int)> insert_callback;
#else
			typedef void(*insert_callback)(int);
#endif

			class InsertTransaction : public Callable<insert_callback>, public Buildable
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
				vector<wg_field*> *_fields = WG_NULL;
			};
		}
	}
}

#endif