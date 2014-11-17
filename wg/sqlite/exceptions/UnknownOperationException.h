#ifndef WG_UNKNOWNOPERATIONEXCEPTION_H
#define WG_UNKNOWNOPERATIONEXCEPTION_H

#include <exception>

namespace wg
{
	namespace sqlite
	{
		namespace exceptions
		{
			class UnknownOperationException : public std::exception
			{
			public: UnknownOperationException(const char* msg) : std::exception(msg) {};
			};
		}
	}
}

#endif