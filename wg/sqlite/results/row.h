#ifndef WG_ROW_H
#define WG_ROW_H

#include <string>
#include <map>
using std::string;
using std::map;

namespace wg
{
	namespace sqlite
	{
		namespace results
		{
			class Row
			{
			public:
				Row();
				~Row();
				string& operator[](const string column);
			private:
				map<string, string> *_columns;
			};
		}
	}
}

#endif