#ifndef WG_QUERYABLE_H
#define WG_QUERYABLE_H

#include "../../wg_utils.h"
#include "../structs.h"
#include <string>
#include <vector>

namespace wg
{
	namespace sqlite
	{
		namespace base
		{
			WG_USE_STRING;
			WG_USE(vector);

			class Queryable
			{
			public:
				Queryable();
				~Queryable();
				Queryable* where(string field, string eq, string val); // select what field has to match what value
				Queryable* where(string field, string val); // select what field has to match what value
				Queryable* orWhere(string field, string eq, string val); // select what field has to match what value
				Queryable* orWhere(string field, string val); // select what field has to match what value
				Queryable* andWhere(string field, string eq, string val); // select what field has to match what value
				Queryable* andWhere(string field, string val); // select what field has to match what value
			protected:
				virtual inline void _prefix(string &field) = 0;
				vector<wg_where*> *_wheres = WG_NULL;
			};
		}
	}
}

#endif