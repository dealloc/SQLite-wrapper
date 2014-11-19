#ifndef WG_OPENEXCEPTION_H
#define WG_OPENEXCEPTION_H

#include <stdexcept>

namespace wg
{
	namespace sqlite
	{
		namespace exceptions
		{
			class OpenException : public std::runtime_error
			{
			public: OpenException(const char* msg) : std::runtime_error(msg){};
			};
		}
	}
}

#endif