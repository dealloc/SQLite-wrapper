#ifndef WG_UNKNOWNOPERATIONEXCEPTION_H
#define WG_UNKNOWNOPERATIONEXCEPTION_H

#include <stdexcept>

namespace wg
{
	namespace sqlite
	{
		namespace exceptions
		{
			class UnknownOperationException : public std::runtime_error
			{
			public: UnknownOperationException(const char* msg) : std::runtime_error(msg) {};
			};
		}
	}
}

#endif