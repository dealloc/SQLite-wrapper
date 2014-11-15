#ifndef WG_CALLABLE_H
#define WG_CALLABLE_H

#include "../../wg_utils.h"
#include "../structs.h"

namespace wg
{
	namespace sqlite
	{
		namespace base
		{
			template<typename T> class Callable
			{
			public:
				bool hasCallback() { return this->_callback == WG_NULL; }
				void callback(T handler) { this->_callback = handler; }
				T getCallback() { return this->_callback; }
			protected:
				T _callback;
			};
		}
	}
}

#endif