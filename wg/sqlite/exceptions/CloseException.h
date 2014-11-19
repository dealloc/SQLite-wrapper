#ifndef WG_CLOSEEXCEPTION_H
#define WG_CLOSEEXCEPTION_H

#include <stdexcept>

namespace wg
{
	namespace sqlite
	{
		namespace exceptions
		{
			class CloseException : public std::runtime_error
			{
			public: CloseException(const char* msg) : std::runtime_error(msg) {};
			};
		}
	}
}

#endif