#ifndef WG_COLUMN_H
#define WG_COLUMN_H

#include <string>
#include <sstream>
#include "../../wg_utils.h"

namespace wg
{
    namespace sqlite
    {
        namespace builders
		{
			WG_USE_STRING; // include std::string
			WG_USE(stringstream);

            class Column
            {
            public:
				Column(const char* name, const char* type);
				Column(const string name, const string type);
				const string getName();
				const string getType();
				Column* primary();
				Column* increments();
				Column* required();
				Column* unique();
                Column* ifNull(string name);
				bool getPrimary();
				bool getIncrements();
				bool getRequired();
				bool getUnique();
				string getDefault();
				const string build();
			private:
				const string _name, _type;
                bool _primary;
                bool _increments;
                bool _notnull;
                bool _unique;
				string _default;
            };
        }
    }
}

#endif // WG_COLUMN_H
