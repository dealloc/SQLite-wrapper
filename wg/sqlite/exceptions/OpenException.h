#ifndef WG_OPENEXCEPTION_H
#define WG_OPENEXCEPTION_H

#include <exception>

namespace wg
{
	namespace sqlite
	{
		namespace exceptions
		{
			class OpenException : public std::exception
			{
			public: OpenException(const char* msg) : std::exception(msg){};
			};
		}
	}
}

#endif