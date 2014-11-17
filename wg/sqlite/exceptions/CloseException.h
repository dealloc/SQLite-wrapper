#ifndef WG_CLOSEEXCEPTION_H
#define WG_CLOSEEXCEPTION_H

#include <exception>

namespace wg
{
	namespace sqlite
	{
		namespace exceptions
		{
			class CloseException : public std::exception
			{
			public: CloseException(const char* msg) : std::exception(msg) {};
			};
		}
	}
}

#endif