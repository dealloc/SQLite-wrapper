#ifndef WG_QUERYEXCEPTION_H
#define WG_QUERYEXCEPTION_H

#include <exception>

namespace wg
{
	namespace sqlite
	{
		namespace exceptions
		{
			class QueryException : public std::exception
			{
			public: QueryException(const char* msg) : std::exception(msg) {};
			};
		}
	}
}

#endif