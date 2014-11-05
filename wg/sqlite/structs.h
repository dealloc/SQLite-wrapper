#ifndef WG_STRUCTS_H
#define WG_STRUCTS_H

#include <string>
#include "../wg_utils.h"

// get the alias of the table, or it's name if no alias is defined
#define WG_TABLE_NAME(table) (table.alias == "" ? table.name : table.alias)

// get the alias of the table, or it's name if no alias is defined
#define WG_TABLE_PTR_NAME(table) (table->alias == "" ? table->name : table->alias)

namespace wg
{
	namespace sqlite
	{
		WG_USE_STRING;

		typedef struct wg_sqlite_table
		{
			string name;
            string alias;
		} wg_table;

		typedef struct wg_sqlite_where
		{
			string field;
            string eq;
			string value;
            string pref;
		} wg_where;

		typedef struct wg_sqlite_field
		{
			string field;
			string value;
		} wg_field;
	}
}

#endif
