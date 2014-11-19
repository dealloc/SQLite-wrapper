#ifndef WG_QUERYEXCEPTION_H
#define WG_QUERYEXCEPTION_H

#include <stdexcept>

namespace wg
{
	namespace sqlite
	{
		namespace exceptions
		{
			class QueryException : public std::runtime_error
			{
			public: QueryException(const char* msg) : std::runtime_error(msg) {};
			};
		}
	}
}

#endif